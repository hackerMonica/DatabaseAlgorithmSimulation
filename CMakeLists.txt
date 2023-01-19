cmake_minimum_required(VERSION 3.24)
project(untitled C)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_C_STANDARD 11)

add_executable(untitled main.c question1.c  extmem.c question2.c question3.c question5.c basement.c question4.c)
