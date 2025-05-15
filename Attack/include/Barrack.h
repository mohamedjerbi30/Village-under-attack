#ifndef BARRACK_H
#define BARRACK_H

#include "Building.h"
#include "Troop.h"
#include <queue>

class Barrack : public Building {
private:
    std::queue<Troop*> trainingQueue;
    int capacity;

public:
    Barrack(Position position);

    bool Train(Troop* troop);
    void Update(Board& board) ;
    const char* getRepr() {return "🪖";};
};

#endif
