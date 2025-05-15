#include "Enemy.h"
#include "Building.h"

Enemy::Enemy(Position position, int attackDamage)
    : Npc(position, 100, "👹"), Targeted(false), AttackDamage(attackDamage) {}

/*void Enemy::AttackBuilding(const Board& board) {
    for (Building* building : board.getBuildings()) {
        if (building->collidesWith(getPosition())) {
            building->loseHealth(AttackDamage);
        }
    }
}*/
//ajouté
void Enemy::AttackBuilding(const Board& board) {
    for (Building* building : board.getBuildings()) {
        if (building->collidesWith(getPosition())) {
            building->loseHealth(AttackDamage);
            break; // attaque un seul bâtiment à la fois
        }
    }
}
//ajouté


void Enemy::Update( Board& board) {
    TownHall* th = board.getTownHall();
    Position pos = th->getPosition();
    moveTowards(pos,board);
    AttackBuilding(board);}
/*
void Enemy::setMoveDelay(DWORD delay) {
    moveDelay = delay;
}
/*/

Enemy::~Enemy()
{
    //dtor
}
void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
}
