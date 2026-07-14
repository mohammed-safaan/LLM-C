add_executable(llm_test test_llm.c)
add_test(NAME llm_test COMMAND llm_test)
target_link_libraries(llm_test PRIVATE llm_c)

add_executable(buffer_test test_buffer.c)
add_test(NAME buffer_test COMMAND buffer_test)
target_link_libraries(buffer_test PRIVATE llm_c)

add_executable(http_test test_http.c)
add_test(NAME http_test COMMAND http_test)
target_link_libraries(http_test PRIVATE llm_c)

add_executable(json_test test_json.c)
target_link_libraries(json_test PRIVATE llm_c)
add_test(NAME json_test COMMAND json_test)