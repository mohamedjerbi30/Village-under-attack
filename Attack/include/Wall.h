#ifndef WALL_H
#define WALL_H
#include "Resources.h"
#include <Building.h>


class Wall : public Building
{
    public:
        Wall(Position position);
        virtual ~Wall();

};

#endif // WALL_H

