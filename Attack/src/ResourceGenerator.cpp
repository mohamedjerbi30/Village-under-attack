#include "ResourceGenerator.h"

ResourceGenerator::ResourceGenerator()
    : Building(), current(0), capacity(100), productionRate(1),
      emptyRepr(L""), fullRepr(L"") {}

ResourceGenerator::ResourceGenerator(Position position, int sizeX, int sizeY,
                                     std::wstring emptyRepr, std::wstring fullRepr,
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
    repr = emptyRepr; // Mettre à jour la représentation
    return collected;
}

void ResourceGenerator::Update() {
    // Générer des ressources au fil du temps
    if (current < capacity) {
        current += productionRate;
        if (current > capacity) {
            current = capacity;
        }

        // Mettre à jour la représentation en fonction de l'état
        if (isFull()) {
            repr = fullRepr;
        } else {
            repr = emptyRepr;
        }
    }
}
