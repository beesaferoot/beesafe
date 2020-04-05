#ifndef LIST_H
#define LIST_H
#include"type.h"
using namespace symbols;

namespace symbols {

class List: public Type {

public:
    type_ptr of;
    int size;
    List(int sz, type_ptr p)
        :Type("[]", TokenType::LIST, sz*p->width), of{p}, size{sz}{}

    ~List()
    {
        delete of;
    }
};

}

#endif // LIST_H
