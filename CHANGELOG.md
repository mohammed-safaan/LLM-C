# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.0] - 2025-07-19

### Added

- Core LLM SDK with pluggable provider architecture
- Ollama provider with chat API support (`/api/chat`)
- HTTP client layer using libcurl (GET, POST, PUT, DELETE, PATCH)
- JSON wrapper module over cJSON with typed accessors
- Dynamic buffer module with grow-on-demand strategy
- Client lifecycle management (`llm_client_create`, `llm_client_destroy`)
- Response lifecycle management (`llm_response_init`, `llm_response_destroy`)
- Bearer token authentication support
- Integration tests for Ollama provider
- Unit tests for buffer, JSON, HTTP, and LLM modules

### Fixed

- Memory leak in `ollama_chat()` using goto cleanup pattern
- HTTP status code validation before response parsing
- URL construction with provider-appended endpoints
- Heap corruption from uninitialized buffer in error paths
