#ifndef LLM_HTTP_H
#define LLM_HTTP_H

#include "buffer.h"

typedef enum {
  HTTP_GET,
  HTTP_POST,
  HTTP_PUT,
  HTTP_DELETE,
  HTTP_PATCH
} HttpMethod;

typedef struct {
  HttpMethod method;
  const char *url;
  const void *body;
  struct curl_slist *headers;
} HttpRequest;
typedef struct {
  long status;
  Buffer body;
} HttpResponse;

int http_init(void);

void http_cleanup(void);

int http_request_init(HttpRequest *request);

int http_response_init(HttpResponse *response);

int http_request_add_header(HttpRequest *request, const char *header);

int http_execute(const HttpRequest *request, HttpResponse *response);

void http_request_destroy(HttpRequest *request) ;

void http_response_destroy(HttpResponse *response);

#endif