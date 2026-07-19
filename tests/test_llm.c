#include "../include/llm/llm.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_llm_response_init(void) {
    LLMResponse response;

    assert(llm_response_init(&response) == 0);

    assert(response.text == NULL);
    assert(response.finish_reason == NULL);

    assert(response.prompt_tokens == 0);
    assert(response.completion_tokens == 0);
    assert(response.total_tokens == 0);
}

static void test_llm_response_init_invalid(void) {
    assert(llm_response_init(NULL) == -1);
}

static void test_llm_response_destroy(void) {
    LLMResponse response;

    assert(llm_response_init(&response) == 0);

    response.text = strdup("Hello");
    response.finish_reason = strdup("stop");

    assert(response.text != NULL);
    assert(response.finish_reason != NULL);

    llm_response_destroy(&response);

    assert(response.text == NULL);
    assert(response.finish_reason == NULL);
}

static void test_llm_response_destroy_empty(void) {
    LLMResponse response;

    assert(llm_response_init(&response) == 0);

    llm_response_destroy(&response);

    assert(response.text == NULL);
    assert(response.finish_reason == NULL);
}

static void test_llm_response_destroy_null(void) {
    llm_response_destroy(NULL);
}


int main(void) {
    test_llm_response_init();
    test_llm_response_init_invalid();

    test_llm_response_destroy();
    test_llm_response_destroy_empty();
    test_llm_response_destroy_null();

    printf("All LLM tests passed.\n");
    return 0;
}