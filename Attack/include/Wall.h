#ifndef WALL_H
#define WALL_H

#include "Building.h"

class Wall : public Building {
public:
    Wall();
    Wall(Position position);
};

#endif // WALL_H
