#ifndef RESOURCES_H
#define RESOURCES_H

class Resources {
private:
    int gold;
    int elixir;

public:
    Resources();
    Resources(int gold, int elixir);

    int getGold() const;
    int getElixir() const;

    void addGold(int amount);
    void addElixir(int amount);

    bool spendGold(int amount);
    bool spendElixir(int amount);

    bool canAfford(int goldCost, int elixirCost) const;
};

#endif // RESOURCES_H
