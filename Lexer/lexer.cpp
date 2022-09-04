#include "lexer.h"
#include<ctype.h>
using namespace lexer;
using namespace token;

Table Lexer::keywords = {};
Lexer* Lexer::New(std::string input)
{
    Lexer* l = new Lexer(input);
    l->readChar();
    return l;
}

Token Lexer::NextToken()
{
    Token tok;
    skipWhiteSpace();

    switch (curChar) {
    case '.':
        if (peekChar() == '.'){
            char ch = curChar;
            readChar();
            std::string literal = "";
            literal += ch;
            literal += curChar;
            tok = Token(TokenType::RANGE, literal);
        }else{
            tok = Token(TokenType::INVALID, std::string(1, curChar));
        }
        break;
    case '\n':
        tok = Symbol(TokenType::NEWLINE, std::string(1, curChar));
        line++;
        break;
    case '+':
        tok = Operator(TokenType::PLUS, std::string(1, curChar));
        break;
    case '-':
        tok = Operator(TokenType::MINUS, std::string(1, curChar));
        break;
    case '*':
        tok = Operator(TokenType::ASTERISK, std::string(1, curChar));
        break;
    case '/':
        tok = Operator(TokenType::DIV, std::string(1, curChar));
        break;
    case '<':
        if (peekChar() == '='){
            char ch = curChar;
            readChar();
            std::string literal = "";
            literal += ch;
            literal += curChar;
            tok = Operator(TokenType::LT_EQ, literal);
        }else{
            tok = Operator(TokenType::LT, std::string(1, curChar));
        }
        break;
    case '>':
        if (peekChar() == '='){
            char ch = curChar;
            readChar();
            std::string literal = "";
            literal += ch;
            literal += curChar;
            tok = Operator(TokenType::GT_EQ, literal);
        }else{
            tok = Operator(TokenType::GT, std::string(1, curChar));
        }

        break;
    case '=':
        if (peekChar() == '='){
            char ch = curChar;
            readChar();
            std::string literal = "";
            literal += ch;
            literal += curChar;
            tok = Operator(TokenType::EQ, literal);
        }else{
            tok = Operator(TokenType::ASSIGN, std::string(1, curChar));
        }
        break;
    case '!':
        if (peekChar() == '='){
            char ch = curChar;
            readChar();
            std::string literal = "";
            literal += ch;
            literal += curChar;
            tok = Operator(TokenType::NOT_EQ, literal);
        }else{
            tok = Operator(TokenType::BANG, std::string(1, curChar));
        }
        break;
    case ',':
        tok = Symbol(TokenType::COMMA, std::string(1, curChar));
        break;
    case '{':
        tok = Symbol(TokenType::LBRACE, std::string(1, curChar));
        break;
    case '}':
        tok = Symbol(TokenType::RBRACE, std::string(1, curChar));
        break;
    case '(':
        tok = Symbol(TokenType::LPAREN, std::string(1, curChar));
        break;
    case ')':
        tok = Symbol(TokenType::RPAREN, std::string(1, curChar));
        break;
    case '\0':
        tok = Token(TokenType::EOB, std::string(1, curChar));
        break;
    default:
        if (isLetter(curChar)){
            std::string literal = readIdent();
            auto wsrch = Lexer::keywords.find(literal);
            if(wsrch != Lexer::keywords.end())
                tok  = wsrch->second;
            else
                tok = Word(TokenType::ID, literal);
            return tok;
        }else if (isDigit(curChar)){
            int val = std::stoi(readInt());
            tok = Num(val);
            return tok;
        }else if(curChar == '\'' || curChar == '"'){
            std::string literal = readStringLiteral(curChar);
            tok = Word(TokenType::LITERAL, literal);
        }
        else{
           tok = Token(TokenType::INVALID, std::string(1, curChar));
        }

    }
    readChar();
    return tok;
}

void Lexer::reserve(Word t)
{
    Lexer::keywords.insert({t.lexeme, t});
}

void Lexer::skipWhiteSpace()
{
    while(curChar == ' ' || curChar == '\t' || curChar == '\r'){
        readChar();
    }
}

void Lexer::readChar()
{
    if(readPosition >= (int) input.size()){
        curChar = '\0';
    }else{
        curChar = input.at(readPosition);
    }
    position = readPosition;
    readPosition++;
}

char Lexer::peekChar()
{
    if(readPosition >= (int) input.size()){
        return 0;
    }else{
        return input.at(readPosition);
    }
}

bool Lexer::isLetter(char character)
{
    return std::isalpha(character) || character == '_' ? true : false;
}


bool Lexer::isDigit(char character)
{
    return std::isdigit(character) ? true : false;
}

std::string Lexer::readIdent()
{
    int pos = position;
    while(isLetter(curChar) || std::isalnum(curChar)){
        readChar();
    }
    return input.substr(pos, position - pos);
}

std::string Lexer::readInt()
{
    int pos = position;
    while(isDigit(curChar)){
        readChar();
    }
    return input.substr(pos, position - pos);
}

std::string Lexer::readStringLiteral(char c)
{
    int pos = position+1;
    while(peekChar() != c){
        readChar();
    }
    readChar();
    return input.substr(pos, position - pos);
}
