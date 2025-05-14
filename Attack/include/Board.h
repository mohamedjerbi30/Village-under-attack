#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "Position.h"

class Building;
class Player;
class Enemy;

class Board {
private:
    int sizeX;
    int sizeY;
    std::vector<Building*> buildings;
    std::vector<Enemy*> enemies;
    Player* player;

public:
    Board();
    Board(int sizeX, int sizeY);
    ~Board();

    int getSizeX() const;
    int getSizeY() const;

    const std::vector<Building*>& getBuildings() const;
    const std::vector<Enemy*>& getEnemies() const;
    Player* getPlayer() const;

    void setPlayer(Player* player);

    bool AddBuilding(Building* building);
    void AddEnemy(Enemy* enemy);

    void RemoveDestroyedBuildings();
    void RemoveDeadEnemies();

    bool isGameOver() const;

    void Update();
};

#endif // BOARD_H
