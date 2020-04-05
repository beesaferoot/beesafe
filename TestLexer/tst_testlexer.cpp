#include <QtTest>
#include"lexer.h"
#include<iostream>
#include<vector>
using namespace lexer;
using namespace token;
using std::cout;
using std::endl;
// add necessary includes here

class TestLexer : public QObject
{
    Q_OBJECT

public:
    TestLexer();
    ~TestLexer();
private slots:
    void testNextToken();
private:
    std::vector<Token> tests;
};

TestLexer::TestLexer()
{
    tests.emplace_back(Word(TokenType::DECLARE, "declare"));
    tests.emplace_back(Word(TokenType::ID, "x"));
    tests.emplace_back(Symbol(TokenType::NEWLINE, "\n"));
    tests.emplace_back(Word(TokenType::DECLARE, "declare"));
    tests.emplace_back(Word(TokenType::ID, "y"));
}

TestLexer::~TestLexer()
{

}
void TestLexer::testNextToken()
{
    Lexer * testLexer = Lexer::New(R"(declare x
                                   declare y)");

    for(auto t : tests){
        auto l = testLexer->NextToken();
        cout << l.Literal << endl;
        QCOMPARE(t.Type, l.Type);
        QCOMPARE(t.Literal, l.Literal);
    }

}

QTEST_APPLESS_MAIN(TestLexer)

#include "tst_testlexer.moc"
