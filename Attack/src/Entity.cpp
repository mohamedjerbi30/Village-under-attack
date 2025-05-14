#include "Entity.h"

Entity::Entity() : position(0, 0), repr(L"") {}

Entity::Entity(Position position, std::wstring repr)
    : position(position), repr(repr) {}

Position Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const Position& pos) {
    position = pos;
}

std::wstring Entity::getRepr() const {
    return repr;
}
