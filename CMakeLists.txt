cmake_minimum_required(VERSION 3.15)
project(jsonfile)

set(CMAKE_CXX_STANDARD 17)

# Builds json library
add_subdirectory(src)

install(TARGETS json LIBRARY
        DESTINATION lib
        INCLUDES DESTINATION include)
install(DIRECTORY include DESTINATION ./)

option(BUILD_TESTS OFF)
if(BUILD_TESTS)
    find_package(GTest)
    if (GTEST_FOUND)
        enable_testing()
        add_subdirectory(test)
    else()
        message("GTEST library not found.")
    endif()
endif()
