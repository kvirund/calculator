cmake_minimum_required(VERSION 3.0)

project(parsers.set)

option(BUILD_TP_CONSOLE "Build console for to test parsers" ON)
if (CMAKE_HOST_UNIX)
    option(BUILD_ARTIFACTS "Rebuild parsers and generators." ON)
    option(BUILD_UTILITIES "Rebuild re2c and lemon. Used only if BUILD_ARTIFACTS is set" ON)
    option(USE_COLORS "Use colors in terminal" ON)
    if (BUILD_TP_CONSOLE)
        find_library(READLINE readline)
        find_library(HISTORY history)
        find_library(NCURSES ncurses)
    endif ()
    if (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_COMPILER_IS_GNUCC)
	add_definitions(-Wall)
    endif ()
else ()
    option(BUILD_ARTIFACTS "Rebuild parsers and generators.")
    option(BUILD_UTILITIES "Rebuild re2c and lemon. Used only if BUILD_ARTIFACTS is set")
    option(USE_COLORS "Use colors in terminal")
endif ()

add_library(parsers.set STATIC
    colors.cpp
    parser.cpp
    parser_utils.cpp
    state.cpp
    expressions.grammar.cpp
    expressions.scanner.cpp
    where.grammar.cpp
    where.scanner.cpp)

if (BUILD_TP_CONSOLE)
    add_executable(test.it
        main.cpp
        console.grammar.cpp
        console.scanner.cpp
        console.cpp)
    target_link_libraries(test.it parsers.set ACE)
    if (READLINE AND HISTORY AND NCURSES)
        message(STATUS "Checking readline... found")
        target_compile_options(test.it PUBLIC "-DUSE_READLINE")
        target_link_libraries(test.it ${READLINE} ${HISTORY} ${NCURSES})
    else ()
        message(STATUS "Checking readline... not found")
    endif ()

    if (USE_COLORS)
        message(STATUS "Will use colors in terminal window")
        target_compile_options(test.it PUBLIC "-DUSE_COLORS")
    else ()
        message(STATUS "Will not use colors in terminal window")
    endif ()
endif ()

if (BUILD_ARTIFACTS)
    set(LEMON_BINARY ${CMAKE_CURRENT_BINARY_DIR}/lemon/lemon)
    if (BUILD_UTILITIES)
        add_subdirectory(re2c)
        set(RE2C_BINARY ${CMAKE_CURRENT_BINARY_DIR}/re2c/artifacts/bin/re2c)

        # Build lemon
        add_subdirectory(lemon)

        #later dependencies will be added
    else ()
        find_program(RE2C_BINARY re2c)
    endif ()

    # Generate parsers
    ## for expressions
    add_custom_target(expressions.grammar
        DEPENDS expressions.grammar.cpp)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/expressions.grammar.cpp
        COMMAND ${LEMON_BINARY} -q -e.cpp -s ${CMAKE_CURRENT_SOURCE_DIR}/expressions.grammar.y -T${CMAKE_CURRENT_SOURCE_DIR}/lempar.cpp
        DEPENDS expressions.grammar.y lempar.cpp)
    ## for WHERE clauses
    add_custom_target(where.grammar
        DEPENDS where.grammar.cpp)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/where.grammar.cpp
        COMMAND ${LEMON_BINARY} -q -e.cpp -s ${CMAKE_CURRENT_SOURCE_DIR}/where.grammar.y -T${CMAKE_CURRENT_SOURCE_DIR}/lempar.cpp
        DEPENDS where.grammar.y lempar.cpp)
    ## for console grammar
    add_custom_target(console.grammar
        DEPENDS console.grammar.cpp)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/console.grammar.cpp
        COMMAND ${LEMON_BINARY} -q -e.cpp -s ${CMAKE_CURRENT_SOURCE_DIR}/console.grammar.y -T${CMAKE_CURRENT_SOURCE_DIR}/lempar.cpp
        DEPENDS console.grammar.y lempar.cpp)

    if (NOT DEFINED RE2C_FAIL)
        # Generate scanners
        ## for expressions
        add_custom_target(expressions.scanner
            DEPENDS expressions.scanner.cpp)
        add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/expressions.scanner.cpp
            COMMAND ${RE2C_BINARY} -8 -o ${CMAKE_CURRENT_SOURCE_DIR}/expressions.scanner.cpp ${CMAKE_CURRENT_SOURCE_DIR}/expressions.scanner.re
            DEPENDS expressions.scanner.re)
        ## for WHERE clauses
        add_custom_target(where.scanner
            DEPENDS where.scanner.cpp)
        add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/where.scanner.cpp
            COMMAND ${RE2C_BINARY} -8 -o ${CMAKE_CURRENT_SOURCE_DIR}/where.scanner.cpp ${CMAKE_CURRENT_SOURCE_DIR}/where.scanner.re
            DEPENDS where.scanner.re)
        ## for console itself
        add_custom_target(console.scanner
            DEPENDS console.scanner.re)
        add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/console.scanner.cpp
            COMMAND ${RE2C_BINARY} -8 -c -o ${CMAKE_CURRENT_SOURCE_DIR}/console.scanner.cpp ${CMAKE_CURRENT_SOURCE_DIR}/console.scanner.re
            DEPENDS console.scanner.re)

        add_dependencies(parsers.set expressions.scanner)
        add_dependencies(parsers.set where.scanner)
        add_dependencies(test.it console.scanner)

        add_dependencies(expressions.scanner expressions.grammar)
        add_dependencies(where.scanner where.grammar)
        add_dependencies(console.scanner console.grammar)

        if (BUILD_UTILITIES)
            add_dependencies(expressions.scanner re2c)
            add_dependencies(where.scanner re2c)
            add_dependencies(console.scanner re2c)
        endif ()
    endif ()

    add_dependencies(parsers.set expressions.grammar)
    add_dependencies(parsers.set where.grammar)

    add_dependencies(test.it console.grammar)

    add_dependencies(expressions.grammar lemon)
    add_dependencies(where.grammar lemon)
    add_dependencies(console.grammar lemon)
endif ()
# vim: set ts=4 sw=4 tw=0 et syntax=cmake :
