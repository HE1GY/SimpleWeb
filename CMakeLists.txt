cmake_minimum_required(VERSION 3.18)
project(SimpleWeb)

set(CMAKE_CXX_STANDARD 17)


add_subdirectory(external/connector)

add_executable(SimpleWeb main.cpp mongoose.c mongoose.h Student.h)


set_target_properties( connector PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set_target_properties( SimpleWeb PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

target_link_libraries(SimpleWeb PRIVATE connector)
target_include_directories(SimpleWeb PRIVATE "D:/dev/MySQLTest/external/connector/include")








