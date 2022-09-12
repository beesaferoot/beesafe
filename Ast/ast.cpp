#include "ast.h"

std::string ast::BlockStmt::toString() const {
    std::ostringstream msg;
    msg << "{" << std::endl;
    for(auto stmt : Stmts){
        msg << stmt;
        msg << std::endl;
    }
    msg << "}";
    return msg.str();
};

std::string ast::ReturnStmt::toString() const {
    std::ostringstream msg;
    msg << "return ";
    msg << returnValue;
    msg << std::endl;
    return msg.str();
};

std::string ast::Program::toString() const {
    std::ostringstream msg;
    for(auto stmt : Stmts){
         msg << stmt;
         msg << std::endl;
     }
     return msg.str();
 };

std::string ast::Identifier::toString() const {
    std::ostringstream msg;
    msg << tok.Literal;
    return msg.str();
};

std::string ast::StringLiteral::toString() const {
    std::ostringstream msg;
    msg << "\"" << Literal << "\"";
    return msg.str();
};

std::string ast::Number::toString() const {
    std::ostringstream msg;
    msg << std::to_string(value);
    return msg.str();
};

std::string ast::ForRangeStmt::toString() const {
    std::ostringstream msg;
    msg << "for ";
    msg << target << " in ";
    msg << "(";
    msg << iter;
    msg << ")";
    msg << body;
    return msg.str();
};

std::string ast::RangeExpr::toString() const {
    std::ostringstream msg;
    msg << std::to_string(init_value);
    msg << "..";
    msg << std::to_string(end_value);
    return msg.str();
};

std::string ast::CallExpression::toString() const
{
    std::ostringstream msg;
    msg << function;
    msg << "(";
    for(int i{0}; i < (int)arguments.size(); i++){
        if(i > 0 && i+1 <= (int)arguments.size()){
            msg << ", ";
        }
        msg << arguments[i];
    }
    msg << ")" << std::endl;
    return msg.str();
}

std::string ast::FunctionLiteral::toString() const
{
    std::ostringstream msg;
    msg << "define ";
    if(header != nullptr){
        msg << header;
    }
    msg << "(";
    for(int i{0}; i < (int)parameters.size(); i++){
        if(i > 0 && i+1 <= (int)parameters.size()){
            msg << ", ";
        }
        msg << parameters[i];
    }
    msg << ")";
    msg << body;
    msg << std::endl;
    return msg.str();
}

std::string ast::ExpressionStmt::toString() const
{
    std::ostringstream msg;
    msg << Expression;
    return msg.str();
}

std::string ast::IfStmt::toString() const
{
    std::ostringstream msg;
    msg << "if ";
    msg << condition << " ";
    msg <<body;
    if (alternative != nullptr){
        msg << "else ";
        msg << alternative;
    }
    return msg.str();
}

std::string ast::WhileStmt::toString() const
{
    std::ostringstream msg;
    msg << "while ";
    msg << condition << " ";
    msg << body;
    return msg.str();
}

std::string ast::DeclareStmt::toString() const
{
    std::ostringstream msg;
    msg << "declare ";
    for(int i{0}; i < (int)varList.size(); i++){
        if(i > 0 && i+1 <= (int)varList.size()){
            msg << ", ";
        }
        msg << varList[i];
    }
    msg << std::endl;
    return msg.str();
}

std::string ast::InitStmt::toString() const
{
    std::ostringstream msg;
    msg << "init ";
    msg << variable;
    msg << " = ";
    msg << value << std::endl;
    return msg.str();
}

std::string ast::BooleanExpr::toString() const
{
    std::ostringstream msg;
    msg  << std::to_string(value);
    return msg.str();
}

std::string ast::Assign::toString() const
{
    std::ostringstream msg;
    msg << LeftOp;
    msg << " = ";
    msg << RightOp;
    return msg.str();
}

std::string ast::Add::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " + ";
    msg << RightOp;
    msg << ")";
    return msg.str();

}

std::string ast::Mult::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " * ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::Sub::toString() const {
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " - ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::Div::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " / ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::LessThan::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " < ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::GreaterThan::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " > ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::LessThanOrEqual::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " <= ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::GreaterThanOrEqual::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " >= ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::Equals::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " == ";
    msg << RightOp;
    msg << ")";
    return  msg.str();
}

std::string ast::NotEquals::toString() const
{
    std::ostringstream msg;
    msg << "(";
    msg << LeftOp;
    msg << " != ";
    msg << RightOp;
    msg << ")";
    return msg.str();
}

std::string ast::Not::toString() const
{
    std::ostringstream msg;
    msg << "(!";
    msg << RightOp << ")";
    return msg.str();
}

std::string ast::UniarySub::toString() const
{
    std::ostringstream msg;
    msg << "(-" << RightOp << ")";
    return msg.str();
}

std::string ast::NullExpr::toString() const
{
   return "null";
}
