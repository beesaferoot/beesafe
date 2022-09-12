#ifndef PARSER_H
#define PARSER_H
#include"Lexer/lexer.h"
#include "Ast/ast.h"
#include "Lexer/Number.h"
#include "Lexer/token.h"
#include <iostream>
#include <cassert>
#include <stack>
#include <unordered_map>
#include <sstream>
#include <vector>

using namespace lexer;
using namespace ast;
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
    ForRangeStmt* parseForIntRangeStatement();
    InitStmt* parseInitStatement();
    DeclareStmt* parseDeclStatement();
    ReturnStmt* parseReturnStatement();
    IfStmt* parseIfStatement();
    FunctionStmt* parseFunctionStatement();
    vector<Identifier*> parseFunctionParameters();
    CallExpression* parseCallExpression();
    ExpressionStmt* parseExpressionStatement();
    Expr* parseExpression();
    Expr* parseFunctionLiteral();
    RangeExpr* parseIntRangeExpression();
    Expr* parseBoolean();
    Expr* parseNull();
    bool peekTokenIs(TokenType);
    bool expectPeek(TokenType);
    bool curTokenIs(TokenType);
    Expr* mkLeaf(Token &);
    Expr* mkNode(Operator&, Expr*, Expr *);
    Operator binary(TokenType);
    Operator uniary(TokenType);
    void expand();
    void produce();
    void pushOperator(Operator&, bool);
    void popOperator(bool);
    vector<std::string> Errors() const;
    bool hasErrors() const;
private:
    Token curToken;
    Token peekToken;
    Lexer* l;
    stack<pair<int, Operator>> Operators;
    stack<Expr*> Operands;
    static  unordered_map<TokenType, PRECENDENCE> precendences;
    static unordered_map<TokenType, bool> BinaryOperators;
    vector<std::string> errors;
};


}

#endif // PARSER_H
