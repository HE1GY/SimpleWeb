cmake_minimum_required(VERSION 3.24)
project(SimpleWeb)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(SimpleWeb)


target_include_directories(SimpleWeb PRIVATE "E:/Program/MySQL/mysql-connector-c++-8.0.32-winx64/include")
target_link_libraries(SimpleWeb PRIVATE "E:/Program/MySQL/mysql-connector-c++-8.0.32-winx64/lib64/vs14/debug/mysqlcppconn.lib")
target_link_libraries(SimpleWeb PRIVATE "E:/Program/MySQL/mysql-connector-c++-8.0.32-winx64/lib64/vs14/debug/mysqlcppconn8.lib")





