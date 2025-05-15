#include "Board.h"
#include "Position.h"
#include "Player.h"
#include <algorithm>

Board::Board(int sizeX, int sizeY)
    : SizeX(sizeX), SizeY(sizeY), townHall(nullptr), player(nullptr) {}

Board::~Board() {
    for (auto& b : buildings) delete b;
    for (auto& e : entities) delete e;
}

bool Board::IsOutOfBounds(Position pos) const {
    return pos.X < 0 || pos.X >= SizeX || pos.Y < 0 || pos.Y >= SizeY;
}

int Board::CountBuildingType(const Building& targetBuilding) const {
    int count = 0;
    for (const Building* b : buildings) {
        if (typeid(*b) == typeid(targetBuilding)) {
            count++;
        }
    }
    return count;
}

/*bool Board::CollidesWith(const Building& newBuilding) const {
    for (const Building* b : buildings) {
        if (b->collidesWith(newBuilding.getPosition())) {
            return true;
        }
    }
    return false;
}*/
bool Board::CollidesWith(const Building& newBuilding) const {
    int newLeft = newBuilding.getPosition().X - newBuilding.getSizeX() / 2;
    int newRight = newBuilding.getPosition().X + newBuilding.getSizeX() / 2;
    int newTop = newBuilding.getPosition().Y - newBuilding.getSizeY() / 2;
    int newBottom = newBuilding.getPosition().Y + newBuilding.getSizeY() / 2;

    for (const Building* b : buildings) {
        int bLeft = b->getPosition().X - b->getSizeX() / 2;
        int bRight = b->getPosition().X + b->getSizeX() / 2;
        int bTop = b->getPosition().Y - b->getSizeY() / 2;
        int bBottom = b->getPosition().Y + b->getSizeY() / 2;

        if (!(newRight < bLeft || newLeft > bRight || newBottom < bTop || newTop > bBottom)) {
            return true;
        }
    }
    return false;
}


bool Board::AddBuilding(Building* newBuilding) {
    // Vérifie les conditions d’échec
    if (IsOutOfBounds(newBuilding->getPosition())) return false;
    if (CollidesWith(*newBuilding)) return false;
    if (player && !player->getResources().canAfford(newBuilding->getCost())) return false;

    if (newBuilding->getMaxInstances() <= CountBuildingType(*newBuilding)) {
        delete newBuilding;
        return false;
    }

    // Consomme les ressources du joueur
    if (player) {
        player->getResources().consume(newBuilding->getCost());
    }

    // Ajoute le bâtiment à la liste
    buildings.push_back(newBuilding);

    // Si c’est un hôtel de ville, on le mémorise
    if (auto* th = dynamic_cast<TownHall*>(newBuilding)) {
        townHall = th;
    }

    return true;  // Succès
}


 bool Board::isPositionValid(const Position& pos) const {

        bool inBounds = (pos.X >= 0 && pos.X < SizeX &&
                        pos.Y >= 0 && pos.Y < SizeY);


        bool collision = false;
        for (const Building* b : buildings) {
            if (b->collidesWith(pos)) {
                collision = true;
                break;
            }
        }

        return inBounds && !collision;
    }
void Board::Draw() const {
    // Bordures horizontales
    std::cout << "+";
    for (int x = 0; x < SizeX; x++) std::cout << "--";
    std::cout << "+\n";

    for (int y = 0; y < SizeY; y++) {
        std::cout << "|"; // Bordure verticale gauche
        for (int x = 0; x < SizeX; x++) {
            bool hasBuilding = false;
            for (const auto& b : buildings) {
                if (b->collidesWith({x, y})) {
                    std::wcout << b->getRepr() << " ";
                    hasBuilding = true;
                    break;
                }
            }
            if (!hasBuilding) std::cout << "  ";
        }
        std::cout << "|\n"; // Bordure verticale droite
    }

    // Bordures horizontales
    std::cout << "+";
    for (int x = 0; x < SizeX; x++) std::cout << "--";
    std::cout << "+\n";
}
//ajouté
std::vector<Building*>& Board::getBuildings() {
    return buildings;
}
//ajouté


