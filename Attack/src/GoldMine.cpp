#include "GoldMine.h"
#include "GoldMine.h"

GoldMine::GoldMine(Position p)
    : ResourceGenerator(p, 3, 3,"🪙",Resources(0, 100), 3,150) {}


Resources GoldMine::Collect() {
    int collected = current;
    current = 0;
    isFull = false;
    return Resources(collected, 0);
}

const char* GoldMine::getRepr() const {
    return isFull ? "⛏️" : "🪙";
}
GoldMine::~GoldMine()
{
    //dtor
}
