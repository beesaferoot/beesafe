#ifndef OBJECT_H
#define OBJECT_H
#include<string>
#include<iostream>
#include<vector>
#include"ast/ast.h"

using namespace ast;
namespace symbols{

enum Types {
    INTTYPE,
    BOOLTYPE,
    NULLTYPE,
    RETURNTYPE,
    ERRORTYPE,
    FUNCTYPE,
    STRINGTYPE,
    RANGETYPE,
    ITERTYPE
};


// Base Object System
struct Object {
    virtual Types type(){
        return _type;
    }
    Object(Types obj)
        :_type{obj} {}
    virtual ~Object(){}
    Types _type;
    virtual std::string toString() const = 0;
    friend std::ostream& operator<<(std::ostream &out, Object const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// Iterator Representation
struct IteratorObject: Object
{
    IteratorObject()
        :Object(Types::ITERTYPE) {}
    IteratorObject(Types obj)
        :Object(obj) {}
    virtual ~IteratorObject() {
    }
    virtual Object* nextItem() = 0;

};


// Interger Representation
struct IntObject: Object {
    long value;
    IntObject(int v)
        :Object(Types::INTTYPE), value{v} {}
    ~IntObject(){

    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, IntObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};


// Booleans
struct BoolObject: Object {
    bool value;
    BoolObject(bool v)
        :Object(Types::BOOLTYPE), value(v) {}
    ~BoolObject(){

    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, BoolObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// NULL value Representation
struct NullObject: Object {
    NullObject()
        :Object(Types::NULLTYPE) {}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, NullObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// Return Object
struct ReturnObject: Object{
    Object* value;
    ReturnObject(Object* obj)
        :Object(Types::RETURNTYPE), value{obj}{}
    ~ReturnObject(){
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out,ReturnObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// Error Representation
struct ErrorObject: Object{
    std::string message;
    std::string type;
    ErrorObject()
        :Object(Types::ERRORTYPE), type{"Error: "}{}
    ErrorObject(std::string t, std::string m)
        :ErrorObject(){
        type = t;
        message = m;
    }
    ~ErrorObject(){
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, ErrorObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};




// Scope Environment
class Env
{
public:
    Env(Env* p)
        :prev{p}
    {
        if(prev != nullptr)
            setDepth(prev->depth);
        else
            setDepth(-1);
    }
    ~Env();
    void put(std::string, Object*);
    Object* get(std::string);
    bool is_recurseLimitExceeded() const;
private:
    std::unordered_map<std::string, Object*> table;
    Env* prev;
    int depth = 1;
    void setDepth(int);
    static const int recurseLimit = 50;
};

// Function Representation
struct FunctionObject: Object {
    std::vector<struct Identifier*> parameters;
    struct BlockStmt* body;
    Env* env;
    FunctionObject( std::vector<struct Identifier*> params, struct BlockStmt* b, Env* e)
        :FunctionObject(){
        parameters = params;
        body = b;
        env = e;
    }
    FunctionObject()
        :Object(Types::FUNCTYPE){}
    ~FunctionObject(){
       for(auto param : parameters){
           delete param;
       }
       parameters.clear();
       delete body;
       body = nullptr;
    }
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, FunctionObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// Range Object Representation
struct RangeObject: IteratorObject{
    int init_value;
    int end_value;
    int iter_value;
    RangeObject(int start, int end)
        :RangeObject(){
        iter_value = init_value = start;
        end_value = end;
    }
    RangeObject()
        :IteratorObject(Types::RANGETYPE){}
    ~RangeObject(){

    }
    virtual std::string toString() const;
    Object* nextItem();
    friend std::ostream& operator<<(std::ostream &out, RangeObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// String Representation
struct StringObject: IteratorObject {
    std::string value;
    StringObject(std::string v)
        :IteratorObject(Types::STRINGTYPE), value{v} {}
    ~StringObject(){

    }
    virtual std::string toString() const;
    Object* nextItem();
    friend std::ostream& operator<<(std::ostream &out, StringObject const *obj)
    {
        out << obj->toString();
        return out;
    }

private:
    int iter_index = 0;
};

}
#endif // OBJECT_H


//dynamic_cast<IteratorObject>(iter) !=  nullptr else ErrorObject("Not an iterator")
//while ( (auto item = iter.nextItem()) != NullObject){
//    Env.put(item)
//    evalBlockStatement(env);

//}
