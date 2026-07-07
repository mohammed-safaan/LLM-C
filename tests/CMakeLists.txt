add_executable(llm_test test_llm.c)

target_link_libraries(llm_test PRIVATE llm_c)