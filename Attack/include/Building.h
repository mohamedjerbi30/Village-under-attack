#ifndef BUILDING_H
#define BUILDING_H

#include "Position.h"
#include "Resources.h"
#include <string>

class Building {
protected:
    Position position;
    int sizeX;
    int sizeY;
    std::wstring repr;
    Resources cost;
    int maxInstances;
    int health;

public:
    Building();
    Building(Position position, int sizeX, int sizeY, std::wstring repr,
             int goldCost, int elixirCost, int maxInstances, int health);
    virtual ~Building() = default;

    Position getPosition() const;
    int getSizeX() const;
    int getSizeY() const;
    std::wstring getRepr() const;
    Resources getCost() const;
    int getMaxInstances() const;
    int getHealth() const;

    void takeDamage(int damage);
    bool isDestroyed() const;

    bool collidesWith(const Position& pos) const;
    bool collidesWith(const Building& other) const;

    virtual void Update() {};
};

#endif // BUILDING_H
