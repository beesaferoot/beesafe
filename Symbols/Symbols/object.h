#ifndef OBJECT_H
#define OBJECT_H
#include<string>
#include<iostream>
#include<vector>
#include"ast.h"
//#include"env.h"

using namespace ast;
namespace symbols{

enum Types {
    INTTYPE,
    BOOLTYPE,
    NULLTYPE,
    RETURNTYPE,
    ERRORTYPE,
    FUNCTYPE,
    STRINGTYPE

};


// Base Object System
struct Object {
    virtual Types type(){
        return _type;
    }
    Object(Types obj)
        :_type{obj} {}
    Types _type;
    virtual std::string toString() const = 0;
    friend std::ostream& operator<<(std::ostream &out, Object const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// Interger Representation
struct IntObject: Object {
    long value;
    IntObject(int v)
        :Object(Types::INTTYPE), value{v} {}
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
    {}
    ~Env();
    void put(std::string, Object*);
    Object* get(std::string);
private:
    std::unordered_map<std::string, Object*> table;
    Env* prev;
};

// Function Representation
struct FunctionObject: Object {
    std::vector<Identifier*> parameters;
    BlockStmt* body;
    Env* env;
    FunctionObject( std::vector<Identifier*> params, BlockStmt* b, Env* e)
        :FunctionObject(){
        parameters = params;
        body = b;
        env = e;
    }
    FunctionObject()
        :Object(Types::FUNCTYPE){}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, FunctionObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};

// String Representation
struct StringObject: Object {
    std::string value;
    StringObject(std::string v)
        :Object(Types::STRINGTYPE), value{v} {}
    virtual std::string toString() const;
    friend std::ostream& operator<<(std::ostream &out, StringObject const *obj)
    {
        out << obj->toString();
        return out;
    }
};

}
#endif // OBJECT_H
