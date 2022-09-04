#ifndef EVALUATOR_H
#define EVALUATOR_H
#include"Parser/parser.h"
#include"Symbols/object.h"
#include"ast/ast.h"
#include<vector>
#include "gc.h"

using namespace symbols;
using namespace ast;
namespace  evaluator {

class Evaluator
{
public:
    Evaluator();
    ~Evaluator();
    static GCPtr<Object> Eval(Node*,  Env *);
    static list<GCPtr<Object>> evalProgram(Program*, Env *);
    static GCPtr<Object> evalBlockStatement(BlockStmt*, Env *);
    static GCPtr<Object> evalIfStatement(IfStmt*, Env*);
    static GCPtr<Object> evalInitStatement(InitStmt*, Env*);
    static GCPtr<Object> evalDeclareStatement(DeclareStmt*, Env*);
    static GCPtr<Object> evalFunctionStmt(FunctionStmt*, Env*);
    static GCPtr<Object> evalIdentifier(Identifier*, Env*);
    static GCPtr<Object> evalStringConcatenate(Token&, GCPtr<Object>, GCPtr<Object>);
    static std::vector<GCPtr<Object>> evalExpressions(std::vector<Expr*>, Env*);
    static GCPtr<Object> applyFunction(GCPtr<Object>, std::vector<GCPtr<Object>>);
    static GCPtr<Object> evalAssignExpression(Assign*, Env*);
    static GCPtr<Object> evalUniaryExpression(Token&, GCPtr<Object>);
    static GCPtr<Object> evalBinaryExpression(Token&, GCPtr<Object>, GCPtr<Object>);
    static GCPtr<Object> evalIntergerBinaryExpression(Token&, GCPtr<Object>, GCPtr<Object>);
    static GCPtr<Object> evalNotOperator(GCPtr<Object>);
    static GCPtr<Object> evalUniaryMinusOperator(GCPtr<Object>);
    static GCPtr<Object> nativeBooleanObject(bool value);
    static bool isTruthy(GCPtr<Object>);
    static bool isError(GCPtr<Object>);
    static GCPtr<Object> newError(std::string, std::string);
    static Env* NewEnvironment();
    static Env* NewEnvironment(Env*);
    static Env* extendFunctionEnv(GCPtr<FunctionObject>, std::vector<GCPtr<Object>>);
    static GCPtr<Object> unWrapReturnvalue(GCPtr<Object>);
};

}


#endif // EVALUATOR_H
