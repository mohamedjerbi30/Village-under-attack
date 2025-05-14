#include "Player.h"
#include "Board.h"
#include "Building.h"
#include "ResourceGenerator.h"
#include "GoldMine.h"
#include "ElixirCollector.h"

Player::Player() : Entity(Position(0, 0), "👤"), resources() {}

Player::Player(Position position) : Entity(position, "👤"), resources() {}

Resources& Player::getResources() {
    return resources;
}

bool Player::moving(Board& board, int deltaX, int deltaY) {
    Position newPosition(position.X + deltaX, position.Y + deltaY);

    // Vérifier les limites du plateau
    if (newPosition.X <= 0 || newPosition.X >= board.getSizeX() - 1 ||
        newPosition.Y <= 0 || newPosition.Y >= board.getSizeY() - 1) {
        return false;
    }

    // Vérifier s'il y a une collision avec un bâtiment
    for (Building* building : board.getBuildings()) {
        // Check if building is a wall (we can't walk through walls)
        if (building->getRepr() == "🧱" && building->collidesWith(newPosition)) {
            return false;
        }
    }

    // Si pas de collision, déplacer le joueur
    position = newPosition;
    return true;
}

void Player::collectResources(Board& board) {
    for (Building* building : board.getBuildings()) {
        if (building->collidesWith(position)) {
            // Si c'est une mine d'or
            GoldMine* goldMine = dynamic_cast<GoldMine*>(building);
            if (goldMine != nullptr && !goldMine->isEmpty()) {
                resources.addGold(goldMine->collect());
                return;
            }

            // Si c'est un collecteur d'élixir
            ElixirCollector* elixirCollector = dynamic_cast<ElixirCollector*>(building);
            if (elixirCollector != nullptr && !elixirCollector->isEmpty()) {
                resources.addElixir(elixirCollector->collect());
                return;
            }
        }
    }
}
