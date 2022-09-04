#include "interpreter.h"

using namespace symbols;
using namespace evaluator;

void interpreter::Interpreter::run()
{
    auto parsed_src = this->parser->parseProgram();
    if(checkParseErrors()) return;
    auto eval_src = Evaluator::evalProgram(parsed_src, this->global_env);
    if(checkEvalErrors(eval_src)) return;
}

bool  interpreter::Interpreter::checkEvalErrors(list<GCPtr<Object>> eval_src)
{
    bool hasErrors = false;
    for(auto eval : eval_src){
        if(eval->type() == Types::ERRORTYPE){
            GCPtr<ErrorObject> err = static_cast<ErrorObject*>(eval.raw());
            cout << err->toString() << endl;
            hasErrors = true;
        }
    }
    return hasErrors;
}

bool  interpreter::Interpreter::checkParseErrors()
{
    if(this->parser->hasErrors()){
        cout << this->src_file_path << ": " << endl;
        for(auto &err: this->parser->Errors()){
            cout << err << endl;
        }
        return true;
    }
    return false;
}
