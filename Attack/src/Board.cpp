#include "Board.h"
#include "Building.h"
#include "TownHall.h"
#include "Player.h"
#include "Enemy.h"
#include <algorithm>

Board::Board() : sizeX(50), sizeY(20), player(nullptr) {}

Board::Board(int sizeX, int sizeY) : sizeX(sizeX), sizeY(sizeY), player(nullptr) {}

Board::~Board() {
    // Nettoyer les b�timents
    for (Building* building : buildings) {
        delete building;
    }
    buildings.clear();

    // Nettoyer les ennemis
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();

    // Ne pas supprimer le joueur car il est suppos� �tre g�r� ailleurs
    player = nullptr;
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

    // V�rifier si le b�timent est dans les limites du plateau
    Position pos = building->getPosition();
    int halfSizeX = building->getSizeX() / 2;
    int halfSizeY = building->getSizeY() / 2;

    if (pos.X - halfSizeX <= 0 || pos.X + halfSizeX >= sizeX - 1 ||
        pos.Y - halfSizeY <= 0 || pos.Y + halfSizeY >= sizeY - 1) {
        return false;
    }

    // V�rifier les collisions avec les b�timents existants
    for (Building* existingBuilding : buildings) {
        if (building->collidesWith(*existingBuilding)) {
            return false;
        }
    }

    // Compter le nombre d'instances du m�me type de b�timent
    int count = 0;
    for (Building* existingBuilding : buildings) {
        if (typeid(*existingBuilding) == typeid(*building)) {
            count++;
        }
    }

    // V�rifier si le nombre maximum d'instances est atteint
    if (count >= building->getMaxInstances()) {
        return false;
    }

    // Ajouter le b�timent
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
        [](Building* b) {
            if (b->isDestroyed()) {
                delete b;
                return true;
            }
            return false;
        });

    buildings.erase(it, buildings.end());
}

void Board::RemoveDeadEnemies() {
    auto it = std::remove_if(enemies.begin(), enemies.end(),
        [](Enemy* e) {
            if (!e->isAlive()) {
                delete e;
                return true;
            }
            return false;
        });

    enemies.erase(it, enemies.end());
}

bool Board::isGameOver() const {
    // Le jeu est termin� si l'h�tel de ville est d�truit
    for (Building* building : buildings) {
        if (dynamic_cast<TownHall*>(building)) {
            return false; // L'h�tel de ville existe encore
        }
    }
    return true; // Pas d'h�tel de ville, le jeu est termin�
}

void Board::Update() {
    // Mettre � jour tous les b�timents
    for (Building* building : buildings) {
        building->Update();
    }

    // Mettre � jour tous les ennemis
    for (Enemy* enemy : enemies) {
        enemy->Update(*this);
    }

    // Supprimer les b�timents d�truits et les ennemis morts
    RemoveDestroyedBuildings();
    RemoveDeadEnemies();
}
