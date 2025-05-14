#include "Position.h"

Position::Position() : X(0), Y(0) {}

Position::Position(int x, int y) : X(x), Y(y) {}

bool Position::operator==(const Position& other) const {
    return X == other.X && Y == other.Y;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}
