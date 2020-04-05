#ifndef TYPE_H
#define TYPE_H
#include"Word.h"

namespace symbols {

class Type : public Word
{

public:
    typedef Type* type_ptr;
    explicit Type(std::string s, TokenType t, int w)
        :Word(t, s), width{w}{}
    int width=0;
    static type_ptr Int, Float, Bool, Char;
    static bool numeric(type_ptr);
    static type_ptr max(type_ptr, type_ptr);
};

typedef Type* type_ptr;

}


#endif // TYPE_H
