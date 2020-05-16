cmake_minimum_required(VERSION 3.5)

project(Symbols LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Symbols STATIC
  object.cpp
  object.h
)

target_compile_definitions(Symbols PRIVATE SYMBOLS_LIBRARY)
include_directories("${CMAKE_SOURCE_DIR}/../../Lexer/Lexer"
                    "${CMAKE_SOURCE_DIR}/../../ast/ast"
                    )
