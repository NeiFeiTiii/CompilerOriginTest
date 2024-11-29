cmake_minimum_required(VERSION 3.27)
project(CompileTestEnd C)

set(CMAKE_C_STANDARD 11)

add_executable(CompileTestEnd main.c
        Lexical.h
        PraseWithRecursive.h
        Lexical2.0.c
        PraseWithRecursive.c)
