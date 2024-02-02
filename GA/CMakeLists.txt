cmake_minimum_required(VERSION 3.25)
project(Genetyczny___Nowy)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Genetyczny___Nowy
        Genetic.cpp
        Genetic.h
        Menu.cpp
        Menu.h
        PEA-Projekt.cpp
        stdafx.cpp
        stdafx.h
        targetver.h)
