#include "http.h"
#include "buffer.h"
#include <curl/curl.h>
#include <curl/easy.h>

void http_cleanup(void) { curl_global_cleanup(); }

int http_init(void) {
  CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
  if (result != CURLE_OK) {
    return -1;
  }
  return 0;
}

int http_request_add_header(HttpRequest *request, const char *header) {
  if (request == NULL || header == NULL) {
    return -1;
  }

  struct curl_slist *list = curl_slist_append(request->headers, header);

  if (list == NULL)
    return -1;

  request->headers = list;

  return 0;
}

int http_request_init(HttpRequest *request) {
  if (request == NULL) {
    return -1;
  }

  request->method = HTTP_GET;
  request->url = NULL;
  request->body = NULL;
  request->headers = NULL;

  return 0;
}

static int http_configure_method(CURL *curl, HttpMethod method) {
  switch (method) {
  case HTTP_GET:
    return 0;

  case HTTP_POST:
    return curl_easy_setopt(curl, CURLOPT_POST, 1L) == CURLE_OK ? 0 : -1;

  case HTTP_PUT:
    return curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT") == CURLE_OK
               ? 0
               : -1;

  case HTTP_DELETE:
    return curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE") == CURLE_OK
               ? 0
               : -1;

  case HTTP_PATCH:
    return curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH") == CURLE_OK
               ? 0
               : -1;

  default:
    return -1;
  }
}

void http_request_destroy(HttpRequest *request) {
  if (request == NULL) {
    return;
  }

  curl_slist_free_all(request->headers);
  request->headers = NULL;
}

static size_t write_callback(void *contents, size_t size, size_t nmemb,
                             void *userp) {
  Buffer *buffer = userp;
  size_t bytes = size * nmemb;

  if (buffer_append(buffer, contents, bytes) != 0) {
    return 0;
  }
  return bytes;
}

int http_response_init(HttpResponse *response) {
  if (response == NULL) {
    return -1;
  }

  response->status = 0;
  buffer_init(&response->body);

  return 0;
}

void http_response_destroy(HttpResponse *response) {
  if (response == NULL) {
    return;
  }

  buffer_destroy(&response->body);
}

int http_execute(const HttpRequest *request, HttpResponse *response) {
  if (response == NULL || request == NULL || request->url == NULL) {
    return -1;
  }
  CURL *curl = curl_easy_init();
  if (curl == NULL) {
    return -1;
  }

  curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
  curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
  curl_easy_setopt(curl, CURLOPT_URL, request->url);

  if (http_configure_method(curl, request->method) != 0) {
    curl_easy_cleanup(curl);
    return -1;
  }

  if (request->body != NULL) {
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request->body);
  }

  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request->headers);

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response->body);

  CURLcode result = curl_easy_perform(curl);
  if (result == CURLE_OK) {
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->status);
  }

  curl_easy_cleanup(curl);

  if (result != CURLE_OK) {
    fprintf(stderr, "libcurl error: %s\n", curl_easy_strerror(result));
    return -1;
  }
  return 0;
}