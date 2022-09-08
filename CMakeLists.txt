cmake_minimum_required(VERSION 3.5)

project(BS LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(BS

        Interpreter/interpreter.cpp
        Ast/ast.cpp
        Evaluator/evaluator.cpp
        Lexer/lexer.cpp
        Parser/parser.cpp
        Symbols/symbols.cpp
        Symbols/object.cpp
        Repl/main.cpp

        Interpreter/interpreter.h
        Ast/ast.h
        Evaluator/evaluator.h
        Evaluator/gc.h
        Lexer/lexer.h
        Lexer/Number.h
        Lexer/Symbol.h
        Lexer/Word.h
        Lexer/token.h
        Lexer/Operator.h
        Parser/parser.h
        Symbols/object.h
        Symbols/symbols.h)

add_executable(tests_out
        Tests/test_evaluator.cpp
        Tests/test_lexer.cpp
        Tests/test_parser.cpp
)
target_link_libraries(
        tests_out
        GTest::gtest_main
)

include(FetchContent)

# setup gtest
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)
gtest_discover_tests(BS)