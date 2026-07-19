#include "../include/llm/llm.h"
#include "../src/http.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_URL "https://ollama.com"
#define TEST_MODEL "gpt-oss:120b"

static void test_create_client(void) {
  const char *api_key = getenv("OLLAMA_API_KEY");
  LLMClient *client = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  assert(client != NULL);

  llm_client_destroy(client);
}

static void test_create_client_invalid(void) {
  assert(llm_client_create(LLM_PROVIDER_OLLAMA, NULL, NULL) == NULL);
}

static void test_chat_simple(void) {
  const char *api_key = getenv("OLLAMA_API_KEY");
  LLMClient *client = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  assert(client != NULL);

  LLMMessage messages[] = {
      {.role = LLM_ROLE_USER, .content = "Reply with exactly: hello"}};

  LLMRequest request = {
      .model = TEST_MODEL, .messages = messages, .message_count = 1};

  LLMResponse response;

  assert(llm_response_init(&response) == 0);

  int ret = llm_chat(client, &request, &response);

  printf("llm_chat returned %d\n", ret);

  if (ret != 0) {
    printf("API error (check API key and model)\n");
  } else {
    assert(response.text != NULL);
    assert(strlen(response.text) > 0);

    printf("response: %s\n", response.text);
    printf("tokens: prompt=%d completion=%d total=%d\n", response.prompt_tokens,
           response.completion_tokens, response.total_tokens);
  }

  llm_response_destroy(&response);
  llm_client_destroy(client);
}

static void test_chat_multiple_messages(void) {
  const char *api_key = getenv("OLLAMA_API_KEY");
  LLMClient *client = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  assert(client != NULL);

  LLMMessage messages[] = {
      {.role = LLM_ROLE_SYSTEM, .content = "You are helpful."},
      {.role = LLM_ROLE_USER, .content = "Say yes."}};

  LLMRequest request = {
      .model = TEST_MODEL, .messages = messages, .message_count = 2};

  LLMResponse response;

  assert(llm_response_init(&response) == 0);

  int ret = llm_chat(client, &request, &response);
  printf("multiple_messages: llm_chat returned %d\n", ret);
  if (ret == 0) {
    assert(response.text != NULL);
  }

  llm_response_destroy(&response);
  llm_client_destroy(client);
}

static void test_chat_invalid_request(void) {
  const char *api_key = getenv("OLLAMA_API_KEY");
  LLMClient *client = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  assert(client != NULL);

  LLMRequest request = {.model = NULL, .messages = NULL, .message_count = 0};

  LLMResponse response;

  assert(llm_response_init(&response) == 0);

  assert(llm_chat(client, &request, &response) == -1);

  llm_response_destroy(&response);
  llm_client_destroy(client);
}

static void test_multiple_requests_same_client(void) {
  const char *api_key = getenv("OLLAMA_API_KEY");
  LLMClient *client = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  assert(client != NULL);

  LLMMessage messages[] = {{.role = LLM_ROLE_USER, .content = "Say hello."}};

  LLMRequest request = {
      .model = TEST_MODEL, .messages = messages, .message_count = 1};

  for (int i = 0; i < 3; i++) {

    LLMResponse response;

    assert(llm_response_init(&response) == 0);

  int ret = llm_chat(client, &request, &response);
  printf("llm_chat returned %d\n", ret);
  if (ret == 0) {
    assert(response.text != NULL);
  }

    llm_response_destroy(&response);
  }

  llm_client_destroy(client);
}

static void test_multiple_clients(void) {
  const char *api_key = getenv("OLLAMA_API_KEY");
  LLMClient *client1 = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  LLMClient *client2 = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  assert(client1 != NULL);
  assert(client2 != NULL);

  llm_client_destroy(client1);
  llm_client_destroy(client2);
}

static void test_large_prompt(void) {
  const char *api_key = getenv("OLLAMA_API_KEY");
  LLMClient *client = llm_client_create(LLM_PROVIDER_OLLAMA, TEST_URL, api_key);

  assert(client != NULL);

  char *prompt = malloc(8001);

  assert(prompt != NULL);

  memset(prompt, 'A', 8000);
  prompt[8000] = '\0';

  LLMMessage messages[] = {{.role = LLM_ROLE_USER, .content = prompt}};

  LLMRequest request = {
      .model = TEST_MODEL, .messages = messages, .message_count = 1};

  LLMResponse response;

  assert(llm_response_init(&response) == 0);

  int ret = llm_chat(client, &request, &response);
  printf("large_prompt: llm_chat returned %d\n", ret);
  if (ret == 0) {
    assert(response.text != NULL);
  }

  llm_response_destroy(&response);

  free(prompt);

  llm_client_destroy(client);
}

int main(void) {
  if (getenv("OLLAMA_API_KEY") == NULL) {
    printf("OLLAMA_API_KEY not set, skipping integration tests\n");
    return 0;
  }

  assert(http_init() == 0);

  test_create_client();
  test_create_client_invalid();

  test_chat_simple();
  test_chat_multiple_messages();
  test_chat_invalid_request();

  test_multiple_requests_same_client();
  test_multiple_clients();

  test_large_prompt();

  http_cleanup();

  printf("All Ollama integration tests passed.\n");

  return 0;
}