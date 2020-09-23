cmake_minimum_required(VERSION 3.14)
project(jsonfile)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_subdirectory(src)

find_package(GTest)
if (GTEST_FOUND)
    enable_testing()
    add_subdirectory(test)
endif()
