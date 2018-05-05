cmake_minimum_required(VERSION 3.7.2)
project(mini-mumble)

#set the flags for g++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -g3")


#tells Cmake to use this folder for includes
include_directories(include)

#list all the source files
set (SRCS
     src/main.cc)

#list all the headers files
set (HDRS
     include/sample.h)

set (TESTS
     tests/main_tests.cc)

#create a rule to make
add_executable(mini-mumble ${SRCS} ${HDRS})

#create a test executable
add_executable(test_exe ${TESTS} ${HDRS})
