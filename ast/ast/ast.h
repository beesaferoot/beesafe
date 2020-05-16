#ifndef AST_H
#define AST_H
#include"token.h"
#include<vector>
#include<ostream>
#include<sstream>

//using namespace symbols;
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
    Expr(int line)
        :Node(line){}
    Expr(){}
    virtual ~Expr(){

    }

    virtual std::string toString() const = 0;
    friend std::ostream& operator<<(std::ostream &out, Expr const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct BooleanExpr : Expr {
    Token* tok;
    bool value;
    BooleanExpr(Token* t, bool v, int line)
        :Expr(line), tok{t}, value{v}{}
    ~BooleanExpr(){
        delete  tok;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, BooleanExpr const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Stmt : Node {
    Stmt(int line)
        :Node(line){}
    Stmt(){}
    virtual ~Stmt(){}
    virtual std::string toString() const = 0;
    friend std::ostream& operator<<(std::ostream &out, Stmt const *obj)
    {
        out << obj->toString();
        return out;
    }
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, BlockStmt const *obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;

    friend std::ostream& operator<<(std::ostream &out, ReturnStmt const *obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Program const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Identifier : Expr{
    Token* tok;
    Expr* value;
    Identifier(Token* t, int line)
        :Expr(line), tok{t}{}
    ~Identifier(){
        delete  tok;
        delete value;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Identifier const *obj)
    {

        out << obj->toString();
        return out;
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

    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, StringLiteral const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Number :  Expr {
    Token* tok;
    int value;
    Number(Token* t, int v, int line)
        :Expr(line), tok{t}, value{v}{}
    ~Number(){
        delete  tok;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Number const *obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const = 0;
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
    virtual std::string toString() const = 0;
};

struct Assign : BinaryOp{
    Assign(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::ADD, ASSOCIATION::LEFT,line){}
    ~Assign(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Assign const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Add : BinaryOp {
    Add(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::ADD, ASSOCIATION::LEFT,line){}
    ~Add(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Add const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Mult : BinaryOp {
    Mult(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::MUL, ASSOCIATION::LEFT, line){}
    ~Mult(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Mult const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Sub : BinaryOp {
    Sub(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::SUB, ASSOCIATION::LEFT, line) {}
    ~Sub(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Sub const *obj)
    {
        out << obj->toString();
        return out;
    }
};



struct Div : BinaryOp {
    Div(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::DIV, ASSOCIATION::LEFT, line){}
    ~Div(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Div const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct LessThan : BinaryOp {
    LessThan(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::LT, ASSOCIATION::LEFT, line){}
    ~LessThan(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, LessThan const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct GreaterThan : BinaryOp {
    GreaterThan(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::GT, ASSOCIATION::LEFT, line){}
    ~GreaterThan(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, GreaterThan const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct LessThanOrEqual : BinaryOp {
    LessThanOrEqual(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::LT_EQ, ASSOCIATION::LEFT, line){}
    ~LessThanOrEqual(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, LessThanOrEqual const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct GreaterThanOrEqual : BinaryOp {
    GreaterThanOrEqual(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::GT_EQ, ASSOCIATION::LEFT, line){}
    ~GreaterThanOrEqual(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, GreaterThanOrEqual const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Equals : BinaryOp {
    Equals(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::EQ, ASSOCIATION::RIGHT, line){}
    ~Equals(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Equals const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct NotEquals : BinaryOp{
    NotEquals(Token* t, int line)
        :BinaryOp(t, PRECENDENCE::NT_EQ, ASSOCIATION::LEFT, line){}
    ~NotEquals(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, NotEquals const * obj)
    {
        out << obj->toString();
        return out;
    }
};


struct UniaryOp : Op {
    Expr* RightOp;
    UniaryOp(Token* t, int p, int a , int line)
        :Op(t, p, a, line){}
    virtual ~UniaryOp(){
        delete  RightOp;
    }
    virtual std::string toString() const = 0;
};

struct UniarySub: UniaryOp {
    UniarySub(Token* t, int line)
        :UniaryOp(t, PRECENDENCE::USUB, ASSOCIATION::LEFT, line){}
    ~UniarySub(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, UniarySub const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Not : UniaryOp{
    Not(Token* t, int line)
        :UniaryOp(t, PRECENDENCE::NOT, ASSOCIATION::RIGHT, line){}
    ~Not(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Not const * obj)
    {
        out << obj->toString();
        return out;
    }
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, InitStmt const * obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, DeclareStmt const * obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, WhileStmt const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct IfStmt: Stmt {
    Token* tok;
    Expr* condition;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, IfStmt const * obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, ExpressionStmt const * obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, FunctionLiteral const * obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, CallExpression const * obj)
    {
        out << obj->toString();
        return out;
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
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, RangeExpr const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct ForStmt: Stmt {
    Token* tok;
    Identifier* target;
    RangeExpr* iter;
    BlockStmt* body;

    ForStmt(Token* token, int line)
        :Stmt(line), tok{token}{}
    ~ForStmt(){
        delete tok;
        delete  target;
        delete iter;
        delete body;
    }
    virtual std::string toString() const;

    friend std::ostream& operator<<(std::ostream &out, ForStmt const * obj)
    {
        out << obj->toString();
        return out;
    }
};


}

#endif // AST_H
