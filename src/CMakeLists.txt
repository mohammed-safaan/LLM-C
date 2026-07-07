add_library(llm_c
    llm.c
    http.c
    buffer.c
    json.c
    providers/ollama.c
    providers/openrouter.c
    stream/stream.c
)

target_include_directories(llm_c
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(llm_c
    PRIVATE
        CURL::libcurl
        cjson
)