#include "Board.h"
#include "Building.h"
#include "Player.h"
#include "Enemy.h"
#include <algorithm>

Board::Board() : sizeX(50), sizeY(20), buildings(), enemies(), player(nullptr) {}

Board::Board(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY), buildings(), enemies(), player(nullptr) {}

Board::~Board() {
    // Clean up all buildings
    for (Building* building : buildings) {
        delete building;
    }
    buildings.clear();

    // Clean up all enemies
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // Note: Do not delete player as it's managed externally
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
    if (!building) return false;

    // Check if building is within board boundaries
    Position pos = building->getPosition();
    int bSizeX = building->getSizeX();
    int bSizeY = building->getSizeY();

    if (pos.X <= 0 || pos.Y <= 0 ||
        pos.X + bSizeX >= sizeX || pos.Y + bSizeY >= sizeY) {
        return false;
    }

    // Check for collisions with other buildings
    for (Building* existingBuilding : buildings) {
        if (building->collidesWith(*existingBuilding)) {
            return false;
        }
    }

    // Count instances of this building type to check against max allowed
    int instanceCount = 0;
    for (Building* existingBuilding : buildings) {
        if (strcmp(existingBuilding->getRepr(), building->getRepr()) == 0) {
            instanceCount++;
        }
    }

    if (instanceCount >= building->getMaxInstances()) {
        return false;
    }

    // All checks passed, add the building
    buildings.push_back(building);
    return true;
}

void Board::AddEnemy(Enemy* enemy) {
    if (enemy) {
        enemies.push_back(enemy);
    }
}

void Board::RemoveDestroyedBuildings() {
    auto it = std::remove_if(buildings.begin(), buildings.end(),
        [](Building* building) {
            if (building->isDestroyed()) {
                delete building;
                return true;
            }
            return false;
        });
    buildings.erase(it, buildings.end());
}

void Board::RemoveDeadEnemies() {
    auto it = std::remove_if(enemies.begin(), enemies.end(),
        [](Enemy* enemy) {
            if (!enemy->isAlive()) {
                delete enemy;
                return true;
            }
            return false;
        });
    enemies.erase(it, enemies.end());
}

bool Board::isGameOver() const {
    // Check if Town Hall is destroyed
    for (Building* building : buildings) {
        if (strcmp(building->getRepr(), "🏛") == 0) {
            return building->isDestroyed();
        }
    }

    // If there's no Town Hall, the game is over
    return true;
}

void Board::Update() {
    // Update all enemies
    for (Enemy* enemy : enemies) {
        enemy->Update(*this);
    }

    // Clean up destroyed buildings and dead enemies
    RemoveDestroyedBuildings();
    RemoveDeadEnemies();
}
