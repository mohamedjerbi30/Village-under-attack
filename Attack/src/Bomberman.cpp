#include "Bomberman.h"
#include "Wall.h"

Bomberman::Bomberman(Position pos) :
    Enemy(pos, 50), explosionRange(3) {}

//ajouté

Building* findNearestWall(Board& board, Position from) {
    Building* nearest = nullptr;
    int minDist = INT_MAX;

    for (Entity* e : board.getEntities()) {
        Wall* wall = dynamic_cast<Wall*>(e);
        if (wall) {
            int dist = from.manhattanDistance(wall->getPosition());
            if (dist < minDist) {
                minDist = dist;
                nearest = wall;
            }
        }
    }

    return nearest;
}

//ajouté
void Bomberman::attackBuilding(Building* target) {
    if(position.manhattanDistance(target->getPosition()) <= 1) {
        target->loseHealth(10);
        this->takeDamage(100); // Meurt après l'explosion
    }
}

void Bomberman::update(Board& board) {
    Building* nearestWall = findNearestWall(board, this->getPosition());
    if(nearestWall) {
        moveTowards(nearestWall->getPosition(), board);
        attackBuilding(nearestWall);
    }
}
Bomberman::~Bomberman() {}
