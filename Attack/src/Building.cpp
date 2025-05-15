#include "Building.h"
#include "Board.h"

Building::Building(Position position, int sX, int sY, const char* r, Resources c, int max, int hp)
    : p(position), sizeX(sX), sizeY(sY), repr(r), cost(c), maxInstances(max), health(hp) {}

bool Building::collidesWith(Position position) const {
    int left = p.X - sizeX/2;
    int right = p.X + sizeX/2;
    int top = p.Y - sizeY/2;
    int bottom = p.Y + sizeY/2;

    return (position.X >= left) && (position.X <= right) &&
           (position.Y >= top) && (position.Y <= bottom);
}

bool Building::collidesWith(const Building& other) const {
    int thisLeft = p.X - sizeX/2;
    int thisRight = p.X + sizeX/2;
    int thisTop = p.Y - sizeY/2;
    int thisBottom = p.Y + sizeY/2;

    int otherLeft = other.p.X - other.sizeX/2;
    int otherRight = other.p.X + other.sizeX/2;
    int otherTop = other.p.Y- other.sizeY/2;
    int otherBottom = other.p.Y + other.sizeY/2;

    return !(thisRight < otherLeft ||
             thisLeft > otherRight ||
             thisBottom < otherTop ||
             thisTop > otherBottom);
}

void Building::update() {}

void Building::loseHealth(int amount) {
    health -= amount;
    if (health < 0) {
        health = 0; // On évite que la santé devienne négative
    }
}
//ajouté
bool Building::isAlive() const {
    return health > 0;
}
//ajouté
