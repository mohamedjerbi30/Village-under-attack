#include "Position.h"
#include <cstdlib>

Position::Position(int x, int y) : X(x), Y(y) {}

Position::~Position() {

}
bool operator==(const Position& p1, const Position& p2) {
    return (p1.X== p2.X) && (p1.Y == p2.Y);
}

bool operator!=(const Position& p1, const Position& p2) {
    return !(p1 == p2);
}

Position operator+(const Position& p1, const Position& p2) {
    return Position(p1.X + p2.X, p1.Y + p2.Y);
}

int Position::manhattanDistance(const Position& other) const {
    return std::abs(X - other.X) + std::abs(Y - other.Y);
}
