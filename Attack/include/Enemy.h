#ifndef ENEMY_H
#define ENEMY_H
#include "Board.h"
#include <Npc.h>
#include <cmath>


class Enemy : public Npc
{
    private:
    bool Targeted;
    int AttackDamage;
    /*
    DWORD lastMoveTime = 0;
    DWORD moveDelay = 800;
    */

    public:
        void AttackBuilding(const Board& board);
        void Update( Board& board) ;
        virtual ~Enemy();
        void takeDamage(int damage);
        //ajout�


        void moveTowards(Position target, const Board& board) {
            Position current = this->getPosition();
            Position next = current;

            // Priorit� � l'axe X
            if (target.X > current.X) next.X++;
            else if (target.X < current.X) next.X--;
            else if (target.Y > current.Y) next.Y++;
            else if (target.Y < current.Y) next.Y--;

            // V�rifie que la nouvelle position est valide
            if (!board.IsOutOfBounds(next)) {
                this->setPosition(next);
            }
        }
        //ajout�


       // void setMoveDelay(DWORD delay);//
        bool IsDestroyed() const {
        return health <= 0;  // V�rifie si la sant� est inf�rieure ou �gale � 0
    }


    protected:

        Enemy(Position position, int attackDamage = 10);

};

#endif // ENEMY_H
