#ifndef RAIDER_H
#define RAIDER_H

#include "Enemy.h"

class Raider : public Enemy {
public:
    Raider();
    Raider(Position position);
};

#endif // RAIDER_H
