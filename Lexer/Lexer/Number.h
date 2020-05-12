#ifndef NUMBER_H
#define NUMBER_H
#include"token.h"
using namespace token;

class Num : public Token{

public:
    Num(int v)
        :Token(TokenType::NUM, std::to_string(v)), value(v){}
    ~Num(){}
    std::ostream& operator<<(std::ostream &out)
    {
        out << std::to_string(this->value);
        return out;
    }
    const int value;
};

#endif // NUMBER_H
