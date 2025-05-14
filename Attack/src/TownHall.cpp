#include "TownHall.h"

TownHall::TownHall()
    : Building(Position(0, 0), 5, 5, "🏛", 0, 0, 1, 500) {}

TownHall::TownHall(Position position)
    : Building(position, 5, 5, "🏛", 0, 0, 1, 500) {}
