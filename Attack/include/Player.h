#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Resources.h"

class Board; // Forward declaration

class Player : public Entity {
private:
    Resources resources;

public:
    Player();
    Player(Position position);

    Resources& getResources();

    bool moving(Board& board, int deltaX, int deltaY);
    void collectResources(Board& board);
};

#endif // PLAYER_H
