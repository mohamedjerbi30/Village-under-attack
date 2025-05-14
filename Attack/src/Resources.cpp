#include "Resources.h"

Resources::Resources() : gold(400), elixir(400) {}

Resources::Resources(int gold, int elixir) : gold(gold), elixir(elixir) {}

int Resources::getGold() const {
    return gold;
}

int Resources::getElixir() const {
    return elixir;
}

void Resources::addGold(int amount) {
    if (amount > 0) {
        gold += amount;
    }
}

void Resources::addElixir(int amount) {
    if (amount > 0) {
        elixir += amount;
    }
}

bool Resources::spendGold(int amount) {
    if (amount <= 0) return false;

    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

bool Resources::spendElixir(int amount) {
    if (amount <= 0) return false;

    if (elixir >= amount) {
        elixir -= amount;
        return true;
    }
    return false;
}

bool Resources::canAfford(int goldCost, int elixirCost) const {
    return gold >= goldCost && elixir >= elixirCost;
}
