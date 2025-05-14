#include "Enemy.h"
#include "Board.h"
#include "Building.h"
#include "TownHall.h"
#include <cmath>
#include <algorithm>

Enemy::Enemy()
    : Npc(), attackDamage(10), moveSpeed(1) {}

Enemy::Enemy(Position position, const char* repr, int health, int attackDamage, int moveSpeed)
    : Npc(position, repr, health), attackDamage(attackDamage), moveSpeed(moveSpeed) {}

int Enemy::getAttackDamage() const {
    return attackDamage;
}

void Enemy::Update(Board& board) {
    if (!isAlive()) return;

    // Trouver le bâtiment le plus proche
    Building* target = FindClosestBuilding(board);

    if (target) {
        // Vérifier si l'ennemi est adjacent au bâtiment (pour l'attaquer)
        bool canAttack = false;
        Position targetPos = target->getPosition();

        // Calculer la distance au centre du bâtiment
        int dx = abs(position.X - targetPos.X);
        int dy = abs(position.Y - targetPos.Y);

        // Si l'ennemi est suffisamment proche pour attaquer
        if (dx <= (target->getSizeX() / 2 + 1) && dy <= (target->getSizeY() / 2 + 1)) {
            AttackBuilding(target);
        } else {
            // Sinon, se déplacer vers le bâtiment
            MoveTowards(board, targetPos);
        }
    }
}

void Enemy::AttackBuilding(Building* building) {
    if (building && !building->isDestroyed()) {
        building->takeDamage(attackDamage);
    }
}

Building* Enemy::FindClosestBuilding(Board& board) {
    Building* closest = nullptr;
    int minDistance = INT_MAX;

    for (Building* building : board.getBuildings()) {
        if (building->isDestroyed()) continue;

        // Priorité à l'Hôtel de Ville
        TownHall* townHall = dynamic_cast<TownHall*>(building);
        if (townHall) {
            return townHall;
        }

        Position buildingPos = building->getPosition();
        int dx = position.X - buildingPos.X;
        int dy = position.Y - buildingPos.Y;
        int distance = dx * dx + dy * dy;  // Distance au carré

        if (distance < minDistance) {
            minDistance = distance;
            closest = building;
        }
    }

    return closest;
}

bool Enemy::MoveTowards(Board& board, const Position& target) {
    // Calculer la direction vers la cible
    int dx = 0;
    int dy = 0;

    if (position.X < target.X) dx = 1;
    else if (position.X > target.X) dx = -1;

    if (position.Y < target.Y) dy = 1;
    else if (position.Y > target.Y) dy = -1;

    // Essayer de se déplacer horizontalement ou verticalement
    Position newPos = position;

    // Essayer d'abord horizontalement
    if (dx != 0) {
        newPos.X += dx * moveSpeed;
        if (newPos.X > 0 && newPos.X < board.getSizeX() - 1) {
            // Vérifier s'il y a une collision avec un mur
            bool collision = false;
            for (Building* building : board.getBuildings()) {
                if ((building->getRepr(), "🧱") == 0 && building->collidesWith(newPos)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                position = newPos;
                return true;
            }
        }
    }

    // Si le mouvement horizontal a échoué, essayer verticalement
    newPos = position;
    if (dy != 0) {
        newPos.Y += dy * moveSpeed;
        if (newPos.Y > 0 && newPos.Y < board.getSizeY() - 1) {
            // Vérifier s'il y a une collision avec un mur
            bool collision = false;
            for (Building* building : board.getBuildings()) {
                if ((building->getRepr(), "🧱") == 0 && building->collidesWith(newPos)) {
                    collision = true;
                    break;
                }
            }

            if (!collision) {
                position = newPos;
                return true;
            }
        }
    }

    return false;
}
