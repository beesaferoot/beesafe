#include <gtest/gtest.h>
#include "Parser/parser.h"
#include "Lexer/lexer.h"
#include "Evaluator/evaluator.h"
#include <iostream>

using std::cout;
using std::string;
using namespace parser;
using namespace evaluator;

TEST(TestEvaluator, TestInt) {
    cout << R"(
            ---- test_int ----
            )" << endl;
    string buff = R"(1+1
                    1-1-1
                    2*1-1+1
                    5/5*5-7
                   )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(!obj.empty());
    EXPECT_EQ(obj.size(), 4);
}

TEST(TestEvaluator, TestFunctionCall){
    cout << R"(
            ---- test_functioncall ----
            )" << endl;
    string buff = R"(init counter = define(x){
                  if(x > 0){
                     return true
                  }else{
                    init foobar = 9999
                    counter(x+1)
                   }
                 }
                 counter(0)
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(!obj.empty());
    EXPECT_EQ(obj.size(), 2);
}


TEST(TestEvaluator, TestIfStmt){
    cout << R"(
            ---- test_ifstmt ----
            )" << endl;
    string buff = R"(if(1 == 1){})";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(!obj.empty());
    EXPECT_EQ(obj.size(), 1);
}

TEST(TestEvaluator, TestError){
    cout << R"(
            ---- test_error----
            )" << endl;
    string buff = R"(
                  if(10>1){
                    if(10>1){
                      true + false
                   }
                   return 1
                  })";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(!obj.empty());
    EXPECT_EQ(obj.size(), 1);
}


TEST(TestEvaluator, TestRecursion){
    cout << R"(
                ----- test_recursion -----
               )" << endl;
    string buff = R"(
                    define fib(a){
                        if(a == 0){
                         return 1
                        }
                        if( a == 1){
                           return 1
                        }
                        return fib(a-1) + fib(a-2)
                     }
                     init recurse = fib(3)
                    )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(!obj.empty());
    EXPECT_EQ(obj.size(), 2);
}


TEST(TestEvaluator, TestReturnStmt){
    cout << R"(
                ----- test_returnstmt -----
               )" << endl;
    string buff = R"(
                    return 22
                    init a = 90
                    return a
                    return invalid_variable
                    )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(!obj.empty());
    EXPECT_EQ(obj.size(), 4);
}

TEST(TestEvaluator, TestForRange){
    cout << R"(
                ----- test_for_range -----
               )" << endl;
    string buff = R"(
                    init sum = 0
                    for i in (1..10){
                      sum = sum + 1
                    }
                    return sum
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(obj.size() == 3);
    EXPECT_EQ(obj.back()->type(), Types::INTTYPE);
}

TEST(TestEvaluator, TestBuiltinLenFunc){
    cout << R"(
            ----- test_builtin_function -----
           )" << endl;
    string buff = R"(len
               len("h")
               len("str")
               len(1)
               len()
              )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    EXPECT_TRUE(obj.size() == 5);
    EXPECT_EQ(obj.back()->type(), Types::ERRORTYPE);
}