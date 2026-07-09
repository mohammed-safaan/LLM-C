add_executable(llm_test test_llm.c)
add_test(NAME llm_test COMMAND llm_test)
target_link_libraries(llm_test PRIVATE llm_c)

add_executable(buffer_test test_buffer.c)
add_test(NAME buffer_test COMMAND buffer_test)
target_link_libraries(buffer_test PRIVATE llm_c)