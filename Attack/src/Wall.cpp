#include "Wall.h"

Wall::Wall(Position position)
    : Building(position, 1, 1,"🧱",Resources(10, 0), 200,50) {}

Wall::~Wall()
{
    //dtor
}
