#include "Barbarian.h"

Barbarian::Barbarian()
    : Troop(Position(0, 0), "⚔️", 10, 10, 1, 0, 10) {}

Barbarian::Barbarian(Position position)
    : Troop(position, "⚔️", 10, 10, 1, 0, 10) {}
