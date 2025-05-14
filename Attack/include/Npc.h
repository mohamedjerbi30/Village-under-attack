#ifndef NPC_H
#define NPC_H

#include "Entity.h"

class Npc : public Entity {
protected:
    int health;

public:
    Npc();
    Npc(Position position, std::wstring repr, int health);
    virtual ~Npc() = default;

    int getHealth() const;
    void takeDamage(int damage);
    bool isAlive() const;

    virtual void Update() override {};
};

#endif // NPC_H
