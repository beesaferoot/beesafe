#ifndef LEXER_H
#define LEXER_H
#include"token.h"
#include"Number.h"
#include"Word.h"
#include"Symbol.h"
#include"Operator.h"


namespace lexer {

class Lexer
{
public:
    explicit Lexer(std::string inp):input{inp},readPosition{0}{
        reserve(Word(TokenType::TRUE, "true"));
        reserve(Word(TokenType::FALSE, "false"));
        reserve(Word(TokenType::DEFINE, "define"));
        reserve(Word(TokenType::DECLARE, "declare"));
        reserve(Word(TokenType::INIT, "init"));
        reserve(Word(TokenType::IF, "if"));
        reserve(Word(TokenType::ELSE, "else"));
        reserve(Word(TokenType::FOR, "for"));
        reserve(Word(TokenType::RETURN, "return"));
        reserve(Word(TokenType::IN, "in"));
        reserve(Word(TokenType::_NULL, "null"));
    };
    static Lexer* New(std::string input);
    token::Token NextToken();
    void skipWhiteSpace();
    void readChar();
    char peekChar();
    bool isLetter(char);
    bool isDigit(char);
    std::string readIdent();
    std::string readInt();
    std::string readStringLiteral(char);
    void reserve(Word);
    void setInputBuf(std::string input )
    {
        readPosition = 0;
        this->input = input;
        readChar();
    }
    int line = 1;
private:
    std::string input;
    int position;
    int readPosition;
    char curChar;
    static Table keywords;
};

}

#endif // LEXER_H
