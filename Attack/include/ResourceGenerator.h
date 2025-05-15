#ifndef RESOURCEGENERATOR_H
#define RESOURCEGENERATOR_H
#include "Board.h"
#include <Building.h>


class ResourceGenerator : public Building
{
    protected:
    int current;
    int capacity;
    bool isFull;

    public:
        ResourceGenerator(Position p, int sX, int sY, const char* r, Resources cost, int maxInst, int hp);
        virtual ~ResourceGenerator();
        void update(const Board& board) ;
        virtual const char* getRepr() const = 0;
};

#endif // RESOURCEGENERATOR_H
