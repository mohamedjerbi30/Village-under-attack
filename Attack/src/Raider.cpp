#include "Raider.h"

Raider::Raider()
    : Enemy(Position(0, 0), "👹", 50, 5, 1) {}

Raider::Raider(Position position)
    : Enemy(position, "👹", 50, 5, 1) {}
