#include"object.h"
using namespace symbols;


Env::~Env()
{
}

void Env::setDepth(int depth_prev)
{
    depth = depth_prev + depth;
}

bool Env::is_recurseLimitExceeded() const
{
    if(recurseLimit <= depth)
        return true;
    return false;
}

void Env::put(std::string s, Object* obj)
{
    if(table.find(s) != table.end()){
        table[s] = obj;
    }else{
        table.insert({s, obj});
    }
}

Object* Env::get(std::string s)
{
    for(Env *e = this; e != nullptr; e = e->prev){
       auto element = e->table.find(s);
       if( element != e->table.end()){
           return element->second;
       }
    }
    // find in builtins
    auto builtin = Builtins.find(s);
    if( builtin != Builtins.end()){
        return builtin->second;
    }
    return nullptr;
}

bool Env::update(std::string s, Object* obj)
{
    for(Env *e = this; e != nullptr; e = e->prev){
        if(  e->table.find(s) != e->table.end()){
            e->table[s] = obj;
            return true;
        }
    }
    return false;
}

std::string IntObject::toString() const
{
   return std::to_string(value);
}


std::string BoolObject::toString() const
{
    if(value)
        return "true";
    return "false";

}

std::string NullObject::toString() const
{
    return "null";
}

std::string ReturnObject::toString() const
{
    return value->toString();
}

std::string ErrorObject::toString() const
{
    return type + message;
}

std::string FunctionObject::toString() const
{
    std::ostringstream out;
    out << "define ";
    out << "(";
    for(int i{0}; i < (int)parameters.size(); i++){
        if(i > 0 && i+1 <= (int)parameters.size()){
            out << ", ";
        }
        out << parameters[i];
    }
    out << ")";
    out << body;
    out << std::endl;
    return out.str();
}


Object* StringObject::nextItem()
{
    if(this->iter_index >= (int)this->value.size()){
        return new NullObject();
    }
    auto curStr = new StringObject(std::string(1, this->value.at(this->iter_index)));
    this->iter_index++;
    return curStr;
}

std::string StringObject::toString() const
{
    return "'" + value + "'";
}


Object* RangeObject::nextItem()
{
    if(iter_value <= end_value){
        return new IntObject(iter_value++);
    }else{
        iter_value = init_value;
        return nullptr;
    }
}

std::string RangeObject::toString() const
{
    std::ostringstream out;
    out << "range " << init_value << ".." << end_value;
    return out.str();
}

std::string BuiltinFunction::toString() const
{
    std::ostringstream out;
    out << "built-in: " << func_name << "() function";
    return out.str();
}
