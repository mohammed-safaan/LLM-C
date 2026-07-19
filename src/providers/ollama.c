#include "../buffer.h"
#include "../http.h"
#include "../json.h"
#include "../llm_internal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *role_to_string(LLMRole role) {
  switch (role) {
  case LLM_ROLE_SYSTEM:
    return "system";
  case LLM_ROLE_USER:
    return "user";
  case LLM_ROLE_ASSISTANT:
    return "assistant";
  default:
    return NULL;
  }
}

static int build_chat_request(const LLMRequest *request, char **body) {
  if (request == NULL || body == NULL) {
    return -1;
  }

  if (request->model == NULL) {
    return -1;
  }

  if (request->messages == NULL && request->message_count != 0) {
    return -1;
  }

  cJSON *root = json_create_object();

  if (root == NULL) {
    return -1;
  }

  cJSON_AddStringToObject(root, "model", request->model);

  cJSON *messages = cJSON_AddArrayToObject(root, "messages");
  if (messages == NULL) {
    json_destroy(root);
    return -1;
  }

  for (size_t i = 0; i < request->message_count; i++) {
    cJSON *msg = cJSON_CreateObject();
    if (msg == NULL) {
      json_destroy(root);
      return -1;
    }

    const char *role = role_to_string(request->messages[i].role);

    if (role == NULL || request->messages[i].content == NULL) {
      json_destroy(root);
      return -1;
    }
    cJSON_AddStringToObject(msg, "role", role);
    cJSON_AddStringToObject(msg, "content", request->messages[i].content);

    cJSON_AddItemToArray(messages, msg);
  }

  cJSON_AddBoolToObject(root, "stream", 0);

  *body = json_print(root);
  json_destroy(root);

  return (*body != NULL) ? 0 : -1;
}

static int parse_chat_response(const HttpResponse *response,
                               LLMResponse *result) {
  if (response == NULL || result == NULL)
    return -1;

  cJSON *root = NULL;

  if (json_parse(response->body.data, &root) != 0)
    return -1;

  cJSON *message = cJSON_GetObjectItemCaseSensitive(root, "message");
  if (!cJSON_IsObject(message)) {
    json_destroy(root);
    return -1;
  }

  const char *content = json_get_string(message, "content");
  if (content == NULL) {
    json_destroy(root);
    return -1;
  }

  result->text = strdup(content);

  if (result->text == NULL) {
    json_destroy(root);
    return -1;
  }

  double prompt_tokens_value;

  if (json_get_number(root, "prompt_eval_count", &prompt_tokens_value) == 0) {
    result->prompt_tokens = (int)prompt_tokens_value;
  }

  double completion_tokens_value;

  if (json_get_number(root, "eval_count", &completion_tokens_value) == 0) {
    result->completion_tokens = (int)completion_tokens_value;
  }

  result->total_tokens = result->prompt_tokens + result->completion_tokens;

  const char *reason = json_get_string(root, "done_reason");
  if (reason != NULL) {
    result->finish_reason = strdup(reason);
    if (result->finish_reason == NULL) {
      json_destroy(root);
      return -1;
    }
  }

  json_destroy(root);

  return 0;
}
static int build_chat_url(const LLMClient *client, Buffer *url) {
  if (client == NULL || url == NULL) {
    return -1;
  }

  if (buffer_append(url, client->base_url, strlen(client->base_url)) != 0) {
    return -1;
  }

  if (buffer_append(url, "/api/chat", strlen("/api/chat")) != 0) {
    return -1;
  }

  return 0;
}

static int ollama_chat(const LLMClient *client, const LLMRequest *request,
                       LLMResponse *response) {
  if (client == NULL || request == NULL || response == NULL)
    return -1;

  int ret = -1;
  char *body = NULL;
  HttpRequest http_request;
  HttpResponse http_response;
  int request_initialized = 0;
  int response_initialized = 0;

  Buffer url_buffer;
  buffer_init(&url_buffer);

  ret = build_chat_request(request, &body);
  if (ret != 0) {
    goto cleanup;
  }

  if (http_request_init(&http_request) != 0) {
    ret = -1;
    goto cleanup;
  }
  request_initialized = 1;

  if (http_response_init(&http_response) != 0) {
    ret = -1;
    goto cleanup;
  }
  response_initialized = 1;

  if (build_chat_url(client, &url_buffer) != 0) {
    ret = -1;
    goto cleanup;
  }

  http_request.method = HTTP_POST;
  http_request.url = url_buffer.data;
  http_request.body = body;

  ret =
      http_request_add_header(&http_request, "Content-Type: application/json");
  if (ret != 0) {
    goto cleanup;
  }

  if (client->api_key != NULL) {
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s",
             client->api_key);
    ret = http_request_add_header(&http_request, auth_header);
    if (ret != 0) {
      goto cleanup;
    }
  }
  ret = http_execute(&http_request, &http_response);
  if (ret == 0) {
    if (http_response.status != 200) {
      ret = -1;
      goto cleanup;
    }
    ret = parse_chat_response(&http_response, response);
  }

cleanup:
  free(body);
  if (request_initialized) {
    http_request_destroy(&http_request);
  }
  if (response_initialized) {
    http_response_destroy(&http_response);
  }
  buffer_destroy(&url_buffer);
  return ret;
}

const LLMProvider ollama_provider = {.chat = ollama_chat};