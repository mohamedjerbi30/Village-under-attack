#include "ResourceGenerator.h"

ResourceGenerator::ResourceGenerator(Position p, int sX, int sY, const char* r, Resources cost, int maxInst, int hp)
    : Building(p, sX, sY, r, cost, maxInst, hp),
      current(0), capacity(100), isFull(false) {}

ResourceGenerator::~ResourceGenerator()
{
    //dtor
}
void ResourceGenerator::update(const Board& board) {
    if (!isFull) {
        current += 1;
        if (current >= capacity) {
            current = capacity;
            isFull = true;
        }
    }
}
