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
    LT = 1,
    GT = 1,
    LT_EQ = 1,
    GT_EQ = 1,
    EQ = 1,
    NT_EQ = 1,
    ADD = 2,
    SUB = 2,
    DIV = 3,
    MUL = 3,
    USUB = 4,
    NOT = 4,
    CALL = 5
};

enum ASSOCIATION {
    LEFT = 0,
    RIGHT = 1
};

struct Node{
    int lineno=0;
    Node(){}
    virtual ~Node(){}
    Node(int line)
        :lineno{line} {}
};

struct Expr : Node {
    type_ptr type;
    Expr(int line)
        :Node(line), type(nullptr) {}
    Expr():type(nullptr){}
    virtual ~Expr(){
        delete  type;
    }
    Expr(type_ptr t, int line)
        :Node(line), type{t}{}
};

struct BooleanExpr : Expr {
    Token* tok;
    bool value;
    BooleanExpr(Token* t, int line)
        :Expr(Type::Bool, line), tok{t}{}
    ~BooleanExpr(){
        delete  tok;
    }
};

struct Stmt : Node {
    Stmt(int line)
        :Node(line){}
    Stmt(){}
    virtual ~Stmt(){}

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

struct ReturnStmt : Stmt {
    Token* tok;
    Expr* returnValue;
    ReturnStmt(Token* t, int line)
        :Stmt(line), tok{t}{}
    ~ReturnStmt(){
        delete  tok;
        delete  returnValue;
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
    ~Identifier(){
        delete  tok;
        delete value;
    }

};

struct StringLiteral : Expr{
    Token* tok;
    std::string Literal;
    StringLiteral(Token* t, int line)
        :Expr(line), Literal{t->Literal}{}
    ~StringLiteral(){
        delete  tok;
    }
};

struct Number :  Expr {
    Token* tok;
    int value;
    Number(Token* t, int v, int line)
        :Expr(Type::Int, line), tok{t}, value{v}{}
    ~Number(){
        delete  tok;
    }
};

struct Op: Expr {
    Token* tok;
    int precedence;
    int associative;
    Op(Token* t, int p, int a, int line)
        :Expr(line), tok{t}, precedence{p}, associative{a}{}
    virtual ~Op(){
        delete  tok;
    }
};


struct BinaryOp : Op {
    Expr* RightOp;
    Expr* LeftOp;
    BinaryOp(Token* t, int p, int a, int line)
        :Op(t, p, a, line){}
    virtual ~BinaryOp(){
        delete  RightOp;
        delete LeftOp;
    }
};

struct Assign : BinaryOp{
    Assign(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::ADD, ASSOCIATION::LEFT,line){}
    ~Assign(){}
};

struct Add : BinaryOp {
    Add(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::ADD, ASSOCIATION::LEFT,line){}
    ~Add(){}
};

struct Mult : BinaryOp {
    Mult(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::MUL, ASSOCIATION::LEFT, line){}
    ~Mult(){}
};

struct Sub : BinaryOp {
    Sub(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::SUB, ASSOCIATION::LEFT, line) {}
    ~Sub(){}
};

struct Div : BinaryOp {
    Div(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::DIV, ASSOCIATION::LEFT, line){}
    ~Div(){}
};

struct LessThan : BinaryOp {
    LessThan(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::LT, ASSOCIATION::LEFT, line){}
    ~LessThan(){}
};

struct GreaterThan : BinaryOp {
    GreaterThan(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::GT, ASSOCIATION::LEFT, line){}
    ~GreaterThan(){}
};

struct LessThanOrEqual : BinaryOp {
    LessThanOrEqual(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::LT_EQ, ASSOCIATION::LEFT, line){}
    ~LessThanOrEqual(){}
};

struct GreaterThanOrEqual : BinaryOp {
    GreaterThanOrEqual(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::GT_EQ, ASSOCIATION::LEFT, line){}
    ~GreaterThanOrEqual(){}
};

struct Equals : BinaryOp {
    Equals(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::EQ, ASSOCIATION::RIGHT, line){}
    ~Equals(){}
};

struct NotEquals : BinaryOp{
    NotEquals(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::NT_EQ, ASSOCIATION::LEFT, line){}
    ~NotEquals(){}
};


struct UniaryOp : Op {
    Expr* RightOp;
    UniaryOp(Token* t, int p, int a , int line)
        :Op(t, p, a, line){}
    virtual ~UniaryOp(){
        delete  RightOp;
    }
};

struct UniarySub: UniaryOp {
    UniarySub(Token* t, int line)
        :UniaryOp(t, PRECENDENCE::USUB, ASSOCIATION::LEFT, line){}
    ~UniarySub(){}
};

struct Not : UniaryOp{
    Not(Token* t, int line)
        :UniaryOp(t, PRECENDENCE::NOT, ASSOCIATION::RIGHT, line){}
    ~Not(){}
};

struct InitStmt: Stmt {
    Token* tok;
    Identifier* variable;
    Expr* value;
    InitStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
    ~InitStmt(){
        delete tok;
        delete  variable;
        delete  value;
    }
};

struct DeclareStmt: Stmt {
    Token* tok;
    vector<Identifier*> varList;
    DeclareStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
    ~DeclareStmt(){
        delete tok;
        for(auto ident : varList){
            delete ident;
        }
    }
};

struct ForStmt: Stmt {
    Token* tok;
    Identifier* target;
    Expr* iter;
    BlockStmt* body;

    ForStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
    ~ForStmt(){
        delete tok;
        delete  target;
        delete iter;
        delete body;
    }
};


struct WhileStmt: Stmt {
    Token* tok;
    BooleanExpr* condition;
    BlockStmt* body;
    WhileStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
    ~WhileStmt(){
        delete tok;
        delete condition;
        delete body;
    }
};

struct IfStmt: Stmt {
    Token* tok;
    BooleanExpr* condition;
    BlockStmt* body;
    BlockStmt* alternative;
    IfStmt(Token* token, int line)
        :Stmt(line), tok{token}, alternative{nullptr} {}
    ~IfStmt(){
        delete tok;
        delete condition;
        delete body;
        delete alternative;
    }
};

struct ExpressionStmt: Stmt {
    Token* tok;
    Expr* Expression;
    ExpressionStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
    ~ExpressionStmt(){
        delete  tok;
        delete Expression;
    }

};

struct FunctionStmt : ExpressionStmt {
    FunctionStmt(Token* token, int line)
        :ExpressionStmt(token, line){}
    ~FunctionStmt(){}
};

struct FunctionLiteral: Expr {
    Token* tok;
    Identifier* header;
    vector<Identifier*> parameters;
    BlockStmt* body;
    FunctionLiteral(Token* token, int line)
        :Expr(line), tok{token}{}
    ~FunctionLiteral(){
        delete  tok;
        delete  header;
        for(auto param: parameters){
            delete  param;
        }
        delete body;
    }
};

struct CallExpression: Expr {
    Token* tok;
    Expr* function;
    vector<Expr*> arguments;
    CallExpression(Token* token, int line)
        :Expr(line), tok{token}{}
    ~CallExpression(){
        delete  tok;
        delete function;
        for(auto arg: arguments){
            delete  arg;
        }
    }
};

struct RangeExpr: Expr {
    Token* tok;
    int init_point;
    int end_point;
    RangeExpr(Token* t, int line)
        :Expr(line), tok{t} {}
    ~RangeExpr(){
        delete tok;
    }
};



}

#endif // AST_H
