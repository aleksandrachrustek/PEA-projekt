cmake_minimum_required(VERSION 3.22)
project(BranchAndBound)

set(CMAKE_CXX_STANDARD 14)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(BranchAndBound main.cpp matrix.cpp Branch_Bound.cpp Branch_Bound.h)
