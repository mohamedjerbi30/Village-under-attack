#include "TownHall.h"


TownHall::TownHall(Position p): Building(p,5, 5,"🏰",Resources(0, 0),1,1000)
    {}
Position TownHall::getPosition()
{
    return p;
}
TownHall::~TownHall()
{
    //dtor
}
