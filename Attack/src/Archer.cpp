#include "Archer.h"

Archer::Archer(Position position)
    : Troop(position, 25, 0, 15) // health = 25, repr = "🏹", cost = 0 gold, 15 elixir
{
}

int Archer::GetAttackRange() const {
    return 7;
}

int Archer::GetAttackDamage() const {
    return 5;
}
