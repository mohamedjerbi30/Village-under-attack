#include "Entity.h"
#include "Board.h"
Entity::Entity(Position pos, const char* emoji)
    : position(pos), repr(emoji) {}

Position Entity::getPosition() const {
    return position;
}

const char* Entity::getRepr() const {
    return repr;
}

void Entity::moving(const Board& board, int dx, int dy) {
    Position newPos = position + Position(dx, dy);

    if (board.isPositionValid(newPos)) {
        position = newPos;
    }
}

