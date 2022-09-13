#include <iostream>
#include <random>
#include "Parser/parser.h"
#include "Evaluator/evaluator.h"
#include "Lexer/lexer.h"

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

void spitParseErrors(std::vector<string> errors){
    cout << "syntax errors:\n";
    for(auto& error : errors){
        cout << error << endl;
    }
}

void read_lines( istream& in, list<string>& list ) {
    std::string line;
    std::string prev_line;
    while( true ) {
        std::getline( in, line );
        if(!line.empty()) {
            list.push_back( line );
        }else if (prev_line.empty()){
            break;
        }
        prev_line = line;
    }
}

string join_lines(list<string> & lines){
    string str;
    for(string& line: lines){
        str += line + "\n";
    }
    return str;
}

int main()
{
    cout << "Welcome to BEESAFE :) REPL v1.0 " << endl;
    cout << "Type CTRL-D to Exit" << endl;
    auto env = Evaluator::NewEnvironment();
    while (true){
        cout << PROMPT;
        list<string> buffer_list;
        read_lines(cin, buffer_list );
        if(cin.eof()){
            cout << "\t" << randomMessageGen() << "..." << endl;
            break;
        }
        auto l = lexer::Lexer::New(join_lines(buffer_list));
        auto p = parser::Parser::New(l);
        auto program = p->parseProgram();
        cout << "Parse -> "<< program << endl;
        if (!p->hasErrors()){
            auto evalStmts = Evaluator::evalProgram(program, env);
            cout << "evaluated statement size: " << evalStmts.size() << endl;
            cout << "Eval -> " << evalStmts.front() << endl;
        }else{
            spitParseErrors(p->Errors());
        }
    }
}
