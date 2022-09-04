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
    -L"${CMAKE_SOURCE_DIR}/../../cmake-build-debug" -lEvaluator
    -L"${CMAKE_SOURCE_DIR}/../../cmake-build-debug" -lParser
    -L"${CMAKE_SOURCE_DIR}/../../Lexer/cmake-build-debug/" -lLexer
    -L"${CMAKE_SOURCE_DIR}/../../ast/cmake-build-debug/" -last
    -L"${CMAKE_SOURCE_DIR}/../../Symbols/cmake-build-debug/" -lSymbols
    )


