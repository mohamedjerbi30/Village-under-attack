#ifndef TROOP_H
#define TROOP_H

#include "Npc.h"
#include "Position.h"
#include "Resources.h"
#include <vector>

class Board;
class Enemy;

class Troop : public Npc {
protected:
    Resources cost;
    int attackDamage;
    int attackRange;
    Enemy* targetedEnemy;

public:
    Troop();
    Troop(Position position, const char* repr, int health, int attackDamage, int attackRange, int goldCost, int elixirCost);
    virtual ~Troop() = default;

    Resources getCost() const;
    int getAttackDamage() const;
    int getAttackRange() const;
    Enemy* getTargetedEnemy() const;

    void setTargetedEnemy(Enemy* enemy);

    virtual void Update(Board& board);
    virtual void Attack(Enemy* enemy);
    virtual void MoveOrAttack(Board& board);

    bool AnyTroopNearTarget(const std::vector<Npc*>& entities);
    Enemy* GetNearestEnemy(const std::vector<Enemy*>& entities);
    void TargetEnemy(const std::vector<Enemy*>& entities);
};

#endif // TROOP_H
