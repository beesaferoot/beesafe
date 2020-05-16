#ifndef EVALUATOR_H
#define EVALUATOR_H
#include"parser.h"
#include"object.h"
#include"ast.h"
#include<vector>

using namespace symbols;
namespace  evaluator {

class Evaluator
{
public:
    Evaluator();
    static Object* Eval(Node*, Env*);
    static Object* evalProgram(Program*, Env*);
    static Object* evalBlockStatement(BlockStmt*, Env*);
    static Object* evalIfStatement(IfStmt*, Env*);
    static Object* evalInitStatement(InitStmt*, Env*);
    static Object* evalDecalareStatement(DeclareStmt*, Env*);
    static Object* evalFunctionStmt(FunctionStmt*, Env*);
    static Object* evalIdentifier(Identifier*, Env*);
    static Object* evalStringConcatenate(Token*, Object*, Object*);
    static std::vector<Object*> evalExpressions(std::vector<Expr*>, Env*);
    static Object* applyFunction(Object*, std::vector<Object*>);
    static Object* evalAssignExpression(Assign*, Env*);
    static Object* evalUniaryExpression(Token*, Object*);
    static Object* evalBinaryExpression(Token*, Object*, Object*);
    static Object* evalIntergerBinaryExpression(Token*, Object*, Object*);
    static Object* evalNotOperator(Object*);
    static Object* evalUniaryMinusOperator(Object*);
    static BoolObject* nativeBooleanObject(bool value);
    static bool isTruthy(Object*);
    static bool isError(Object*);
    static ErrorObject* newError(std::string, std::string);
    static Env* NewEnvironment();
    static Env* NewEnvironment(Env*);
    static Env* extendFunctionEnv(FunctionObject*, std::vector<Object*>);
    static Object* unWrapReturnvalue(Object*);
};

}


#endif // EVALUATOR_H
