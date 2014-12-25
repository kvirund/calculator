cmake_minimum_required(VERSION 2.8)

project(expr)

add_definitions(-Wall -g)
add_definitions(-DUSE_COLORS)
add_executable(expr main.cpp scanner.cpp parser.cpp grammar.c state.cpp)

find_library(READLINE readline)
if (${READLINE} STREQUAL "READLINE-NOTFOUND")
    message(STATUS "Readline not found")
else (${READLINE} STREQUAL "READLINE-NOTFOUND")
    message(STATUS "Readline found")
    add_definitions(-DUSE_READLINE)
    target_link_libraries(expr ${READLINE})
endif (${READLINE} STREQUAL "READLINE-NOTFOUND")

add_custom_command(OUTPUT ${CMAKE_HOME_DIRECTORY}/scanner.cpp
    COMMAND re2c -o scanner.cpp scanner.re
    MAIN_DEPENDENCY scanner.re
    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
add_custom_command(OUTPUT ${CMAKE_HOME_DIRECTORY}/grammar.c
    COMMAND lemon grammar.y
    MAIN_DEPENDENCY grammar.y
    WORKING_DIRECTORY ${CMAKE_HOME_DIRECTORY})
