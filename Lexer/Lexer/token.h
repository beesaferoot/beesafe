#ifndef TOKEN_H
#define TOKEN_H
#include<string>
#include<unordered_map>


namespace token {
enum TokenType{
    NUM = 256,
    ID = 257,
    TRUE = 258,
    FALSE = 259,
    EQ = 260,
    NOT_EQ = 261,
    GT = 262,
    LT = 263,
    COMMA = 264,
    ASSIGN = 265,
    PLUS = 266,
    UMINUS = 292,
    MINUS = 267,
    BANG = 268,
    ASTERISK = 269,
    SLASH = 270,
    DEFINE = 271,
    INIT = 272,
    DECLARE = 273,
    FUNCTION = 274,
    NEWLINE = 275,
    EOB = 276,
    INVALID = 277,
    LBRACE = 278,
    RBRACE = 279,
    LPAREN = 280,
    RPAREN = 281,
    IF = 282,
    ELSE = 283,
    FOR = 284,
    RETURN = 285,
    BASIC = 286,
    LIST = 287,
    IN = 288,
    RANGE = 289,
    SENTINEL = 290
};

class Token{

public:
    TokenType Type;
    std::string Literal;
    Token(){}
    Token(TokenType Type, std::string literal)
        :Type{Type}, Literal{literal}
    {}
    bool operator==(const Token & rhs){
        return rhs.Type == this->Type && rhs.Literal == this->Literal;
    }
};

using Table = std::unordered_map<std::string, Token>;

}

#endif // TOKEN_H
