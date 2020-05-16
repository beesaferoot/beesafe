#include<iostream>
#include<sstream>
#include<random>
#include<exception>
#include"parser.h"
#include"evaluator.h"

#define PROMPT ">>"
using namespace std;
using namespace evaluator;

// an harry potter fan?
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
    const int SIZE = 200;
    char lineBuffer[SIZE];
    cout << "Welcome to BEESAFE :) REPL v1.0 " << endl;
    cout << "Type CTRL-D to Exit" << endl;
    auto env = Evaluator::NewEnvironment();
    while (true){
        cout << PROMPT;
        cin.getline(lineBuffer, SIZE);
        if(cin.eof()){
            cout << "\t" << randomMessageGen() << "..." << endl;
            return 0;
        }
        // hack to add newline using semicolons ; :)
        for(int i{0}; i < cin.gcount(); ++i){
            if(lineBuffer[i] == ';')
                lineBuffer[i] = '\n';

        }
        auto l = Lexer::New(lineBuffer);
        auto p = parser::Parser::New(l);
        try {
            auto program = p->parseProgram();
            auto object = Evaluator::Eval(program, env);
//            cout << "Parse -> "<< program << endl;
//            cout << "Eval -> " << object << endl;
            cout << object << endl;
        } catch (parser::SyntaxError &err) {
            cerr << "SyntaxError: " << err.what()
             << endl;
        }
    }
    return 0;
}
