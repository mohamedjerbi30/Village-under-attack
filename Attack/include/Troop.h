#ifndef TROOP_H
#define TROOP_H

#include "Entity.h"
#include "Enemy.h"
#include "Position.h"
#include <vector>

class Board;
class Npc;

class Troop : public Entity {
public:
    int Cost;
    int AttackDamage;
    Enemy* TargetedEnemy;
    int Health;

    // Constructeur protégé
    Troop(Position position, int cost, int attackDamage, int health);

public:
    virtual ~Troop() = default;

    void Attack(Enemy* enemy);
    void MoveOrAttack(Board& board);
    bool AnyTroopNearerTarget(const std::vector<Npc>& entities) const;
    Enemy* GetNearestEnemy(const std::vector<Npc>& entities) const;
    void TargetEnemy(const std::vector<Npc>& entities);
    void Update(const Board& board) ;
    void takeDamage(int damage);
    void SetPosition( Position& newPos);
    int getAttackRange() const { return AttackDamage; }
};

#endif // TROOP_H
