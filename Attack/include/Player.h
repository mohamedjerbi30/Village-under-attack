#ifndef PLAYER_H
#define PLAYER_H

#include "Resources.h"
#include "Board.h"
#include <Entity.h>
#include "Raider.h"

class Building;
class Barrack;      // ✅ déclaration anticipée
class Troop;        // ✅ utile si tu manipules Troop* dans Player

class Player : public Entity
{
private:
    Resources resources;
    bool isBuildingMode = false;
public:

    Player(Position position);
    void ToggleBuildMode() { isBuildingMode = !isBuildingMode; }
    bool IsBuilding() const { return isBuildingMode; }

    Building* Interact(Board& board);

    Resources& getResources();
    void Update(const Board&) override;
    const char* getRepr() const override { return "🧑"; }
    virtual ~Player();
    using Entity::moving;
    void moving(const Board& board, const std::vector<Raider*>& raiders, int dx, int dy);

    void trainTroop(Barrack& barrack, Troop* type); // ✅ plus d'erreur ici
    void takeDamage(int) override; // ✅ override correct aussi

};

#endif // PLAYER_H
