cmake_minimum_required(VERSION 3.13)
project(source)

set(CMAKE_CXX_STANDARD 14)
include_directories('./program1')
include_directories('./program2')
include_directories('./bouns')

add_executable(source bonus/myfork.c)
#add_executable(source program1/program1.c)