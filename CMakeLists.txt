cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Example)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
   set(CMAKE_CXX_EXTENSIONS ON)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wconversion -Wnarrowing")
   set(CMAKE_EXE_LINKER_FLAGS "-lm -lquadmath")
elseif()
   set(CMAKE_EXE_LINKER_FLAGS "-lm")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(example.x example.cpp)



