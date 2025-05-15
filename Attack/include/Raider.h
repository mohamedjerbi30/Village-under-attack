#ifndef RAIDER_H
#define RAIDER_H

#include "Building.h"
#include "Board.h"
#include <vector>
#include <ctime>
#include <Enemy.h>

class Raider : public Enemy {
public:
    Raider(Position position);
    Building* GetNearestBuilding(const std::vector<Building*>& buildings);
    void Update(const Board& board) override;
    virtual ~Raider();

private:
    int calculateManhattanDistance(const Position& p1, const Position& p2) const;

    unsigned long lastMoveTime;
    unsigned int moveDelay; // en millisecondes
};

#endif // RAIDER_H
