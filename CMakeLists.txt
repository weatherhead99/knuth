project(knuth)
cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)

find_package(GSL REQUIRED)

add_library(knuth src/gsl_wrap.cpp)
target_include_directories(knuth PRIVATE src)
