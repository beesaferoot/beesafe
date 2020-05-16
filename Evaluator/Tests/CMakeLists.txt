cmake_minimum_required(VERSION 3.5)

project(TestEvaluator LANGUAGES CXX)


find_package(Qt5Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_executable(TestEvaluator tst_testevaluator.cpp)
add_test(NAME TestEvaluator COMMAND TestEvaluator)

include_directories(
    "${CMAKE_SOURCE_DIR}/../../Lexer/Lexer"
    "${CMAKE_SOURCE_DIR}/../../ast/ast"
    "${CMAKE_SOURCE_DIR}/../../Symbols/Symbols"
    "${CMAKE_SOURCE_DIR}/../../Parser"
    "${CMAKE_SOURCE_DIR}/../"
    )
target_link_libraries(TestEvaluator PRIVATE Qt5::Test
    -L"${CMAKE_SOURCE_DIR}/../../build-Evaluator-Qt-Debug" -lEvaluator
    -L"${CMAKE_SOURCE_DIR}/../../build-Parser-Qt-Debug" -lParser
    -L"${CMAKE_SOURCE_DIR}/../../Lexer/build-Lexer-Qt-Debug/" -lLexer
    -L"${CMAKE_SOURCE_DIR}/../../ast/build-ast-Qt-Debug/" -last
    -L"${CMAKE_SOURCE_DIR}/../../Symbols/build-Symbols-Qt-Debug/" -lSymbols
    )


