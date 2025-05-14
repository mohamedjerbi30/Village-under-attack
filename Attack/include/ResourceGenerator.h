#ifndef RESOURCE_GENERATOR_H
#define RESOURCE_GENERATOR_H

#include "Building.h"

class ResourceGenerator : public Building {
protected:
    int current;
    int capacity;
    int productionRate;
    std::wstring emptyRepr;
    std::wstring fullRepr;

public:
    ResourceGenerator();
    ResourceGenerator(Position position, int sizeX, int sizeY,
                       std::wstring emptyRepr, std::wstring fullRepr,
                       int goldCost, int elixirCost, int maxInstances,
                       int health, int capacity, int productionRate);

    bool isFull() const;
    bool isEmpty() const;
    int getCurrent() const;
    int getCapacity() const;

    virtual int collect();
    virtual void Update() override;
};

#endif // RESOURCE_GENERATOR_H
