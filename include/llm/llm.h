#ifndef LLM_H
#define LLM_H
#include <stddef.h>

typedef enum { LLM_ROLE_SYSTEM, LLM_ROLE_USER, LLM_ROLE_ASSISTANT } LLMRole;

typedef struct LLMClient LLMClient;
typedef struct {
  LLMRole role;
  const char *content;
} LLMMessage;

typedef struct {
  const char *model;

  LLMMessage *messages;
  size_t message_count;
} LLMRequest;

typedef struct {
  char *text;

  int prompt_tokens;
  int completion_tokens;
  int total_tokens;

  char *finish_reason;
} LLMResponse;

typedef enum { LLM_PROVIDER_OLLAMA } LLMProviderType;

LLMClient *llm_client_create(LLMProviderType provider, const char *base_url,
                             const char *api_key);

void llm_client_destroy(LLMClient *client);

int llm_chat(const LLMClient *client, const LLMRequest *request,
             LLMResponse *response);

int llm_response_init(LLMResponse *response);

void llm_response_destroy(LLMResponse *response);

#endif