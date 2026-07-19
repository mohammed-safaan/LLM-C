#ifndef LLM_INTERNAL_H
#define LLM_INTERNAL_H

#include "../include/llm/llm.h"

typedef struct {
  int (*chat)(const LLMClient *client, const LLMRequest *request,
              LLMResponse *response);
} LLMProvider;

struct LLMClient {
  const LLMProvider *provider;

  char *base_url;
  char *api_key;
};

#endif