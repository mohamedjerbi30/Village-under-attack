#include "Entity.h"

Entity::Entity() : position(0, 0), repr("") {}

Entity::Entity(Position position, const char* repr)
    : position(position), repr(repr) {}

Position Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const Position& pos) {
    position = pos;
}

const char* Entity::getRepr() const {
    return repr;
}
