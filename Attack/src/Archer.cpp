#include "Archer.h"

Archer::Archer()
    : Troop(Position(0, 0), "🏹", 25, 5, 7, 0, 15) {}

Archer::Archer(Position position)
    : Troop(position, "🏹", 25, 5, 7, 0, 15) {}
