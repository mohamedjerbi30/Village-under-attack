#include "Troop.h"
#include "Npc.h"
#include "Board.h"
#include <cmath>
#include <limits>

Troop::Troop(Position position, int cost, int attackDamage, int health)
    : Entity(position), Cost(cost), AttackDamage(attackDamage), Health(health), TargetedEnemy(nullptr) {}

void Troop::Attack(Enemy* enemy) {
    if (enemy) {
        enemy->takeDamage(AttackDamage);
        Health -= 3;
        if (Health <= 0) {
            // Gérer la suppression du troop si nécessaire
        }
    }
}
void Troop::SetPosition( Position& newPos) {
    position = newPos; // Met à jour la position de la troupe
}
void Troop::MoveOrAttack(Board& board) {
    if (!TargetedEnemy) return;

    Position enemyPos = TargetedEnemy->getPosition();
    Position currentPos = getPosition();

    int dx = enemyPos.X - currentPos.X;
    int dy = enemyPos.Y - currentPos.Y;
    double distance = sqrt(dx * dx + dy * dy);

    if (distance <= 1.0) {
        Attack(TargetedEnemy);
    } else {
        int stepX = (dx != 0) ? (dx > 0 ? 1 : -1) : 0;
        int stepY = (dy != 0) ? (dy > 0 ? 1 : -1) : 0;

        Position newPos(currentPos.X+ stepX, currentPos.Y + stepY);
        if (board.isPositionValid(newPos)) {
            SetPosition(newPos);
        }
    }
}

bool Troop::AnyTroopNearerTarget(const std::vector<Npc>& entities) const {
    if (!TargetedEnemy) return false;

    Position targetPos = TargetedEnemy->getPosition();  // Position de la cible
    // Utilisation de la distance de Manhattan
    int currentDistance = getPosition().manhattanDistance(targetPos);

    for (const Npc& npc : entities) {
        const Troop* troop = dynamic_cast<const Troop*>(&npc);
        if (troop && troop != this) {
            // Utilisation de la distance de Manhattan
            int d = troop->getPosition().manhattanDistance(targetPos);
            if (d < currentDistance) return true;  // Si une troupe est plus proche
        }
    }
    return false;  // Aucune troupe plus proche
}

Enemy* Troop::GetNearestEnemy(const std::vector<Npc>& entities) const {
    Enemy* nearest = nullptr;
    int minDist = std::numeric_limits<int>::max();  // Utiliser int au lieu de double

    for (const Npc& npc : entities) {
        Enemy* enemy = dynamic_cast<Enemy*>(const_cast<Npc*>(&npc));
        if (enemy) {
            // Utilisation de la distance de Manhattan
            int d = getPosition().manhattanDistance(enemy->getPosition());
            if (d < minDist) {
                minDist = d;
                nearest = enemy;
            }
        }
    }
    return nearest;
}


void Troop::TargetEnemy(const std::vector<Npc>& entities) {
    TargetedEnemy = GetNearestEnemy(entities);
}

void Troop::Update(const Board& board) {
    // Chercher l'ennemi le plus proche
    Enemy* nearestEnemy = nullptr;
    int minDistance = INT_MAX;

    for (Entity* entity : board.getEntities()) {
        Enemy* enemy = dynamic_cast<Enemy*>(entity);
        if (enemy) {
            int distance = position.manhattanDistance(enemy->getPosition());
            if (distance < minDistance) {
                minDistance = distance;
                nearestEnemy = enemy;
            }
        }
    }
}
void Troop::takeDamage(int damage) {
    Health -= damage;
    if (Health < 0) Health = 0;
}
