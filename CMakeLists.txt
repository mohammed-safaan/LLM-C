cmake_minimum_required(VERSION 3.14)

project(llm_ai_sdk VERSION 0.1.0 LANGUAGES C)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(ENABLE_CJSON_TEST OFF)

include(CTest)

option(LLM_USE_SYSTEM_CJSON "Use system cJSON" OFF)

find_package(CURL REQUIRED)

if(LLM_USE_SYSTEM_CJSON)
    find_package(cJSON CONFIG REQUIRED)
else()
    add_subdirectory(third_party/cjson)
endif()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(examples)