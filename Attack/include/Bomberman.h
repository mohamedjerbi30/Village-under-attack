#ifndef BOMBERMAN_H
#define BOMBERMAN_H

#include <Enemy.h>


class Bomberman : public Enemy
{
    private:
        int explosionRange;

    public:
        Bomberman(Position pos);
        virtual ~Bomberman();
        void attackBuilding(Building* target) ;
        void update(Board& board) ;
        int getExplosionRange() const;
        const char* getRepr() const {return "💣";}

};

#endif // BOMBERMAN_H
