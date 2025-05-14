#ifndef BARRACK_H
#define BARRACK_H

#include "Building.h"
#include "Troop.h"
#include <queue>
#include <vector>
#include "Board.h"

class Barrack : public Building {
private:
    std::vector<Troop*> entities;
    std::queue<Troop*> trainingQueue;
    int capacity;

public:
    Barrack();
    Barrack(Position position);
    virtual ~Barrack();

    int getCapacity() const;
    const std::vector<Troop*>& getEntities() const;

    bool Train(Troop* troop);
    virtual void Update(Board& board) ;
};

#endif // BARRACK_H
