#include "Building.h"
#include <cstring>

Building::Building()
    : position(), sizeX(1), sizeY(1), repr(""), cost(), maxInstances(0), health(100) {}

Building::Building(Position position, int sizeX, int sizeY, const char* repr,
                 int goldCost, int elixirCost, int maxInstances, int health)
    : position(position), sizeX(sizeX), sizeY(sizeY), repr(repr),
      cost(goldCost, elixirCost), maxInstances(maxInstances), health(health) {}

Position Building::getPosition() const {
    return position;
}

int Building::getSizeX() const {
    return sizeX;
}

int Building::getSizeY() const {
    return sizeY;
}

const char* Building::getRepr() const {
    return repr;
}

Resources Building::getCost() const {
    return cost;
}

int Building::getMaxInstances() const {
    return maxInstances;
}

int Building::getHealth() const {
    return health;
}

void Building::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }
}

bool Building::isDestroyed() const {
    return health <= 0;
}

bool Building::collidesWith(const Position& pos) const {
    return (pos.X >= position.X && pos.X < position.X + sizeX &&
            pos.Y >= position.Y && pos.Y < position.Y + sizeY);
}

bool Building::collidesWith(const Building& other) const {
    // Check if this building's area overlaps with the other building's area
    return !(position.X + sizeX <= other.position.X ||
             other.position.X + other.sizeX <= position.X ||
             position.Y + sizeY <= other.position.Y ||
             other.position.Y + other.sizeY <= position.Y);
}
