cmake_minimum_required(VERSION 3.10)
project(Kaleidoscope CXX)

# Export compile_commands.json for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
add_compile_options(-Wall -Wextra)

# Include directories
include_directories(include)

# Lexer library
add_library(lexer src/lexer.cpp)

# Test executable
add_executable(lexer_test tests/lexer_test.cpp)
target_link_libraries(lexer_test lexer)

# Enable testing
enable_testing()
add_test(NAME LexerTests COMMAND lexer_test)
