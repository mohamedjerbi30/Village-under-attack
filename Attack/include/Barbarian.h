#ifndef BARBARIAN_H
#define BARBARIAN_H

#include "Troop.h"
#include "Position.h"

class Barbarian : public Troop {
private:
    int AttackRange;
    static constexpr const char* REPR = "🪓";

public:
    explicit Barbarian(Position position);
    int GetAttackRange() const;
    const char* getRepr() const { return REPR; }

    void attack(Entity* target) ;
    void specialAbility() ;
};

#endif // BARBARIAN_H
