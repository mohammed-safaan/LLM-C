#include "../src/http.h"

#include <assert.h>
#include <stdio.h>

static void test_http_request_init(void) {
  HttpRequest request;

  assert(http_request_init(&request) == 0);

  assert(request.method == HTTP_GET);
  assert(request.url == NULL);
  assert(request.body == NULL);
  assert(request.headers == NULL);

  http_request_destroy(&request);
}

static void test_http_response_init(void) {
  HttpResponse response;

  assert(http_response_init(&response) == 0);

  assert(response.status == 0);
  assert(response.body.data == NULL);
  assert(response.body.length == 0);
  assert(response.body.capacity == 0);

  http_response_destroy(&response);
}

static void test_http_add_header(void) {
  HttpRequest request;

  assert(http_request_init(&request) == 0);

  assert(http_request_add_header(&request, "Content-Type: application/json") ==
         0);

  assert(http_request_add_header(&request,
                                 "Authorization: Bearer test-token") == 0);

  assert(request.headers != NULL);

  http_request_destroy(&request);
}

static void test_http_invalid_arguments(void) {
  HttpRequest request;
  HttpResponse response;

  assert(http_request_init(&request) == 0);
  assert(http_response_init(&response) == 0);

  // assert(http_execute(NULL, &response) == -1);
  // assert(http_execute(&request, NULL) == -1);

  request.url = NULL;
  request.method = HTTP_POST;
  request.body = "{}";

  assert(http_execute(&request, &response) == -1);

  http_request_destroy(&request);
  http_response_destroy(&response);
}

static void test_http_post(void) {
  HttpRequest request;
  HttpResponse response;

  assert(http_request_init(&request) == 0);
  assert(http_response_init(&response) == 0);

  request.method = HTTP_POST;
  request.url = "https://httpbin.org/post";
  request.body = "{\"key\":\"value\"}";

  assert(http_request_add_header(&request, "Content-Type: application/json") ==
         0);

  int result = http_execute(&request, &response);
  printf("result=%d status=%ld\n", result, response.status);
  printf("response body: %.*s\n", (int)response.body.length,
         (char *)response.body.data);
  printf("body=\n%s\n", response.body.data);
  assert(result == 0);
  assert(response.status >= 200);
  assert(response.status < 600);
  assert(response.body.length > 0);

  /* Ensure the request object can be reused */
  http_response_destroy(&response);
  assert(http_response_init(&response) == 0);

  assert(http_execute(&request, &response) == 0);
  assert(response.status == 200);

  http_request_destroy(&request);
  http_response_destroy(&response);
}

int main(void) {
  assert(http_init() == 0);

  test_http_request_init();
  test_http_response_init();
  test_http_add_header();
  test_http_invalid_arguments();
  test_http_post();

  http_cleanup();

  printf("All HTTP tests passed.\n");

  return 0;
}