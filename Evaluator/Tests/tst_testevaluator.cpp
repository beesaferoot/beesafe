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
                  if(x > 100){
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
    auto obj = Evaluator::Eval(program, env);
    QVERIFY(obj != nullptr);
    QCOMPARE(Types::BOOLTYPE, obj->type());
}

void TestEvaluator::test_int()
{
    cout << R"(
            ---- test_int ----
            )" << endl;
    string buff = R"(1)";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::Eval(program, env);
    QVERIFY(obj != nullptr);
    QCOMPARE(Types::INTTYPE, obj->type());
}

void TestEvaluator::test_ifstmt()
{
    cout << R"(
            ---- test_ifstmt----
            )" << endl;
    string buff = R"(if(1 == 1){})";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto env = Evaluator::NewEnvironment();
    auto program = parser->parseProgram();
    auto obj = Evaluator::Eval(program, env);
    QVERIFY(obj != nullptr);
    QCOMPARE(Types::NULLTYPE, obj->type());


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
    auto obj = Evaluator::Eval(program, env);
    QVERIFY(obj != nullptr);
    QCOMPARE(Types::ERRORTYPE, obj->type());
}
QTEST_APPLESS_MAIN(TestEvaluator)

#include "tst_testevaluator.moc"
