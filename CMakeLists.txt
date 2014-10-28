cmake_minimum_required(VERSION 2.8)

project(expr)

add_custom_command(OUTPUT scanner.cpp COMMAND re2c -o scanner.cpp scanner.re MAIN_DEPENDENCY scanner.re)
add_executable(expr main.cpp scanner.cpp parser.cpp)
add_definitions(-Wall -g)
