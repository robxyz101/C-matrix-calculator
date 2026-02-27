cmake_minimum_required(VERSION 4.0)
project(The_Matrix C)

set(CMAKE_C_STANDARD 11)

add_executable(The_Matrix
        "The Matrix.c"
        matrix.h
        matrix.c)
