cmake_minimum_required(VERSION 3.24)
project(SimpleWeb)

set(CMAKE_CXX_STANDARD 20)

add_executable(SimpleWeb main.cpp mongoose.c mongoose.h Student.h)

