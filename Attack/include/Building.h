#pragma once
#include "Position.h"
#include "Resources.h"
#include <iostream>
class Board;
class Building {
protected:
    Position p;
    int sizeX;
    int sizeY;
    const char* repr;
    Resources cost;
    int maxInstances;
    int health;

public:
    Building(Position position, int sX, int sY, const char* r, Resources c, int max, int hp);
    virtual ~Building() = default;

    bool collidesWith(Position position) const;
    bool collidesWith(const Building& other) const;
    virtual void update();

    Position getPosition() const { return p; }
    int getSizeX() const { return sizeX; }
    int getSizeY() const { return sizeY; }
    //modifié
     const char* getRepr() const { return repr; }
    int getHealth() const { return health; }
    void loseHealth(int amount);
    Resources getCost() const {return cost;}
    int getMaxInstances() const {return maxInstances;}
    bool isAlive() const;
};
