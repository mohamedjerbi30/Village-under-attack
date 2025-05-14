#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <memory>
#include <cctype>
#include <cstring>

// Include all header files
#include "Position.h"
#include "Resources.h"
#include "Entity.h"
#include "Npc.h"
#include "Building.h"
#include "ResourceGenerator.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "TownHall.h"
#include "Wall.h"
#include "Barrack.h"
#include "Troop.h"
#include "Archer.h"
#include "Barbarian.h"
#include "Enemy.h"
#include "Raider.h"
#include "Player.h"
#include "Board.h"

// Console utilities for display
#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

// Function to clear the console screen
/*void clearScreen() {
    system(CLEAR_SCREEN);
}

// Function to render the game board
void renderBoard(const Board& board) {
    const int sizeX = board.getSizeX();
    const int sizeY = board.getSizeY();

    // Create a 2D grid for the board
    std::vector<std::vector<const char*>> grid(sizeY, std::vector<const char*>(sizeX, " "));

    // Draw the border
    for (int x = 0; x < sizeX; x++) {
        grid[0][x] = "-";
        grid[sizeY - 1][x] = "-";
    }

    for (int y = 0; y < sizeY; y++) {
        grid[y][0] = "|";
        grid[y][sizeX - 1] = "|";
    }

    // Draw buildings
    const std::vector<Building*>& buildings = board.getBuildings();
    for (const Building* building : buildings) {
        if (building->isDestroyed()) continue;

        Position pos = building->getPosition();
        int sizeX = building->getSizeX();
        int sizeY = building->getSizeY();
        const char* repr = building->getRepr();

        // For buildings larger than 1x1, just mark the top-left corner
        grid[pos.Y][pos.X] = repr;

        // For large buildings, fill the area
        for (int y = 0; y < sizeY && pos.Y + y < board.getSizeY(); y++) {
            for (int x = 0; x < sizeX && pos.X + x < board.getSizeX(); x++) {
                grid[pos.Y + y][pos.X + x] = repr;
            }
        }
    }

    // Draw enemies
    const std::vector<Enemy*>& enemies = board.getEnemies();
    for (const Enemy* enemy : enemies) {
        if (!enemy->isAlive()) continue;

        Position pos = enemy->getPosition();
        if (pos.X > 0 && pos.X < sizeX && pos.Y > 0 && pos.Y < sizeY) {
            grid[pos.Y][pos.X] = enemy->getRepr();
        }
    }

    // Draw the player
    Player* player = board.getPlayer();
    if (player) {
        Position pos = player->getPosition();
        if (pos.X > 0 && pos.X < sizeX && pos.Y > 0 && pos.Y < sizeY) {
            grid[pos.Y][pos.X] = player->getRepr();
        }
    }

    // Print the grid
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            std::cout << grid[y][x];
        }
        std::cout << std::endl;
    }

    // Print resources if player exists
    if (player) {
        Resources& res = player->getResources();
        std::cout << "Gold: " << res.getGold() << " | Elixir: " << res.getElixir() << std::endl;
    }

    // Print game instructions
    std::cout << "\nCommands: w/a/s/d - move, c - collect, b - build, t - train troops, q - quit" << std::endl;
}*/

// Function to display building options
void displayBuildingOptions() {
    std::cout << "Building options:" << std::endl;
    std::cout << "1. Gold Mine (Cost: 0 Gold, 100 Elixir)" << std::endl;
    std::cout << "2. Elixir Collector (Cost: 100 Gold, 0 Elixir)" << std::endl;
    std::cout << "3. Town Hall (Cost: 0 Gold, 0 Elixir)" << std::endl;
    std::cout << "4. Wall (Cost: 10 Gold, 0 Elixir)" << std::endl;
    std::cout << "5. Barrack (Cost: 200 Gold, 0 Elixir)" << std::endl;
    std::cout << "0. Cancel" << std::endl;
    std::cout << "Enter your choice: ";
}

// Function to display troop training options
void displayTroopOptions() {
    std::cout << "Troop options:" << std::endl;
    std::cout << "1. Archer (Cost: 0 Gold, 15 Elixir)" << std::endl;
    std::cout << "2. Barbarian (Cost: 0 Gold, 10 Elixir)" << std::endl;
    std::cout << "0. Cancel" << std::endl;
    std::cout << "Enter your choice: ";
}

// Function to spawn a random raider on the edge of the board
void spawnRaider(Board& board) {
    int sizeX = board.getSizeX();
    int sizeY = board.getSizeY();
    Position pos;

    // Pick a random edge
    int edge = rand() % 4;

    switch (edge) {
        case 0: // Top edge
            pos.X = 1 + rand() % (sizeX - 2);
            pos.Y = 1;
            break;
        case 1: // Right edge
            pos.X = sizeX - 2;
            pos.Y = 1 + rand() % (sizeY - 2);
            break;
        case 2: // Bottom edge
            pos.X = 1 + rand() % (sizeX - 2);
            pos.Y = sizeY - 2;
            break;
        case 3: // Left edge
            pos.X = 1;
            pos.Y = 1 + rand() % (sizeY - 2);
            break;
    }

    Raider* raider = new Raider(pos);
    board.AddEnemy(raider);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialize the game board
    Board board(50, 20);

    // Create the player
    Player* player = new Player(Position(10, 10));
    board.setPlayer(player);

    // Add an initial town hall
    TownHall* townHall = new TownHall(Position(25, 10));
    board.AddBuilding(townHall);

    // Add initial resources
    GoldMine* goldMine = new GoldMine(Position(20, 10));
    board.AddBuilding(goldMine);

    ElixirCollector* elixirCollector = new ElixirCollector(Position(30, 10));
    board.AddBuilding(elixirCollector);

    // Game loop
    bool running = true;
    int gameTime = 0;
    int raiderSpawnInterval = 50; // Spawn a raider every 50 game ticks

    while (running) {
        clearScreen();


        // Check if game is over
        if (board.isGameOver()) {
            std::cout << "Game Over! The Town Hall was destroyed!" << std::endl;
            break;
        }

        // Spawn raiders periodically
        if (gameTime % raiderSpawnInterval == 0 && gameTime > 0) {
            spawnRaider(board);
        }

        // Handle player input
        std::cout << "Enter command: ";
        char input;
        std::cin >> input;
        input = std::tolower(input);

        switch (input) {
            case 'w': // Move up
                player->moving(board, 0, -1);
                break;
            case 'a': // Move left
                player->moving(board, -1, 0);
                break;
            case 's': // Move down
                player->moving(board, 0, 1);
                break;
            case 'd': // Move right
                player->moving(board, 1, 0);
                break;
            case 'c': // Collect resources
                player->collectResources(board);
                break;
            case 'b': // Build a structure
                displayBuildingOptions();
                int buildingChoice;
                std::cin >> buildingChoice;

                if (buildingChoice > 0 && buildingChoice <= 5) {
                    Position buildPos = player->getPosition();
                    buildPos.X += 1; // Build to the right of the player

                    Building* newBuilding = nullptr;

                    switch (buildingChoice) {
                        case 1: // Gold Mine
                            newBuilding = new GoldMine(buildPos);
                            break;
                        case 2: // Elixir Collector
                            newBuilding = new ElixirCollector(buildPos);
                            break;
                        case 3: // Town Hall
                            newBuilding = new TownHall(buildPos);
                            break;
                        case 4: // Wall
                            newBuilding = new Wall(buildPos);
                            break;
                        case 5: // Barrack
                            newBuilding = new Barrack(buildPos);
                            break;
                    }

                    if (newBuilding) {
                        Resources& res = player->getResources();
                        Resources cost = newBuilding->getCost();

                        if (res.canAfford(cost.getGold(), cost.getElixir())) {
                            if (board.AddBuilding(newBuilding)) {
                                res.spendGold(cost.getGold());
                                res.spendElixir(cost.getElixir());
                                std::cout << "Building placed successfully!" << std::endl;
                            } else {
                                std::cout << "Cannot place building there!" << std::endl;
                                delete newBuilding;
                            }
                        } else {
                            std::cout << "Not enough resources!" << std::endl;
                            delete newBuilding;
                        }
                    }
                }
                break;
            case 't': // Train troops
                {
                    // Find a barrack
                    Barrack* barrack = nullptr;
                    for (Building* building : board.getBuildings()) {
                        barrack = dynamic_cast<Barrack*>(building);
                        if (barrack && !barrack->isDestroyed()) {
                            break;
                        }
                    }

                    if (!barrack) {
                        std::cout << "You need to build a Barrack first!" << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        break;
                    }

                    displayTroopOptions();
                    int troopChoice;
                    std::cin >> troopChoice;

                    if (troopChoice > 0 && troopChoice <= 2) {
                        Troop* newTroop = nullptr;

                        switch (troopChoice) {
                            case 1: // Archer
                                newTroop = new Archer();
                                break;
                            case 2: // Barbarian
                                newTroop = new Barbarian();
                                break;
                        }

                        if (newTroop) {
                            Resources& res = player->getResources();
                            Resources cost = newTroop->getCost();

                            if (res.canAfford(cost.getGold(), cost.getElixir())) {
                                if (barrack->Train(newTroop)) {
                                    res.spendGold(cost.getGold());
                                    res.spendElixir(cost.getElixir());
                                    std::cout << "Troop training started!" << std::endl;
                                } else {
                                    std::cout << "Barrack is at full capacity!" << std::endl;
                                    delete newTroop;
                                }
                            } else {
                                std::cout << "Not enough resources!" << std::endl;
                                delete newTroop;
                            }
                        }
                    }
                }
                break;
            case 'q': // Quit
                running = false;
                break;
            default:
                std::cout << "Invalid command!" << std::endl;
                break;
        }

        // Update game state
        board.Update();

        // Update resources
        for (Building* building : board.getBuildings()) {
            ResourceGenerator* generator = dynamic_cast<ResourceGenerator*>(building);
            if (generator) {
                generator->Update();
            }

            // Update barracks to deploy troops
            Barrack* barrack = dynamic_cast<Barrack*>(building);
            if (barrack) {
                barrack->Update(board);
            }
        }

        // Short delay to make the game playable
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        gameTime++;
    }

    std::cout << "Thanks for playing!" << std::endl;

    // Clean up
    delete player;

    return 0;
}
