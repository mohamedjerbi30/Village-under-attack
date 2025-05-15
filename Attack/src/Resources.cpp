#include "Resources.h"

Resources::Resources(int initialGold, int initialElixir)
    : gold(initialGold), elixir(initialElixir) {}

void Resources::addGold(int amount) { gold += amount; }
void Resources::addElixir(int amount) { elixir += amount; }

bool Resources::spendGold(int amount) {
    if (gold >= amount) {
        gold -= amount;
        return true;
    }
    return false;
}

bool Resources::spendElixir(int amount) {
    if (elixir >= amount) {
        elixir -= amount;
        return true;
    }
    return false;
}

int Resources::getGold() const
{
    return gold;
}
int Resources::getElixir() const
{
    return elixir;
}
bool Resources::canAfford(const Resources& cost) const {
    return gold >= cost.gold && elixir >= cost.elixir;
}

void Resources::consume(const Resources& cost) {
    gold -= cost.gold;
    elixir -= cost.elixir;
}

