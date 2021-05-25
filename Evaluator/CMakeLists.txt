cmake_minimum_required(VERSION 3.5)

project(Evaluator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Evaluator STATIC
  evaluator.cpp
  evaluator.h
  gc.h
)

target_compile_definitions(Evaluator PRIVATE EVALUATOR_LIBRARY)
include_directories(
    "${CMAKE_SOURCE_DIR}/../Lexer/Lexer"
    "${CMAKE_SOURCE_DIR}/../ast/ast"
    "${CMAKE_SOURCE_DIR}/../Symbols/Symbols"
    "${CMAKE_SOURCE_DIR}/../Parser"
    )
target_link_libraries(Evaluator
    -L"${CMAKE_SOURCE_DIR}/../build-Parser-Qt-Debug" -lParser
    -L"${CMAKE_SOURCE_DIR}/../ast/build-ast-Qt-Debug" -last
    -L"${CMAKE_SOURCE_DIR}/../Symbols/build-Symbols-Qt-Debug" -lSymbols
    )


