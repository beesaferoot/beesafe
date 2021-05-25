cmake_minimum_required(VERSION 3.5)

project(Repl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Repl main.cpp)

include_directories(
    "${CMAKE_SOURCE_DIR}/../Lexer/Lexer"
    "${CMAKE_SOURCE_DIR}/../Symbols/Symbols"
    "${CMAKE_SOURCE_DIR}/../ast/ast"
    "${CMAKE_SOURCE_DIR}/../Parser"
    "${CMAKE_SOURCE_DIR}/../Evaluator"
    )

target_link_libraries(Repl
     -L"${CMAKE_SOURCE_DIR}/../build-Evaluator-Desktop-Debug" -lEvaluator
    -L"${CMAKE_SOURCE_DIR}/../build-Parser-Desktop-Debug" -lParser
    -L"${CMAKE_SOURCE_DIR}/../Lexer/build-Lexer-Desktop-Debug" -lLexer
    -L"${CMAKE_SOURCE_DIR}/../ast/build-ast-Desktop-Debug" -last
    -L"${CMAKE_SOURCE_DIR}/../Symbols/build-Symbols-Desktop-Debug" -lSymbols

    )

