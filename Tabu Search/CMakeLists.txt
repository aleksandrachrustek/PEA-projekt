cmake_minimum_required(VERSION 3.22)
project(TabuSearch)

set(CMAKE_CXX_STANDARD 14)

add_executable(TabuSearch main.cpp Tabu.h Tabu.cpp matrix.cpp matrix.h)
