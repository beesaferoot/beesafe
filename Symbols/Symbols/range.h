#ifndef RANG_H
#define RANGE_H
#include "type.h"
using namespace symbols;

namespace symbols{

struct RangeGen : public Type{
    int start;
    int end; 
    RangeGen(int s, int e)
        :Type("(..)", TokenType::RANGE, Type::Int->width), start{s}, end{e} {}
};

}

#endif //RANGE_H
