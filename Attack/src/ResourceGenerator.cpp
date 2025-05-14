#include "ResourceGenerator.h"

ResourceGenerator::ResourceGenerator()
    : Building(), current(0), capacity(100), productionRate(5),
      emptyRepr(""), fullRepr("") {}

ResourceGenerator::ResourceGenerator(Position position, int sizeX, int sizeY,
                                    const char* emptyRepr, const char* fullRepr,
                                    int goldCost, int elixirCost, int maxInstances,
                                    int health, int capacity, int productionRate)
    : Building(position, sizeX, sizeY, emptyRepr, goldCost, elixirCost, maxInstances, health),
      current(0), capacity(capacity), productionRate(productionRate),
      emptyRepr(emptyRepr), fullRepr(fullRepr) {}

bool ResourceGenerator::isFull() const {
    return current >= capacity;
}

bool ResourceGenerator::isEmpty() const {
    return current <= 0;
}

int ResourceGenerator::getCurrent() const {
    return current;
}

int ResourceGenerator::getCapacity() const {
    return capacity;
}

int ResourceGenerator::collect() {
    int collected = current;
    current = 0;
    repr = emptyRepr;  // Set visual representation to empty
    return collected;
}

void ResourceGenerator::Update() {
    if (!isDestroyed() && current < capacity) {
        current += productionRate;
        if (current > capacity) {
            current = capacity;
        }

        // Update visual representation based on fullness
        if (isFull()) {
            repr = fullRepr;
        } else {
            repr = emptyRepr;
        }
    }
}
