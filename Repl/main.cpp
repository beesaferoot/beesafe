#include <iostream>
#include<random>
#include"parser.h"

#define PROMPT ">>"
using namespace std;


string randomMessageGen()
{
    random_device rd;
    mt19937 gen(rd());
    const int AVALIVABLE_MSG = 8;
    uniform_int_distribution<> dis(1, AVALIVABLE_MSG);
    switch (dis(gen)) {
      case 1:
         return "Riddikulus";
      case 2:
          return "Crucio";
      case 3:
          return "Lumos";
      case 4:
          return "Nox";
      case 5:
          return "Patronus";
      case 6:
         return "Imperio";
      case 7:
        return "Expelliarmus";
      default:
         return "Pulvis et umbra sumus";
    }
}
int main()
{


    const int SIZE = 90;
    char lineBuffer[SIZE];
    cout << "Welcome to BEESAFE :) REPL v1.0 " << endl;
    cout << "Type CTRL-D to Exit" << endl;
    while (true){
        cout << PROMPT;
        cin.getline(lineBuffer, SIZE);
        if(cin.eof()){
            cout << "\t" << randomMessageGen() << "..." << endl;
            return 0;
        }
        auto l = Lexer::New(lineBuffer);
        auto p = parser::Parser::New(l);
        try {
            auto program = p->parseProgram();
            cout << program;
        } catch (parser::SyntaxError &err) {
            cout << "SyntaxError: " << err.what()
             << endl;
        }


    }
    return 0;
}
