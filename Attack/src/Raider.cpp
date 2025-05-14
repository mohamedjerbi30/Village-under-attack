#include "Raider.h"

Raider::Raider()
    : Enemy(Position(0, 0), L"👹", 50, 5, 1) {}

Raider::Raider(Position position)
    : Enemy(position, L"👹", 50, 5, 1) {}
