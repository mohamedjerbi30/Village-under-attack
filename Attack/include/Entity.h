#ifndef ENTITY_H
#define ENTITY_H
#include "Position.h"

class Board;
class Entity
{
    protected:
        Position position;
        const char* repr;

    public:
        Entity(Position pos, const char* emoji = "👤");
        virtual ~Entity() = default;
        Position getPosition() const;
        virtual const char* getRepr() const;
        virtual void moving(const Board& board, int dx, int dy);
        virtual void Update(const Board&)=0;
        virtual void takeDamage(int damage) = 0;


};

#endif // ENTITY_H
