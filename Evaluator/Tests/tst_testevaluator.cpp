#include <QtTest>
#include"parser.h"
#include"lexer.h"
#include"evaluator.h"
#include<iostream>

using std::cout;
using std::string;
using namespace parser;
using namespace evaluator;
// add necessary includes here

class TestEvaluator : public QObject
{
    Q_OBJECT

public:
    TestEvaluator();
    ~TestEvaluator();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_int();
    void test_ifstmt();
    void test_error();
    void test_functioncall();
    void test_recursion();
};

TestEvaluator::TestEvaluator()
{

}

TestEvaluator::~TestEvaluator()
{

}

void TestEvaluator::initTestCase()
{

}

void TestEvaluator::cleanupTestCase()
{

}

void TestEvaluator::test_functioncall(){
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
    QVERIFY(obj.size() > 0);
    QCOMPARE(obj.size(), 2);
}

void TestEvaluator::test_int()
{
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
    QVERIFY(obj.size() > 0);
    QCOMPARE(obj.size(), 4);
}

void TestEvaluator::test_ifstmt()
{
    cout << R"(
            ---- test_ifstmt ----
            )" << endl;
    string buff = R"(if(1 == 1){})";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::evalProgram(program, env);
    QVERIFY(obj.size() > 0);
    QCOMPARE(obj.size(), 1);
}

void TestEvaluator::test_error()
{
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
    QVERIFY(obj.size() > 0);
    QCOMPARE(obj.size(), 1);
}
void TestEvaluator::test_recursion()
{
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
    QVERIFY(obj.size() > 0);
    QCOMPARE(obj.size(), 2);
}
QTEST_APPLESS_MAIN(TestEvaluator);

#include "tst_testevaluator.moc"
