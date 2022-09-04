#ifndef WORD_H
#define WORD_H
#include"token.h"
using namespace token;

class Word : public Token{

public:
    Word(TokenType type, std::string literal)
        :Token(type, literal), lexeme(literal){}
    ~Word(){

    }
    std::ostream& operator<<(std::ostream &out)
    {
        out << this->lexeme;
        return out;
    }
   std::string lexeme;
};

#endif // WORD_H
