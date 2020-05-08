#ifndef OPERATOR_H
#define OPERATOR_H
#include"token.h"
using namespace token;

class Operator : public Token{

public:
    Operator(TokenType op, std::string literal)
        :Token(op, literal), symbol(literal){}
    ~Operator(){}
    std::string symbol;
};

#endif // OPERATOR_H
