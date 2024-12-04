cmake_minimum_required(VERSION 3.10)
project(snakeGame)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CXX_WARNINGS -Wno-macro-redefined -Wall -Wextra)

if((DEFINED DEBUG) AND (DEBUG EQUAL 1))
    set(CXX_DEBUG -DDEBUG_FLAG)
endif()

# List all source files
set(SOURCE_DIR src)
set(SOURCE_FILES ${SOURCE_DIR}/game.cpp
    ${SOURCE_DIR}/curses_tui.cpp
    ${SOURCE_DIR}/logger.cpp
    ${SOURCE_DIR}/mechanics.cpp
    ${SOURCE_DIR}/snake.cpp
)

add_executable(snake ${SOURCE_FILES})
target_link_libraries(snake ncurses)
target_compile_options(snake PRIVATE ${CXX_WARNINGS} ${CXX_DEBUG})
