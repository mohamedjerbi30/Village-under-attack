#include "Troop.h"
#include "Board.h"
#include "Enemy.h"
#include <cmath>
#include <algorithm>

Troop::Troop()
    : Npc(), cost(0, 0), attackDamage(0), attackRange(0), targetedEnemy(nullptr) {}

Troop::Troop(Position position, const char* repr, int health, int attackDamage, int attackRange, int goldCost, int elixirCost)
    : Npc(position, repr, health), cost(goldCost, elixirCost), attackDamage(attackDamage), attackRange(attackRange), targetedEnemy(nullptr) {}

Resources Troop::getCost() const {
    return cost;
}

int Troop::getAttackDamage() const {
    return attackDamage;
}

int Troop::getAttackRange() const {
    return attackRange;
}

Enemy* Troop::getTargetedEnemy() const {
    return targetedEnemy;
}

void Troop::setTargetedEnemy(Enemy* enemy) {
    targetedEnemy = enemy;
}

void Troop::Update(Board& board) {
    if (!isAlive()) return;

    // If we have no target or target is dead, find a new one
    if (!targetedEnemy || !targetedEnemy->isAlive()) {
        TargetEnemy(board.getEnemies());
    }

    // If we have a target, attack or move towards it
    if (targetedEnemy) {
        MoveOrAttack(board);
    }
}

void Troop::Attack(Enemy* enemy) {
    if (enemy && enemy->isAlive()) {
        enemy->takeDamage(attackDamage);
        takeDamage(3); // Troops lose 3 health when attacking
    }
}

void Troop::MoveOrAttack(Board& board) {
    if (!targetedEnemy) return;

    Position enemyPos = targetedEnemy->getPosition();
    int dx = abs(position.X - enemyPos.X);
    int dy = abs(position.Y - enemyPos.Y);
    int distance = std::max(dx, dy); // Using Chebyshev distance

    if (distance <= attackRange) {
        // Enemy is in range, attack
        Attack(targetedEnemy);
    } else {
        // Move towards enemy
        int moveX = 0, moveY = 0;

        if (position.X < enemyPos.X) moveX = 1;
        else if (position.X > enemyPos.X) moveX = -1;

        if (position.Y < enemyPos.Y) moveY = 1;
        else if (position.Y > enemyPos.Y) moveY = -1;

        // Try to move
        Position newPos(position.X + moveX, position.Y + moveY);

        // Check if the new position is valid
        if (newPos.X > 0 && newPos.X < board.getSizeX() - 1 &&
            newPos.Y > 0 && newPos.Y < board.getSizeY() - 1) {

            // Check for collisions with buildings
            bool collision = false;
            for (Building* building : board.getBuildings()) {
                if (building->collidesWith(newPos)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                position = newPos;
            }
        }
    }
}

bool Troop::AnyTroopNearTarget(const std::vector<Npc*>& entities) {
    if (!targetedEnemy) return false;

    for (Npc* entity : entities) {
        Troop* troop = dynamic_cast<Troop*>(entity);
        if (troop && troop != this && troop->isAlive()) {
            Position troopPos = troop->getPosition();
            Position enemyPos = targetedEnemy->getPosition();

            int dx = abs(troopPos.X - enemyPos.X);
            int dy = abs(troopPos.Y - enemyPos.Y);

            if (dx <= 1 && dy <= 1) {
                return true;
            }
        }
    }

    return false;
}

Enemy* Troop::GetNearestEnemy(const std::vector<Enemy*>& entities) {
    Enemy* nearest = nullptr;
    int minDistance = INT_MAX;

    for (Enemy* enemy : entities) {
        if (enemy->isAlive()) {
            Position enemyPos = enemy->getPosition();
            int dx = position.X - enemyPos.X;
            int dy = position.Y - enemyPos.Y;
            int distance = dx * dx + dy * dy;  // Squared distance

            if (distance < minDistance) {
                minDistance = distance;
                nearest = enemy;
            }
        }
    }

    return nearest;
}

void Troop::TargetEnemy(const std::vector<Enemy*>& entities) {
    if (entities.empty()) {
        targetedEnemy = nullptr;
        return;
    }

    targetedEnemy = GetNearestEnemy(entities);
}
