#include <gtest/gtest.h>
#include "Lexer/lexer.h"
#include <iostream>
#include<vector>
using namespace lexer;
using namespace token;
using std::cout;
using std::endl;

std::vector<Token> keywords(10);

TEST(TestLexer, TestNextToken){
    keywords.emplace_back(Word(TokenType::DECLARE, "declare"));
    keywords.emplace_back(Word(TokenType::ID, "x"));
    keywords.emplace_back(Symbol(TokenType::NEWLINE, "\n"));
    keywords.emplace_back(Word(TokenType::DECLARE, "declare"));
    keywords.emplace_back(Word(TokenType::ID, "y"));

    Lexer * testLexer = Lexer::New(R"(declare x
                                   declare y)");

    for(auto& kw : keywords){
        auto l = testLexer->NextToken();
        cout << l.Literal << endl;
        EXPECT_EQ(kw.Type, l.Type);
        EXPECT_EQ(kw.Literal, l.Literal);
    }
}