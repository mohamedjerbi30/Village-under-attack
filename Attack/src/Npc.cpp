#include "Npc.h"

Npc::Npc() : Entity(), health(100) {}

Npc::Npc(Position position, const char* repr, int health)
    : Entity(position, repr), health(health) {}

int Npc::getHealth() const {
    return health;
}

void Npc::takeDamage(int damage) {
    if (damage > 0) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }
}

bool Npc::isAlive() const {
    return health > 0;
}
