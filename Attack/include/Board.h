#pragma once
#ifndef BOARD_H
#define BOARD_H
#include "Position.h"
#include "Building.h"
#include "TownHall.h"
#include "Entity.h"
#include <vector>
class Player;
class Board {
private:
    bool raidersSpawned = false; //ajout�
    int SizeX;
    int SizeY;
    std::vector<Building*> buildings;
    std::vector<Entity*> entities;
    TownHall* townHall;
    Player* player;


public:
    std::vector<Building*>& getBuildings();
    void PlaceTownHall() {
        Position center(SizeX/2, SizeY/2);
        if (!townHall) {
            townHall = new TownHall(center);
            AddBuilding(townHall);
        }
    }
    Board(int sizeX, int sizeY);

    // V�rifie si une position est hors limites
    bool IsOutOfBounds(Position pos) const;

    // Compte les b�timents du m�me type
    int CountBuildingType(const Building& targetBuilding) const;

    // V�rifie les collisions avec d'autres b�timents
    bool CollidesWith(const Building& newBuilding) const;

    // Ajoute un b�timent en respectant les contraintes
    bool AddBuilding(Building* newBuilding);

    // Getters
    int getSizeX() const { return SizeX; }
    int getSizeY() const { return SizeY; }
    const std::vector<Building*>& getBuildings() const { return buildings; }
    Player* getPlayer() const { return player; }
    TownHall* getTownHall() const { return townHall; }

    // Setters (optionnels)
    void setPlayer(Player* p) { player = p; }
    void setTownHall(TownHall* th) { townHall = th; }
    bool isPositionValid(const Position&) const;
    void Draw() const;
    virtual ~Board();
    //ajout�
    void AddEntity(Entity* entity) {
    // Exemple : tu peux stocker l'entit� dans un conteneur
    entities.push_back(entity); // Assure-toi que entities existe !
    }
     const std::vector<Entity*>& getEntities() const {
        return entities;
    }
};


#endif // BOARD_H
