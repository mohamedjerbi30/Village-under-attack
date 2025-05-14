#include "Building.h"

Building::Building()
    : position(0, 0), sizeX(1), sizeY(1), repr(""),
      cost(0, 0), maxInstances(0), health(100) {}

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
    // Calculer les limites du bâtiment
    int leftX = position.X - sizeX / 2;
    int rightX = position.X + sizeX / 2;
    int topY = position.Y - sizeY / 2;
    int bottomY = position.Y + sizeY / 2;

    // Vérifier si la position est à l'intérieur des limites
    return (pos.X >= leftX && pos.X <= rightX &&
            pos.Y >= topY && pos.Y <= bottomY);
}

bool Building::collidesWith(const Building& other) const {
    // Calculer les limites des deux bâtiments
    int thisLeftX = position.X - sizeX / 2;
    int thisRightX = position.X + sizeX / 2;
    int thisTopY = position.Y - sizeY / 2;
    int thisBottomY = position.Y + sizeY / 2;

    int otherLeftX = other.position.X - other.sizeX / 2;
    int otherRightX = other.position.X + other.sizeX / 2;
    int otherTopY = other.position.Y - other.sizeY / 2;
    int otherBottomY = other.position.Y + other.sizeY / 2;

    // Vérifier s'il y a une intersection entre les deux rectangles
    return !(thisRightX < otherLeftX || thisLeftX > otherRightX ||
             thisBottomY < otherTopY || thisTopY > otherBottomY);
}
