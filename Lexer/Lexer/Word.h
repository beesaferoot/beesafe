#ifndef WORD_H
#define WORD_H
#include"token.h"
using namespace token;

class Word : public Token{

public:
    Word(TokenType type, std::string literal)
        :Token(type, literal), lexeme(literal){}
    ~Word(){}
   std::string lexeme;
};

#endif // WORD_H
