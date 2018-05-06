cmake_minimum_required(VERSION 3.7.2)
project(mini-mumble)

enable_testing()

#set the flags for g++
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -O3")


#tells Cmake to use this folder for includes
include_directories(include)

#list all the files that should not be tested (like main)
set (MAIN 
     src/main.cc)

#list all the source files except main
set (SRCS 
     src/options.cc)

#list all the headers files
set (HDRS
     include/sample.h
     include/options.h)

set (TESTS
     tests/main_tests.cc
     )

#create a rule to make
add_executable(mini-mumble ${SRCS} ${MAIN} ${HDRS})


#create a test executable
add_executable(test_exe EXCLUDE_FROM_ALL ${TESTS} ${HDRS} ${SRCS})


add_test(unit_test test_exe)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS test_exe)
