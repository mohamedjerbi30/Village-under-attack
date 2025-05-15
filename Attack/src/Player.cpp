#include "Player.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "Wall.h"
#include "Building.h"
#include "Raider.h"
#include "Troop.h"
#include "Barrack.h"

Player::Player(Position position)
    : Entity(position, "🧑"), resources(400, 400) {}


Building* Player::Interact(Board& board) {

    for (Building* b : board.getBuildings()) {
        if (b->collidesWith(position)) {
            if (auto* mine = dynamic_cast<GoldMine*>(b)) {
                resources.addGold(mine->Collect().getGold());
            }
            else if (auto* collector = dynamic_cast<ElixirCollector*>(b)) {
                resources.addElixir(collector->Collect().getElixir());
            }
        }
    }


    if (resources.getGold() >= 10) {
        Wall* newWall = new Wall(position);
        if (board.AddBuilding(newWall)) {
            resources.spendGold(10);
            return newWall;
        }
        delete newWall;
    }

    return nullptr;
}


Resources& Player::getResources() {
    return resources;
}
Player::~Player()
{
    //dtor
}
void Player::Update(const Board& board) {
    // Exemple 1 : Collecte automatique des ressources
    for (Building* building : board.getBuildings()) {
        if (building->collidesWith(position)) {
            if (auto* mine = dynamic_cast<GoldMine*>(building)) {
                resources.addGold(mine->Collect().getGold());
            }
            else if (auto* collector = dynamic_cast<ElixirCollector*>(building)) {
                resources.addElixir(collector->Collect().getElixir());
            }
        }
    }

    // Exemple 2 : Vérification de la victoire/défaite
    if (board.getTownHall()->getHealth() <= 0) {
        std::cout << "Game Over : Town Hall destroyed!\n";
    }
}
//ajouté
void Player::moving(const Board& board, const std::vector<Raider*>& raiders, int dx, int dy) {
    Position newPos = position + Position(dx, dy);

    // Empêche de sortir de la carte
    if (newPos.X <= 0 || newPos.Y <= 0 ||
        newPos.X >= board.getSizeX() - 1 ||
        newPos.Y >= board.getSizeY() - 1) {
        return;
    }

    // Empêche de traverser les bâtiments
    for (Building* b : board.getBuildings()) {
        if (b->collidesWith(newPos)) {
            return;
        }
    }

    // Empêche de marcher sur un raider
    for (Raider* r : raiders) {
        if (r->getPosition() == newPos) {
            return;
        }
    }

    position = newPos;
}

//ajouté
/*void Player::moving(const Board& board, int dx, int dy) {
    Position newPos = position + Position(dx, dy);

    if (newPos.X <= 0 || newPos.Y <= 0 ||
        newPos.X >= board.getSizeX() - 1 ||
        newPos.Y >= board.getSizeY() - 1) {
        return;
    }

    for (Building* b : board.getBuildings()) {
        if (b->collidesWith(newPos) && !b->collidesWith(position)) {
            return;
        }
    }

    position = newPos;
}*/
void Player::trainTroop(Barrack& barrack, Troop* type) {
    if (resources.getElixir() >= type->Cost) {
        barrack.Train(type);
    }
}

void Player::takeDamage(int) {
    // Le joueur ne prend pas de dégâts
}


