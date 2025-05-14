#include "Wall.h"

Wall::Wall()
    : Building(Position(0, 0), 1, 1, L"🧱", 10, 0, 200, 100) {}

Wall::Wall(Position position)
    : Building(position, 1, 1, L"🧱", 10, 0, 200, 100) {}
