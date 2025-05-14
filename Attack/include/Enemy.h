#ifndef ENEMY_H
#define ENEMY_H
#include "Building.h"
#include "Npc.h"

class Board;

class Enemy : public Npc {
protected:
    int attackDamage;
    int moveSpeed;

public:
    Enemy();
    Enemy(Position position, std::wstring repr, int health, int attackDamage, int moveSpeed);
    virtual ~Enemy() = default;

    int getAttackDamage() const;

    virtual void Update(Board& board);
    void AttackBuilding(Building* building);

private:
    Building* FindClosestBuilding(Board& board);
    bool MoveTowards(Board& board, const Position& target);
};

#endif // ENEMY_H
