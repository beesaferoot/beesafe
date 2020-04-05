#include"type.h"
using namespace symbols;

type_ptr Type::Int = new Type("int", TokenType::BASIC, 4);
type_ptr Type::Float = new Type("float", TokenType::BASIC, 8);
type_ptr Type::Char = new Type("char", TokenType::BASIC, 1);
type_ptr Type::Bool = new Type("bool", TokenType::BASIC, 1);

bool Type::numeric(type_ptr p)
{
    if(p == Type::Char || p == Type::Int || p == Type::Float)
        return true;
    else
        return false;
}
type_ptr Type::max(type_ptr p1, type_ptr p2)
{
    if(!numeric(p1) || !numeric(p2))
        return nullptr;
    else if(p1 == Type::Float || p2 == Type::Float)
        return  Type::Float;
    else if(p1 == Type::Int || p2 == Type::Int)
        return Type::Int;
    else
        return Type::Char;

}
