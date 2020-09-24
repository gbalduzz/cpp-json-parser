add_library(json STATIC json_writer.cpp json_reader.cpp details/json_group.cpp
                        details/json_entry.cpp details/util.cpp)

target_include_directories(json PUBLIC ${PROJECT_SOURCE_DIR}/include)
