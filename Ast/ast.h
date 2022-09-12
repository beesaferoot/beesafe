#ifndef AST_H
#define AST_H
#include"Lexer/token.h"
#include<vector>
#include<ostream>
#include<sstream>

//using namespace symbols;
using namespace token;
using std::vector;


namespace ast {

enum NodeType {
    UndefinedType,
    BooleanType,
    StmtType,
    BlockStmtType,
    NullType,
    NumberType,
    BinaryOpType,
    UniaryOpType,
    ExpressionStmtType,
    ExpressionType,
    ReturnStmtType,
    StringLiteralType,
    IdentifierType,
    IfStmtType,
    WhileStmtType,
    FunctionStmtType,
    FunctionExprType,
    InitStmtType,
    RangeExprType,
    CallExprType,
    DeclareStmtType,
    ForStmtType,
    AssignStmtType
};

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
    CALL = 5,
    ASSIGN = 1
};

enum ASSOCIATION {
    LEFT = 0,
    RIGHT = 1
};

struct Node {
    int lineno=0;
    NodeType type_value = NodeType::UndefinedType;
    Node(){}
    virtual ~Node(){}
    virtual NodeType type() { return type_value; }
    Node(int line, NodeType type)
        :lineno{line}, type_value{type} {}
    Node(NodeType type)
        : type_value{type} {}
};

struct Expr : Node {
    Expr(int line, NodeType type)
        :Node(line, type){}
    Expr(int line)
        :Node(line, NodeType::ExpressionType){}
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
    Token tok;
    bool value;
    BooleanExpr(Token t, bool v, int line)
        :Expr(line, NodeType::BooleanType), tok{t}, value{v}{}
    ~BooleanExpr(){
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, BooleanExpr const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Stmt : Node {
    Stmt(NodeType type)
        :Node(type){}

    Stmt(int line)
        :Node(line, NodeType::StmtType){}

    Stmt(int line, NodeType type)
        :Node(line, type){}
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
    Token tok;
    vector<Stmt*> Stmts{};
    BlockStmt(Token &tok)
        :Stmt(NodeType::BlockStmtType), tok{tok}{
    }
    ~BlockStmt()
    {
        for(auto stmt : Stmts)
        {
            delete stmt;
            stmt = nullptr;
        }
        Stmts.clear();
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, BlockStmt const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct ReturnStmt : Stmt {
    Token tok;
    Expr* returnValue;
    ReturnStmt(Token t, int line)
        :Stmt(line, NodeType::ReturnStmtType), tok{t}, returnValue{nullptr} {}
    ~ReturnStmt(){
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
    vector<struct Stmt*> Stmts;
    ~Program()
    {
        for(auto stmt: Stmts){
            delete  stmt;
            stmt = nullptr;
        }
        Stmts.clear();
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Program const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct NullExpr : Expr {
    Token tok;
    NullExpr(Token t, int line)
        :Expr(line, NodeType::NullType), tok{t}{}
    ~NullExpr(){
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, NullExpr const *obj)
    {

        out << obj->toString();
        return out;
    }
};

struct Identifier : Expr{
    Token tok;
    Expr* value;
    Identifier(Token t, int line)
        :Expr(line, NodeType::IdentifierType), tok{t}, value{nullptr} {}
    ~Identifier(){
        delete value;
        value = nullptr;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Identifier const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct StringLiteral : Expr{
    Token tok;
    std::string Literal;
    StringLiteral(Token t, int line)
        :Expr(line, NodeType::StringLiteralType), tok{t}, Literal{t.Literal}{}
    ~StringLiteral(){
    }

    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, StringLiteral const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Number :  Expr {
    Token tok;
    int value;
    Number(Token t, int v, int line)
        :Expr(line, NodeType::NumberType), tok{t}, value{v}{}
    ~Number(){
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Number const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Op: Expr {
    Token tok;
    int precedence;
    int associative;
    Op(Token t, int p, int a, int line, NodeType type)
        :Expr(line, type), tok{t}, precedence{p}, associative{a}{}
    virtual ~Op(){
    }
    virtual std::string toString() const = 0;
};


struct BinaryOp : Op {
    Expr* RightOp;
    Expr* LeftOp;
    BinaryOp(Token t, int p, int a, int line)
        :Op(t, p, a, line, NodeType::BinaryOpType), RightOp{nullptr}, LeftOp{nullptr} {}
    BinaryOp(Token t, int p, int a, int line, NodeType node_type)
        :Op(t, p, a, line, node_type), RightOp{nullptr}, LeftOp{nullptr} {}
    virtual ~BinaryOp(){
         delete  RightOp;
        RightOp = nullptr;
         delete LeftOp;
        LeftOp = nullptr;
    }
    virtual std::string toString() const = 0;
};

struct Assign : BinaryOp{
    Assign(Token t, int line)
        :BinaryOp(t, PRECENDENCE::ADD, ASSOCIATION::LEFT,line, NodeType::AssignStmtType){}
    ~Assign(){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, Assign const *obj)
    {
        out << obj->toString();
        return out;
    }
};

struct Add : BinaryOp {
    Add(Token t, int line)
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
    Mult(Token t, int line)
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
    Sub(Token t, int line)
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
    Div(Token t, int line)
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
    LessThan(Token t, int line)
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
    GreaterThan(Token t, int line)
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
    LessThanOrEqual(Token t, int line)
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
    GreaterThanOrEqual(Token t, int line)
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
    Equals(Token t, int line)
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
    NotEquals(Token &t, int line)
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
    UniaryOp(Token &t, int p, int a , int line)
        :Op(t, p, a, line, NodeType::UniaryOpType), RightOp{nullptr} {}
    virtual ~UniaryOp(){
        delete  RightOp;
        RightOp = nullptr;
    }
    virtual std::string toString() const = 0;
};

struct UniarySub: UniaryOp {
    UniarySub(Token &t, int line)
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
    Not(Token &t, int line)
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
    Token &tok;
    struct Identifier* variable;
    Expr* value;
    InitStmt(Token &token, int line)
        :Stmt(line, NodeType::InitStmtType), tok{token}, variable{nullptr}{}
    ~InitStmt(){
        delete  variable;
        variable = nullptr;
        delete  value;
        value = nullptr;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, InitStmt const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct DeclareStmt: Stmt {
    Token tok;
    vector<struct Identifier*> varList;
    DeclareStmt(Token token, int line)
        :Stmt(line, NodeType::DeclareStmtType), tok{token}{}
    ~DeclareStmt(){
        for(auto ident : varList){
            delete ident;
        }
        varList.clear();
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, DeclareStmt const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct WhileStmt: Stmt {
    Token tok;
    BooleanExpr* condition;
    struct BlockStmt* body;
    WhileStmt(Token token, int line)
        :Stmt(line, NodeType::WhileStmtType), tok{token}, condition{nullptr}, body{nullptr} {}
    ~WhileStmt(){
       delete condition;
       condition = nullptr;
       delete body;
       body = nullptr;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, WhileStmt const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct IfStmt: Stmt {
    Token tok;
    Expr* condition;
    struct BlockStmt* body;
    struct BlockStmt* alternative;
    IfStmt(Token token, int line)
        :Stmt(line, NodeType::IfStmtType), tok{token}, condition{nullptr}, body{nullptr}, alternative{nullptr} {}
    ~IfStmt(){
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
    Token tok;
    Expr* Expression;
    ExpressionStmt(Token token, int line)
        :Stmt(line, NodeType::ExpressionStmtType), tok{token}, Expression{nullptr} {}
    ExpressionStmt(Token token, int line, NodeType type)
            :Stmt(line, type), tok{token}, Expression{nullptr} {}
    ~ExpressionStmt(){
        delete Expression;
        Expression = nullptr;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, ExpressionStmt const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct FunctionStmt : ExpressionStmt {
    FunctionStmt(Token token, int line)
        :ExpressionStmt(token, line, NodeType::FunctionStmtType){}
    ~FunctionStmt(){}
};

struct FunctionLiteral: Expr {
    Token tok;
    struct Identifier* header;
    vector<Identifier*> parameters;
    struct BlockStmt* body;
    FunctionLiteral(Token token, int line)
        :Expr(line, NodeType::FunctionExprType), tok{token}, header{nullptr}, body{nullptr} {}
    ~FunctionLiteral(){
        delete  header;
        header = nullptr;
        for(auto param: parameters){
            delete  param;
            param = nullptr;
        }
        parameters.clear();
        delete body;
        body = nullptr;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, FunctionLiteral const * obj)
    {
        out << obj->toString();
        return out;
    }
};

struct CallExpression: Expr {
    Token tok;
    Expr* function;
    vector<Expr*> arguments;
    CallExpression(Token token, int line)
        :Expr(line, NodeType::CallExprType), tok{token}, function{nullptr} {}
    ~CallExpression(){
        delete function;
        function = nullptr;
        for(auto arg: arguments){
           delete  arg;
           arg = nullptr;
        }
        arguments.clear();
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, CallExpression const * obj)
    {
        out << obj->toString();
        return out;
    }
};


struct RangeExpr: Expr {
    Token tok;
    int init_value;
    int end_value;
    RangeExpr(Token t, int line)
        :Expr(line, NodeType::RangeExprType), tok{t} {}
    ~RangeExpr(){
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, RangeExpr const * obj)
    {
        out << obj->toString();
        return out;
    }
};


struct ForStmt: Stmt {
    Token tok;
    Identifier* target;
    RangeExpr* iter;
    BlockStmt* body;

    ForStmt(Token token, int line)
        :Stmt(line, NodeType::ForStmtType), tok{token}{}
    ~ForStmt(){
        delete  target;
        target = nullptr;
        delete iter;
        iter = nullptr;
        delete body;
        body = nullptr;
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
