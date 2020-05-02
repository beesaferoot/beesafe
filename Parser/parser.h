#ifndef PARSER_H
#define PARSER_H
#include"lexer.h"
#include"token.h"
#include"ast.h"
#include"Number.h"
#include"range.h"
#include<iostream>
#include<cassert>
#include<stack>
#include<unordered_map>

using namespace lexer;
using namespace ast;
using namespace symbols;
using std::cerr;
using std::cout;
using std::endl;
using std::stack;
using std::pair;
using std::unordered_map;

namespace parser {



class Parser
{
public:
    Parser(Lexer*);
    ~Parser();
    static Parser* New(Lexer*);
    void nextToken();
    Program* parseProgram();
    Stmt* parseStatement();
    BlockStmt* parseBlockStatement();
    ForStmt* parserForStatement();
    InitStmt* parseInitStatement();
    DeclareStmt* parseDeclStatement();
    ExpressionStmt* parseExpressionStatement();
    Expr* parseExpression();
    RangeExpr* parseRangeExpression();
    bool peekTokenIs(TokenType);
    bool expectPeek(TokenType);
    bool curTokenIs(TokenType);
    Expr* mkLeaf(Token *);
    Expr* mkNode(Operator*, Expr*, Expr *);
    Operator* binary(TokenType);
    Operator* uniary(TokenType);
    void E();
    void produce();
    void pushOperator(Operator *, bool);
    void popOperator(bool);
private:
    Token* curToken=nullptr;
    Token* peekToken=nullptr;
    Lexer* l;
    stack<pair<int, Operator*>> Operators;
    stack<Expr*> Operands;
    static  unordered_map<TokenType, PRECENDENCE> precendences;
};

}

#endif // PARSER_H
