#ifndef BS_INTERPRETER_H
#define BS_INTERPRETER_H

#include "Evaluator/evaluator.h"
#include "Evaluator/gc.h"
#include "Parser/parser.h"
#include "Lexer/lexer.h"
#include "Symbols/object.h"

using namespace symbols;
using namespace parser;

namespace interpreter {

    class Interpreter {
    public:
        Interpreter(string source_file_path)
            :src_file_path(source_file_path)
            {
            auto source = resolve_open_source_path();
            auto lexer = Lexer::New(source);
            this->parser = new Parser(lexer);
            this->global_env = new Env(nullptr);
        }
        void run();
        bool checkEvalErrors(list<GCPtr<Object>> eval_src);
        bool checkParseErrors();

    private:
        string resolve_open_source_path();
        Parser* parser{nullptr};
        Env* global_env{nullptr};
        string src_file_path {};
    };
}
#endif //BS_INTERPRETER_H
