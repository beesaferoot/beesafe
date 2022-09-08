#include <gtest/gtest.h>
#include "Parser/parser.h"
#include "Lexer/lexer.h"
#include <string>

using std::string;
using std::cerr;
using std::cout;
using namespace parser;


TEST(TestParser, TestFunctionCall){
    cout << R"(
            ---- test_functioncall ----
            )" << endl;
    string buff = R"(init a = define(x){ x + 2}(2)
                  add(2, 2*4)
                  sqrt(2, 3)
                  ((3*8) - (1 + power(2, 3)))
                  init a = define(){
                    return "flower"
                  }()
                  entry("headers")
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 6);
    cout << program << endl;
}

TEST(TestParser, TestFunctionStmt){
    cout << R"(
            ---- test_functionstmt ----
            )" << endl;
    string buff = R"(define subtext(){
                     return 'hello world'
                  }
                  define entry(){
                      declare text
                      text = 6
                      init subval = 6
                  }
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 2);
    cout << program << endl;
}

TEST(TestParser, TestFunctionLiteral){
    cout << R"(
            ---- test_functionliteral ----

            )" << endl;
    string buff = R"( init fn = define (){}
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 1);
    cout << program << endl;
}

TEST(TestParser, TestReturnStmt){

    cout << R"(
            ---- test_returnstmt ----
            )" << endl;
    string buff = R"(return (1+1)
                     return 1 - 9
                     return 3
                     return 8 * 9
                     return 4 / 2
                     return (1 > 9)
                     return (1 != 0)
                     return (9 == 9)
                     return (20 >= 100)
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 9);
    cout << program << endl;
}


TEST(TestParser, TestIfStmt){
    cout << R"(
            ---- test_ifstmt ----
            )" << endl;
    string buff = R"(if(1 >= 1){

                    }else{

                    }
                    if(0 == 0){

                    }
                    if(0 < 0){

                    }
                    if(19 > 18){

                    }
                    if(17 != 90){

                    }
                   )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 5);
    cout << program << endl;
}

TEST(TestParser, TestForStmt){
    cout << R"(
            ---- test_forstmt ----
            )" << endl;
    string buff = R"(for i in (1..10) {
                   })";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 1);
    cout << program;
}


TEST(TestParser, TestDeclareStmt){
    cout << R"(
            ---- test_declarestmt ----
            )" << endl;
    string buff = R"(
                    declare x
                    declare x, y
                    declare x, y, z
                   )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 3);
    cout << program << endl;
}

TEST(TestParser, TestExpression){
    cout << R"(
            ---- test_expression ----
            )" << endl;
    string buff = R"((7 * (3 + 4 - 4))
                    -2
                    (-3)
                    (6 + (8+9) - 8)
                    3 * 8 + (9 + 0)
                    (9) * (8-9)
                   )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 6);
    cout << program << endl;
}

TEST(TestParser, TestInitStmt){
    cout << R"(
            ---- test_initstmt ----
            )" << endl;
    string buff = R"(
                    init x = 1 + 1
                    init y = 2 - 1
                    init z = 3 + 8
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 3);
    cout << program << endl;
}

TEST(TestParser, TestStmts){
    cout << R"(
            ---- test_stmts ----
            )" << endl;
    string buff = R"(
                   declare x, y
                   init x = 9
                   init z = 1 + (3 + 2)
                   for a in (0..3){
                      init o = 0
                      declare p
                   }
                  )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(!program->Stmts.empty());
    EXPECT_EQ((int)program->Stmts.size(), 4);
    cout << program << endl;
}

TEST(TestParser, TestException){
    cout << R"(
                ----- test_exception ------
            )" << endl;
    string buff = R"((-
                    (]
                    -=
                    =-
                    ++
                    --
                    /)
                    -+
                    )-
                    --
                    !)
                    ()";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    EXPECT_TRUE(program->Stmts.empty());
    cout << program << endl;
}
