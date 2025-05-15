#ifndef ARCHER_H
#define ARCHER_H

#include "Troop.h"
#include "Position.h"

class Archer : public Troop {
public:
    Archer(Position position);

    int GetAttackRange() const ;
    int GetAttackDamage() const ;
    const char* getRepr() const { return "🏹";}
};

#endif
