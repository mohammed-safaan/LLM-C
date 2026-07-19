#include "llm/llm.h"
#include "llm_internal.h"
#include "providers/ollama.h"
#include <stdlib.h>
#include <string.h>

LLMClient *llm_client_create(LLMProviderType provider, const char *base_url,
                             const char *api_key) {
  if (base_url == NULL)
    return NULL;

  LLMClient *client = malloc(sizeof(*client));

  if (client == NULL)
    return NULL;

  client->provider = NULL;
  client->base_url = NULL;
  client->api_key = NULL;

  switch (provider) {
  case LLM_PROVIDER_OLLAMA:
    client->provider = &ollama_provider;
    break;

  default:
    free(client);
    return NULL;
  }

  client->base_url = strdup(base_url);

  if (client->base_url == NULL) {
    free(client);
    return NULL;
  }

  if (api_key != NULL) {
    client->api_key = strdup(api_key);

    if (client->api_key == NULL) {
      free(client->base_url);
      free(client);
      return NULL;
    }
  }

  return client;
}

int llm_response_init(LLMResponse *response) {
  if (response == NULL)
    return -1;

  response->text = NULL;
  response->prompt_tokens = 0;
  response->completion_tokens = 0;
  response->total_tokens = 0;
  response->finish_reason = NULL;

  return 0;
}

int llm_chat(const LLMClient *client, const LLMRequest *request,
             LLMResponse *response) {
  if (client == NULL || request == NULL || response == NULL)
    return -1;

  if (client->provider == NULL)
    return -1;

  return client->provider->chat(client, request, response);
}

void llm_response_destroy(LLMResponse *response) {
  if (response == NULL)
    return;

  free(response->text);
  response->text = NULL;

  free(response->finish_reason);
  response->finish_reason = NULL;
}

void llm_client_destroy(LLMClient *client) {
  if (client == NULL)
    return;

  free(client->base_url);
  free(client->api_key);

  free(client);
}
