cmake_minimum_required(VERSION 3.5)

project(ast LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ast STATIC
  ast.cpp
  ast.h
)

target_compile_definitions(ast PRIVATE AST_LIBRARY)
include_directories("/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Symbols/Symbols"
    "/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/Lexer")
