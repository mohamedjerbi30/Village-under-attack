#include "Barbarian.h"
#include "Enemy.h"

Barbarian::Barbarian(Position pos)
    : Troop(pos, 15, 1, 3), AttackRange(1) {}  // cost, damage, health

void Barbarian::attack(Entity* target) {
    if (position.manhattanDistance(target->getPosition()) <= AttackRange) {
        target->takeDamage(AttackDamage);

        // Effet de rage : +2% de dégâts par attaque successive
        AttackDamage = static_cast<int>(AttackDamage * 1.02);
    }
}

void Barbarian::specialAbility() {
    // Boost de rage pendant 5 secondes
    AttackDamage *= 2;

    // Si tu veux vraiment utiliser attackSpeed, tu dois le déclarer
    // AttackSpeed *= 2; // Déclarer cette variable dans Troop si nécessaire
    // Réinitialisation à gérer dans le système de jeu (timer)
}

int Barbarian::GetAttackRange() const {
    return AttackRange;
}
