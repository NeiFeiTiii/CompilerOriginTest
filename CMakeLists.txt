cmake_minimum_required(VERSION 3.10)
project(ComplieTest2)

set(CMAKE_C_STANDARD 11)

add_executable(ComplieTest2 main.c Recursive.c Lexical.c
        SLR1.h
        SLR1.c
        GramAnalysis.h
        GramAnalysis.c
        ToPoland.c
        ToPoland.h
        PraseWithRecursive.c
        PraseWithRecursive.h)