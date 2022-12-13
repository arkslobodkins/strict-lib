cmake_minimum_required(VERSION 3.15)

project(Example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-lm")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set(CMAKE_CXX_EXTENSIONS ON)
   set(CMAKE_EXE_LINKER_FLAGS "-lquadmath")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wnarrowing")
endif()

add_executable(example.x example.cpp)

