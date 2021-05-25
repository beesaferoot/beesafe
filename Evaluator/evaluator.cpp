#include "evaluator.h"
using namespace evaluator;


BoolObject* NATIVE_TRUE = new BoolObject(true);
BoolObject* NATIVE_FALSE = new BoolObject(false);

Object* NATIVE_NULL = new NullObject();

Evaluator::Evaluator()
{

}
Evaluator::~Evaluator()
{
    delete NATIVE_TRUE;
    delete NATIVE_FALSE;
    delete NATIVE_NULL;
}

Object* Evaluator::Eval(Node* node, Env* env)
{
   if(BlockStmt* block = dynamic_cast<BlockStmt*>(node)){
       return evalBlockStatement(block, env);
   }
   else if(IfStmt* stmt = dynamic_cast<IfStmt*>(node)){
       return evalIfStatement(stmt, env);
   }
   else if(ReturnStmt* stmt = dynamic_cast<ReturnStmt*>(node)){
       auto value = Eval(stmt->returnValue, env);
       if(isError(value)){
           return value;
       }
       return new ReturnObject(value);
   }
   else if(InitStmt* stmt = dynamic_cast<InitStmt*>(node)){
       return evalInitStatement(stmt, env);
   }
   else if(DeclareStmt* stmt = dynamic_cast<DeclareStmt*>(node)){
       return evalDecalareStatement(stmt, env);
   }
   else if(FunctionStmt* stmt = dynamic_cast<FunctionStmt*>(node)){
       return evalFunctionStmt(stmt, env);
   }
   else if(ExpressionStmt* stmt = dynamic_cast<ExpressionStmt*>(node)){
       return Eval(stmt->Expression, env);
   }
   else if(CallExpression* expr = dynamic_cast<CallExpression*>(node)){

       auto function = Eval(expr->function, env);
       if(isError(function)){
           return function;
       }
       auto args = evalExpressions(expr->arguments, env);
       if(args.size() == 1 && isError(args[0])){
            return args[0];
       }
       return applyFunction(function, args);
   }
   else if(FunctionLiteral* func = dynamic_cast<FunctionLiteral*>(node)){
       return new FunctionObject(func->parameters, func->body, env);
   }
   else if(Assign* expr = dynamic_cast<Assign*>(node)){
       return evalAssignExpression(expr, env);
   }
   else if(Identifier* ident = dynamic_cast<Identifier*>(node)){
       return evalIdentifier(ident, env);
   }
   else if(StringLiteral* literal = dynamic_cast<StringLiteral*>(node)){
       return new StringObject(literal->Literal);
   }
   else if(Number* num = dynamic_cast<Number*>(node)){
       return new IntObject(num->value);
   }
   else if(NullExpr* null = dynamic_cast<NullExpr*>(node)){
       return NATIVE_NULL;
   }
   else if(BooleanExpr* expr = dynamic_cast<BooleanExpr*>(node)){
       return nativeBooleanObject(expr->value);
   }
   else if(UniaryOp* Op = dynamic_cast<UniaryOp*>(node)){
       auto RightValue = Eval(Op->RightOp, env);
       if(isError(RightValue)){
           return RightValue;
       }
       return evalUniaryExpression(Op->tok, RightValue);
   }else if(BinaryOp* Op = dynamic_cast<BinaryOp*>(node)){
       auto LeftValue = Eval(Op->LeftOp, env);
       if(isError(LeftValue)){
           return LeftValue;
       }
       auto RightValue = Eval(Op->RightOp, env);
       if(isError(RightValue)){
           return RightValue;
       }
       return evalBinaryExpression(Op->tok, LeftValue, RightValue);
   }
   return nullptr;
}

list<Object*> Evaluator::evalProgram(Program *program, Env* env){
    list<Object*> evaluatedStmts;
    for(auto stmt : program->Stmts){
        Object* result = Eval(stmt, env);
        if(auto res = dynamic_cast<ReturnObject*>(result)){
            result =  res->value;
        }
        evaluatedStmts.push_back(result);
    }
    return evaluatedStmts;
}

Object* Evaluator::evalBlockStatement(BlockStmt *block, Env* env){
    auto scope = NewEnvironment(env);
    if(scope->is_recurseLimitExceeded())
        return newError("RecursionLimitExceeded: ", "recursion depth exceeded limit");

    for(auto stmt : block->Stmts){
        Object* result = Eval(stmt, scope);
        if(result != nullptr){
            auto type = result->type();
            if(type == Types::RETURNTYPE || type == Types::ERRORTYPE){
                return result;
            }
        }
    }
    return NATIVE_NULL;
}

Object* Evaluator::evalInitStatement(InitStmt *stmt,  Env* env){
    auto value = Eval(stmt->value, env);
    if(isError(value)){
        return value;
    }
    env->put(stmt->variable->toString(), value);
    return NATIVE_NULL;
}

Object* Evaluator::evalIdentifier(Identifier *ident, Env* env){
    auto value = env->get(ident->toString());
    if(value == nullptr){
        auto message = "name " + ident->toString() + " not defined";
        return newError("NameError: ", message);
    }
    return value;
}

Object* Evaluator::evalDecalareStatement(DeclareStmt *stmt, Env* env)
{
    for(auto ident : stmt->varList){
        auto result = Eval(ident, env);
        if(!isError(result)){
            auto message = "name " + ident->toString() + " has already been defined";
            return newError("ValueError: ", message);
        }
        env->put(ident->toString(), NATIVE_NULL);

    }
    return NATIVE_NULL;
}

Object* Evaluator::evalFunctionStmt(FunctionStmt *stmt, Env* env){
    auto function = dynamic_cast<FunctionLiteral*>(stmt->Expression);
    auto funcObj = Eval(stmt->Expression, env);
    if(isError(funcObj)){
        return funcObj;
    }
    auto funcName = function->header->tok.Literal;
    env->put(funcName, funcObj);
    return NATIVE_NULL;
}

std::vector<Object*> Evaluator::evalExpressions(std::vector<Expr *> exps, Env* env){
    std::vector<Object*> result;
    for(auto e: exps){
        auto evaluated = Eval(e, env);
        if(isError(evaluated)){
            return std::vector<Object*>{evaluated};
        };
        result.push_back(evaluated);
    }
    return result;
}

Object* Evaluator::applyFunction(Object *obj, std::vector<Object*> args){
       auto function = dynamic_cast<FunctionObject*>(obj);
       if( function == nullptr){
           auto message = "'"+ obj->toString() + "'" + " object not a callable";

       }
       auto env = extendFunctionEnv(function, args);
       auto evaluated = evalBlockStatement(function->body, env);
       return unWrapReturnvalue(evaluated);
}

Env* Evaluator::extendFunctionEnv(FunctionObject* function, std::vector<Object*> args){
    auto env = NewEnvironment(function->env);
    for(int paramIndex{0}; paramIndex < (int)function->parameters.size(); paramIndex++){
        env->put(function->parameters[paramIndex]->tok.Literal, args[paramIndex]);
    }
    return env;
}

Object* Evaluator::unWrapReturnvalue(Object * obj){
    if(auto returnValue = dynamic_cast<ReturnObject*>(obj)){
        return returnValue->value;
    }
    return obj;
}

Object* Evaluator::evalAssignExpression(Assign *expr, Env *env){
    auto leftValue = Eval(expr->LeftOp, env);
    if(isError(leftValue)){
        return leftValue;
    }
    auto rightValue = Eval(expr->RightOp, env);
    if(isError(rightValue)){
        return rightValue;
    }
    env->put(expr->LeftOp->toString(), rightValue);
    return NATIVE_NULL;
}

Object* Evaluator::evalIfStatement(IfStmt *stmt, Env* env){
    auto condition = Eval(stmt->condition, env);
    if(isError(condition)){
        return condition;
    }
    if(isTruthy(condition)){
        return  Eval(stmt->body, env);
    }else if(stmt->alternative != nullptr){
        return Eval(stmt->alternative, env);
    }
   return NATIVE_NULL;
}

Object* Evaluator::evalUniaryExpression(Token& op, Object * rightValue){

    switch (op.Type) {
    case BANG:
        return evalNotOperator(rightValue);
    case UMINUS:
        return evalUniaryMinusOperator(rightValue);
    default:
        return newError("Unknown Operator: ", op.Literal);
    }
}

Object* Evaluator::evalNotOperator(Object *rightExpr){
    if(rightExpr == NATIVE_TRUE){
        return NATIVE_FALSE;
    }else if(rightExpr == NATIVE_FALSE){
        return NATIVE_TRUE;
    }else if(rightExpr == NATIVE_NULL){
       return NATIVE_NULL;
    }
    return NATIVE_FALSE;
}

Object* Evaluator::evalUniaryMinusOperator(Object *rightExpr){
    if(rightExpr->type() != Types::INTTYPE){
        auto message = "-" + rightExpr->toString();
        return newError("TypeError: ", message);
    }
    auto value = dynamic_cast<IntObject*>(rightExpr)->value;
    return new IntObject(-value);
}

Object* Evaluator::evalBinaryExpression(Token& op, Object *leftExpr, Object *rightExpr){

   if(leftExpr->type() == Types::INTTYPE && rightExpr->type() == Types::INTTYPE){
       return evalIntergerBinaryExpression(op, leftExpr, rightExpr);
   }else if( leftExpr->type() == Types::STRINGTYPE && rightExpr->type() == Types::STRINGTYPE){
       return evalStringConcatenate(op, leftExpr, rightExpr);
   }
   else if( op.Type == TokenType::EQ) {
       return nativeBooleanObject(leftExpr == rightExpr);
   }else if( op.Type == TokenType::NOT_EQ){
       return nativeBooleanObject(leftExpr != rightExpr);
   }else if(leftExpr->type() != rightExpr->type()){
       auto message = leftExpr->toString() + " " + op.Literal + " " + rightExpr->toString();
       return newError("TypeError: ", message);
   }
   auto message = leftExpr->toString() + " " + op.Literal + " " + rightExpr->toString();
   return newError("Unknown Operator: ", message);
}

Object* Evaluator::evalStringConcatenate(Token& op, Object *left, Object *right){
    auto leftValue = dynamic_cast<StringObject*>(left)->value;
    auto rightValue = dynamic_cast<StringObject*>(right)->value;
    switch (op.Type) {
    case TokenType::PLUS:
    {
        return new StringObject(leftValue + rightValue);
    }
    case TokenType::EQ:
        return nativeBooleanObject(leftValue  == rightValue);
    case TokenType::NOT_EQ:
        return nativeBooleanObject(leftValue  != rightValue);
    case TokenType::LT:
        return nativeBooleanObject(leftValue  < rightValue);
    case TokenType::GT:
        return nativeBooleanObject(leftValue  > rightValue);
    default:
        return newError("Unknown Operator: ", op.Literal + " invalid operation on type string");
    }
}

Object* Evaluator::evalIntergerBinaryExpression(Token& op, Object* leftExpr, Object* rightExpr){

    auto leftValue = dynamic_cast<IntObject*>(leftExpr)->value;
    auto rightValue = dynamic_cast<IntObject*>(rightExpr)->value;
    switch (op.Type) {
     case TokenType::PLUS:
        return new IntObject(leftValue + rightValue);
     case TokenType::MINUS:
        return new IntObject(leftValue - rightValue);
     case TokenType::ASTERISK:
        return new IntObject(leftValue * rightValue);
     case TokenType::DIV:
        {
            if(rightValue == 0)
               return newError("ZeroDivisionError: ",  "division by zero");
            return new IntObject(leftValue / rightValue);
        }
     case TokenType::EQ:
        return nativeBooleanObject(leftValue == rightValue);
     case TokenType::NOT_EQ:
        return nativeBooleanObject(leftValue != rightValue);
     case TokenType::LT:
        return nativeBooleanObject(leftValue < rightValue);
     case TokenType::GT:
        return nativeBooleanObject(leftValue > rightValue);
     case TokenType::LT_EQ:
        return nativeBooleanObject(leftValue <= rightValue);
     case TokenType::GT_EQ:
        return nativeBooleanObject(leftValue >= rightValue);
     default:
        return newError("Unknown Operator: ", op.Literal);
    }
}

BoolObject* Evaluator::nativeBooleanObject(bool value){
    if(value)
        return NATIVE_TRUE;
    return NATIVE_FALSE;
}

bool Evaluator::isTruthy(Object * obj){
    if(obj == NATIVE_NULL)
        return false;
    else if( obj == NATIVE_TRUE)
        return true;
    else if( obj == NATIVE_FALSE)
        return false;
    return true;
}

ErrorObject* Evaluator::newError(std::string type, std::string message)
{
    return new ErrorObject(type, message);
}

bool Evaluator::isError(Object * obj){
    if(obj->type() == Types::ERRORTYPE)
        return true;
    return false;
}

Env* Evaluator::NewEnvironment(){
    return new Env(nullptr);
}

Env* Evaluator::NewEnvironment(Env* prev){
    return new Env(prev);
}
