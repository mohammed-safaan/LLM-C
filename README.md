# LLM-C

A lightweight C SDK for integrating Large Language Models into native applications.

## Overview

LLM-C wraps HTTP communication, JSON parsing, and provider-specific LLM protocols into a single portable C99 header. Target use cases include embedded systems, game engines, high-performance computing, and native desktop applications.

**Status:** Early development (v0.1.0). Source files are scaffolded but not yet implemented.

## Goals

- Clean, minimal C99 API with zero bloat
- Pluggable provider architecture via function pointer vtable
- Cross-platform: Windows (WinHTTP) and POSIX (libcurl)
- Streaming support with background threading
- No runtime dependencies beyond libcurl and cJSON

## Providers

| Provider       | Endpoint       | Default Model   | Auth         |
| -------------- | -------------- | --------------- | ------------ |
| **Ollama**     | Local REST API | `llama3`        | None         |
| **OpenRouter** | REST API       | `openai/gpt-4o` | Bearer token |

## Dependencies

| Dependency                                   | Purpose      | Required                   |
| -------------------------------------------- | ------------ | -------------------------- |
| [libcurl](https://curl.se/libcurl/)          | HTTP client  | Yes                        |
| [cJSON](https://github.com/DaveGamble/cJSON) | JSON parsing | Yes (bundled as submodule) |
| [CMake](https://cmake.org/) >= 3.14          | Build system | Yes                        |

## Building

### Prerequisites

- CMake 3.14+ (presets require 3.21+)
- C compiler (MSYS2 Clang recommended on Windows, GCC/Clang on POSIX)
- libcurl development headers

### Build

```bash
git clone --recurse-submodules <repo-url>
cd llm_ai_sdk
cmake --preset debug
cmake --build --preset debug
```

### Running Tests

```bash
ctest --test-dir build --verbose# LLM-C

A lightweight C SDK for integrating Large Language Models into native applications.

## Overview

LLM-C wraps HTTP communication, JSON parsing, and provider-specific LLM protocols into a single portable C99 header. Target use cases include embedded systems, game engines, high-performance computing, and native desktop applications.

**Status:** Early development (v0.1.0). Source files are scaffolded but not yet implemented.

## Goals

- Clean, minimal C99 API with zero bloat
- Pluggable provider architecture via function pointer vtable
- Cross-platform: Windows (WinHTTP) and POSIX (libcurl)
- Streaming support with background threading
- No runtime dependencies beyond libcurl and cJSON

## Providers

| Provider       | Endpoint       | Default Model   | Auth         |
| -------------- | -------------- | --------------- | ------------ |
| **Ollama**     | Local REST API | `llama3`        | None         |
| **OpenRouter** | REST API       | `openai/gpt-4o` | Bearer token |

## Dependencies

| Dependency                                   | Purpose      | Required                   |
| -------------------------------------------- | ------------ | -------------------------- |
| [libcurl](https://curl.se/libcurl/)          | HTTP client  | Yes                        |
| [cJSON](https://github.com/DaveGamble/cJSON) | JSON parsing | Yes (bundled as submodule) |
| [CMake](https://cmake.org/) >= 3.14          | Build system | Yes                        |

## Building

### Prerequisites

- CMake 3.14+ (presets require 3.21+)
- C compiler (MSYS2 Clang recommended on Windows, GCC/Clang on POSIX)
- libcurl development headers

### Build

```bash
git clone --recurse-submodules <repo-url>
cd llm_ai_sdk
cmake --preset debug
cmake --build --preset debug
```

### Running Tests

```bash
ctest --test-dir build --verbose
```

### Build Options

| Option                 | Default | Description                                   |
| ---------------------- | ------- | --------------------------------------------- |
| `LLM_USE_SYSTEM_CJSON` | `OFF`   | Use system cJSON instead of bundled submodule |

## Project Structure

```
llm_ai_sdk/
├── include/llm/       # Public API headers
├── src/               # Implementation
│   ├── providers/     # Provider adapters (Ollama, OpenRouter)
│   └── stream/        # Streaming support
├── tests/             # Unit tests
├── examples/          # Demo applications
├── third_party/cjson/ # Bundled cJSON
└── docs/              # Documentation
```

## Roadmap

1. HTTP layer (cross-platform)
2. JSON parser (zero-dependency recursive descent)
3. LLM core abstraction
4. Ollama provider
5. OpenRouter provider
6. Streaming support
7. Tests and examples
8. Tokenizer, embeddings, RAG (future)

## License

TBD

```

### Build Options

| Option                 | Default | Description                                   |
| ---------------------- | ------- | --------------------------------------------- |
| `LLM_USE_SYSTEM_CJSON` | `OFF`   | Use system cJSON instead of bundled submodule |

## Project Structure

```
llm_ai_sdk/
├── include/llm/       # Public API headers
├── src/               # Implementation
│   ├── providers/     # Provider adapters (Ollama, OpenRouter)
│   └── stream/        # Streaming support
├── tests/             # Unit tests
├── examples/          # Demo applications
├── third_party/cjson/ # Bundled cJSON
└── docs/              # Documentation
```

## Roadmap

1. HTTP layer (cross-platform)
2. JSON parser (zero-dependency recursive descent)
3. LLM core abstraction
4. Ollama provider
5. OpenRouter provider
6. Streaming support
7. Tests and examples
8. Tokenizer, embeddings, RAG (future)

## License

TBD
