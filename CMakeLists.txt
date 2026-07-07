cmake_minimum_required(VERSION 3.14)
project(llm_ai_sdk VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(llm_c
    src/llm.c
    src/http.c
    src/buffer.c
    src/json.c
    src/providers/ollama.c
    src/providers/openrouter.c
    src/stream/stream.c
)

find_package(CURL REQUIRED)
# find_package(cJSON CONFIG REQUIRED)

option(LLM_USE_SYSTEM_CJSON "Use system cJSON" OFF)

if(LLM_USE_SYSTEM_CJSON)
    find_package(cJSON CONFIG REQUIRED)
    target_link_libraries(llm_c PRIVATE cjson)
else()
    add_subdirectory(third_party/cjson)
    target_link_libraries(llm_c PRIVATE cjson)
endif()

target_link_libraries(llm_c
    PRIVATE
        CURL::libcurl
        # cjson
)

target_include_directories(llm_c
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_executable(llm_test
    tests/test_llm.c
)
target_link_libraries(llm_test PRIVATE llm_c)


