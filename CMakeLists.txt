cmake_minimum_required(VERSION 3.17)
project(IFJproject_CLion C)

set(CMAKE_C_STANDARD 99)

add_executable(IFJproject_CLion test.c test.h scanner.c scanner.h string.c string.h error.h parser.h parser.c)