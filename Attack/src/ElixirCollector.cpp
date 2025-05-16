#include "ElixirCollector.h"

ElixirCollector::ElixirCollector(Position p)
    : ResourceGenerator(p, 1, 1,"🔮",Resources(0, 100),3,150) {}

Resources ElixirCollector::Collect() {
    int collected = current;
    current = 0;
    isFull = false;
    return Resources(0, collected);
}

const char* ElixirCollector::getRepr() const {
    return isFull ? "⚗️" : "🔮";
}
ElixirCollector::~ElixirCollector()
{
    //dtor
}
