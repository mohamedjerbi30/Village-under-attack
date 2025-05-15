#include "Raider.h"
#include <limits>
#include <ctime>
#include <windows.h> // Pour GetTickCount()

Raider::~Raider() {}

Raider::Raider(Position position)
    : Enemy(position, 1), moveDelay(1000) // délai entre mouvements
{
    repr = "😈";
    lastMoveTime = GetTickCount(); // initialisation
}

Building* Raider::GetNearestBuilding(const std::vector<Building*>& buildings) {
    Building* nearest = nullptr;
    int minDistance = std::numeric_limits<int>::max();

    for (Building* building : buildings) {
        int dist = calculateManhattanDistance(getPosition(), building->getPosition());
        if (dist < minDistance) {
            minDistance = dist;
            nearest = building;
        }
    }
    return nearest;
}
void Raider::Update(const Board& board) {
    unsigned long currentTime = GetTickCount();
    if (currentTime - lastMoveTime < moveDelay) {
        return; // trop tôt pour se déplacer
    }

    Building* target = GetNearestBuilding(board.getBuildings());
    if (!target) return;

    if (target->collidesWith(getPosition())) {
        AttackBuilding(board);
    } else {
        moveTowards(target->getPosition(),board);
    }

    lastMoveTime = currentTime;
}

int Raider::calculateManhattanDistance(const Position& p1, const Position& p2) const {
    return abs(p1.X - p2.X) + abs(p1.Y - p2.Y);
}
