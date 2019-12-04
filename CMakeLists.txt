cmake_minimum_required(VERSION 3.14)
project(c_ex3 C)

set(CMAKE_C_STANDARD 99)

add_executable(c_ex3 RBTree.c Structs.c RBTree.h Structs.h ProductExample.c)