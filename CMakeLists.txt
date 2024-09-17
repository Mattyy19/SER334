cmake_minimum_required(VERSION 3.29)
project(SER334 C)

set(CMAKE_C_STANDARD 11)

add_executable(SER334
    "Module 1/NguyenCollatz.c"
    "Module 1/NguyenComputation.c"
    "Module 1/NguyenDecrypt.c"
        "Module 2/CompletedScheduler.c"
        "Module 3/BMPHandler.h"
        "Module 3/BMPHandler.c"
        "Module 3/Image.c"
        "Module 3/NguyenImageProcessor.c")
