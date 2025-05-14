#include "../include/Board.h"
#include "../include/Building.h"
#include "../include/Enemy.h"
#include "../include/Player.h"
#include "../include/TownHall.h"

Board::Board() : sizeX(50), sizeY(20), player(nullptr) {
}

Board::Board(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY), player(nullptr) {
}

Board::~Board() {
    // Clean up buildings
    for (Building* building : buildings) {
        delete building;
    }
    buildings.clear();

    // Clean up enemies
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // Player is deleted in main.cpp
}

int Board::getSizeX() const {
    return sizeX;
}

int Board::getSizeY() const {
    return sizeY;
}

const std::vector<Building*>& Board::getBuildings() const {
    return buildings;
}

const std::vector<Enemy*>& Board::getEnemies() const {
    return enemies;
}

Player* Board::getPlayer() const {
    return player;
}

void Board::setPlayer(Player* player) {
    this->player = player;
}

bool Board::AddBuilding(Building* building) {
    // Check if the building is within bounds
    Position pos = building->getPosition();
    int buildingSizeX = building->getSizeX();
    int buildingSizeY = building->getSizeY();

    if (pos.X < 1 || pos.Y < 1 || pos.X + buildingSizeX > sizeX - 1 || pos.Y + buildingSizeY > sizeY - 1) {
        return false;
    }

    // Check for collision with other buildings
    for (Building* existingBuilding : buildings) {
        if (existingBuilding->isDestroyed()) continue;

        Position existingPos = existingBuilding->getPosition();
        int existingSizeX = existingBuilding->getSizeX();
        int existingSizeY = existingBuilding->getSizeY();

        // Check if the new building's area overlaps with an existing building's area
        if (pos.X < existingPos.X + existingSizeX &&
            pos.X + buildingSizeX > existingPos.X &&
            pos.Y < existingPos.Y + existingSizeY &&
            pos.Y + buildingSizeY > existingPos.Y) {
            return false;
        }
    }

    buildings.push_back(building);
    return true;
}

void Board::AddEnemy(Enemy* enemy) {
    enemies.push_back(enemy);
}

void Board::RemoveDestroyedBuildings() {
    auto it = buildings.begin();
    while (it != buildings.end()) {
        if ((*it)->isDestroyed()) {
            delete *it;
            it = buildings.erase(it);
        } else {
            ++it;
        }
    }
}

void Board::RemoveDeadEnemies() {
    auto it = enemies.begin();
    while (it != enemies.end()) {
        if (!(*it)->isAlive()) {
            delete *it;
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }
}

bool Board::isGameOver() const {
    // Game is over if there's no town hall
    bool hasTownHall = false;
    for (Building* building : buildings) {
        TownHall* townHall = dynamic_cast<TownHall*>(building);
        if (townHall && !townHall->isDestroyed()) {
            hasTownHall = true;
            break;
        }
    }
    return !hasTownHall;
}

void Board::Update() {
    // Update all enemies
    for (Enemy* enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->Update(*this);
        }
    }

    // Remove dead enemies
    RemoveDeadEnemies();

    // Remove destroyed buildings
    RemoveDestroyedBuildings();
}
