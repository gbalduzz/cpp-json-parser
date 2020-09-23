function (json_add_test name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} gtest gtest_main pthread json)
    target_compile_definitions(${name} PRIVATE TEST_DIR=\"${PROJECT_SOURCE_DIR}/test/\")
    add_test(NAME ${name} COMMAND ${name})
endfunction()

json_add_test(read_test)
json_add_test(write_test)
