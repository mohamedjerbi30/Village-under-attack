#ifndef NPC_H
#define NPC_H
#include "Board.h"
#include "Position.h"

#include <Entity.h>


class Npc : public Entity
{
    protected:
    int health;
    public:
        void setPosition(Position newPosition) {
        this->position = newPosition;
                }
        Npc(Position position, int initialHealth = 100, const char* emoji = "👤");
        void loseHealth(int amount);
        void moveTowards(const Board& board, Position destination);
        virtual void Update(const Board& board);
        virtual ~Npc();
        int getHealth() const { return health; }
};

#endif // NPC_H
