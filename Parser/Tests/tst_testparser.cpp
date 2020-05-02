#include <QtTest>
#include"parser.h"
#include"lexer.h"
#include<string>

using std::string;
using std::cerr;
using std::cout;
using namespace parser;
// add necessary includes here

class TestParser : public QObject
{
    Q_OBJECT

public:
    TestParser();
    ~TestParser();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_forstmt();
    void test_declarestmt();
    void test_initstmt();
    void test_expression();
    void test_stmts();
};

TestParser::TestParser()
{

}

TestParser::~TestParser()
{

}

void TestParser::initTestCase()
{

}

void TestParser::cleanupTestCase()
{

}

void TestParser::test_forstmt()
{
    cout << R"(
            ---- test_forstmt ----
            )" << endl;
    string buff = R"(for i in (1..10) {
                     for j in (10..20){}
                     for j in (10..20){}
                     for j in (10..20){}
                   })";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    cout << "LEN(STMTS) -> " << program->Stmts.size()
         << endl;
    QCOMPARE(program->Stmts.size(), 1);
    ForStmt* forstmt = (ForStmt*)program->Stmts[0];
    BlockStmt* stmtBody = nullptr;

    stmtBody = forstmt->body;
    QCOMPARE(stmtBody->Stmts.size(), 3);

    for(Stmt* stmt : stmtBody->Stmts)
    {
        cout << "statement at line numnber " << stmt->lineno << endl;
    }



}

void TestParser::test_declarestmt()
{

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
    cout << "LEN(STMTS) -> " << program->Stmts.size()
         << endl;
    QCOMPARE(program->Stmts.size(), 3);

    for(int i{0}; i < (int) program->Stmts.size(); i++)
    {
        auto temp = (DeclareStmt*) program->Stmts[i];
        cout << "statement " << (i+1) << endl;
        cout << "declare ";
        for(auto id : temp->varList){
            QCOMPARE(id->tok->Type, TokenType::ID);
            cout << id->tok->Literal << ", ";
        }
        cout << endl;
    }
}

void TestParser::test_expression()
{
    cout << R"(
            ---- test_expression ----
            )" << endl;
    string buff = R"(
                    (6 + (8+9) - 8)
                    3 * 8 + (9 + 0)
                    (9) * (8-9)
                   )";
    Lexer* l = Lexer::New(buff);
    Parser* parser = Parser::New(l);
    auto program = parser->parseProgram();
    cout << "LEN(STMTS) -> " << program->Stmts.size()
         << endl;
    QCOMPARE(program->Stmts.size(), 3);
}

void TestParser::test_initstmt()
{
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
    cout << "LEN(STMTS) -> " << program->Stmts.size()
         << endl;
    QCOMPARE(program->Stmts.size(), 3);
    for(int i{0}; i < (int) program->Stmts.size(); i++)
    {
        auto temp = (InitStmt*) program->Stmts[i];
        cout << "statement " << (i+1) << endl;
        cout << "init " << temp->variable->tok->Literal;
        auto val = (Op*)temp->value;
        cout << " = " << val->tok->Literal;
        cout << endl;
    }
}

void TestParser::test_stmts()
{
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
    cout << "LEN(STMTS) -> " << program->Stmts.size()
         << endl;
    QCOMPARE(program->Stmts.size(), 4);
}

QTEST_APPLESS_MAIN(TestParser)

#include "tst_testparser.moc"
