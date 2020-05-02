#ifndef AST_H
#define AST_H
#include"type.h"
#include"token.h"
#include<vector>

using namespace symbols;
using namespace token;
using std::vector;


namespace ast {

enum PRECENDENCE {
    BASE = 0,
    ADD = 1,
    SUB = 1,
    DIV = 3,
    MUL = 3,
    USUB = 4
};

enum ASSOCIATION {
    LEFT = 0,
    RIGHT = 1
};

struct Node{
    int lineno=0;
    Node(){}
    Node(int line)
        :lineno{line} {}
};

struct Expr : Node {
    type_ptr type;
    Expr(int line)
        :Node(line) {}
    Expr(){}
    Expr(type_ptr t, int line)
        :Node(line), type{t}{}
};

struct BooleanExpr : Expr {
    Token* tok;
    bool value;
    BooleanExpr(Token* t, int line)
        :Expr(Type::Bool, line), tok{t}{}
};

struct Stmt : Node {
    Stmt(int line)
        :Node(line){}
    Stmt(){}

};

struct BlockStmt : Stmt {
    Token* tok;
    vector<Stmt*> Stmts{};
    ~BlockStmt()
    {
        delete tok;
        for(auto stmt : Stmts)
        {
            delete stmt;
        }
    }
};

struct Program : Node {
    vector<Stmt*> Stmts;
    ~Program()
    {
        for(auto stmt: Stmts){
            delete  stmt;
        }
    }
};

struct Identifier : Expr{
    Token* tok;
    Expr* value;
    Identifier(Token* t, Expr* v, int line)
        :Expr(v->type, line), tok{t}{}
    Identifier(Token* t, int line)
        :Expr(line), tok{t}{}

};

struct Number :  Expr {
    Token* tok;
    int value;
    Number(Token* t, int v, int line)
        :Expr(Type::Int, line), tok{t}, value{v}{}
};

struct Op: Expr {
    Token* tok;
    int precedence;
    int associative;
    Op(Token* t, int p, int a, int line)
        :Expr(line), tok{t}, precedence{p}, associative{a}{}
};

struct BinaryOp : Op {
    Expr* RightOp;
    Expr* LeftOp;
    BinaryOp(Token* t, int p, int a, int line)
        :Op(t, p, a, line){}
};

struct Add : BinaryOp {
    Add(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::ADD, ASSOCIATION::LEFT,line){}
};

struct Mult : BinaryOp {
    Mult(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::MUL, ASSOCIATION::LEFT, line){}
};

struct Sub : BinaryOp {
    Sub(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::SUB, ASSOCIATION::LEFT, line) {}
};

struct Div : BinaryOp {
    Div(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::DIV, ASSOCIATION::LEFT, line){}
};

struct UniaryOp : Op {
    Expr* RightOp;
    UniaryOp(Token* t, int p, int a , int line)
        :Op(t, p, a, line){}
};

struct UniarySub: UniaryOp {
    UniarySub(Token* t, int line)
        :UniaryOp(t, PRECENDENCE::USUB, ASSOCIATION::LEFT, line){}
};

struct InitStmt: Stmt {
    Token* tok;
    Identifier* variable;
    Expr* value;
    InitStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
};

struct DeclareStmt: Stmt {
    Token* tok;
    vector<Identifier*> varList;
    DeclareStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
};

struct ForStmt: Stmt {
    Token* tok;
    Identifier* target;
    Expr* iter;
    BlockStmt* body;

    ForStmt(Token* token, int line)
        :Stmt(line), tok{token}{}

};

struct WhileStmt: Stmt {
    Token* tok;
    BooleanExpr* condition;
    BlockStmt* body;
    WhileStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
};

struct ExpressionStmt: Stmt {
    Token* tok;
    Expr* Expression;
    ExpressionStmt(Token* token, int line)
        :Stmt(line), tok{token}{}

};

struct RangeExpr: Expr {
    Token* tok;
    int init_point;
    int end_point;
    RangeExpr(Token* t, int line)
        :Expr(line), tok{t} {}
};



}

#endif // AST_H
