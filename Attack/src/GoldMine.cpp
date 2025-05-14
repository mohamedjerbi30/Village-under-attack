#include "GoldMine.h"

GoldMine::GoldMine()
    : ResourceGenerator(Position(0, 0), 3, 3, L"⛏", L"🪙",
                         0, 100, 3, 200, 100, 5) {}

GoldMine::GoldMine(Position position)
    : ResourceGenerator(position, 3, 3, L"⛏", L"🪙",
                         0, 100, 3, 200, 100, 5) {}
