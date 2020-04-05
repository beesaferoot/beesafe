cmake_minimum_required(VERSION 3.5)

project(Symbols LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Symbols STATIC
  symbols.cpp
  symbols.h
  type.cpp
  type.h
)

target_compile_definitions(Symbols PRIVATE SYMBOLS_LIBRARY)
include_directories("/home/beesafe/Documents/Source Code/Prj/Qt_Creator/BS/Lexer/Lexer")
