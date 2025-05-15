#ifndef RESOURCES_H
#define RESOURCES_H

class Resources {
private:
    int gold;
    int elixir;
public:
    Resources(int initialGold =400, int initialElixir =400);
    void addGold(int amount);
    void addElixir(int amount);
    bool spendGold(int amount);
    bool spendElixir(int amount);
    int getGold() const;
    int getElixir() const;
    bool canAfford(const Resources& cost) const;
    void consume(const Resources& cost);

};

#endif // RESSOURCES_H


