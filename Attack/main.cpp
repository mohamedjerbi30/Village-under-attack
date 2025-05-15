#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <chrono>

#include "Board.h"
#include "Player.h"
#include "Building.h"
#include "Wall.h"
#include "TownHall.h"
#include "Raider.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "Barrack.h"
#include "Barbarian.h"
#include "Archer.h"
#include "Bomberman.h"

// Key definitions
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define KEY_SPACE 32
#define KEY_B 98    // For building mode
#define KEY_W 119   // For wall building
#define KEY_G 103   // For gold mine
#define KEY_E 101   // For elixir collector
#define KEY_T 116   // For town hall
#define KEY_R 114   // For barracks
#define KEY_A 97    // For training archers
#define KEY_D 100   // For training barbarians
#define KEY_ESC 27  // For exit

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoXY(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

// Helper function to clear the screen
void clearScreen() {
    COORD topLeft = { 0, 0 };
    DWORD written;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console, &csbi);
    FillConsoleOutputCharacterA(console, ' ', csbi.dwSize.X * csbi.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

// Display game instructions
void displayInstructions() {
    clearScreen();
    std::cout << "=== VILLAGE UNDER ATTACK - GAME INSTRUCTIONS ===" << std::endl << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  Arrow Keys - Move player" << std::endl;
    std::cout << "  B - Toggle building mode" << std::endl;
    std::cout << "  In building mode:" << std::endl;
    std::cout << "    W - Build Wall (10 Gold)" << std::endl;
    std::cout << "    G - Build Gold Mine (100 Gold)" << std::endl;
    std::cout << "    E - Build Elixir Collector (100 Elixir)" << std::endl;
    std::cout << "    T - Build Town Hall (200 Gold, 200 Elixir)" << std::endl;
    std::cout << "    R - Build Barracks (150 Gold, 50 Elixir)" << std::endl;
    std::cout << "  Stand next to Barracks and press:" << std::endl;
    std::cout << "    A - Train Archer (15 Elixir)" << std::endl;
    std::cout << "    D - Train Barbarian (3 Elixir)" << std::endl;
    std::cout << "  ESC - Exit game" << std::endl << std::endl;
    std::cout << "Game Objective:" << std::endl;
    std::cout << "  Protect your Town Hall from Raiders and Bombermen" << std::endl;
    std::cout << "  Collect resources and build defenses" << std::endl;
    std::cout << "  You lose when your Town Hall is destroyed" << std::endl << std::endl;
    std::cout << "Press any key to start...";
    _getch();
}

// Draw the game board with all entities
void drawBoard(Board& board, Player& player, const std::vector<Raider*>& raiders,
               const std::vector<Bomberman*>& bombermen, const std::vector<Troop*>& troops,
               bool buildingMode, const std::string& buildingType) {
    clearScreen();

    int boardWidth = board.getSizeX();
    int boardHeight = board.getSizeY();

    // Draw resources and game information
    std::cout << "Gold: " << player.getResources().getGold() << "\tElixir: " << player.getResources().getElixir() << std::endl;
    std::cout << "Building Mode: " << (buildingMode ? "ON" : "OFF");
    if (buildingMode) {
        std::cout << " - Building: " << buildingType;
    }
    std::cout << std::endl;

    // Draw controls reminder
    std::cout << "Controls: B-Build Mode, W-Wall, G-Gold Mine, E-Elixir Collector, T-Town Hall, R-Barracks, ESC-Exit" << std::endl;
    std::cout << "         A-Train Archer, D-Train Barbarian" << std::endl << std::endl;

    // Draw board border
    for (int x = 0; x <= boardWidth; x++) {
        std::cout << "·";
    }
    std::cout << std::endl;

    // Draw board content
    for (int y = 0; y < boardHeight; y++) {
        std::cout << "·"; // Left border
        for (int x = 0; x < boardWidth; x++) {
            Position currentPos(x, y);
            bool printed = false;

            // Check player position
            if (player.getPosition() == currentPos) {
                std::cout << player.getRepr();
                printed = true;
                continue;
            }

            // Check enemy positions - Raiders
            for (const Raider* raider : raiders) {
                if (raider->getPosition() == currentPos) {
                    std::cout << raider->getRepr();
                    printed = true;
                    break;
                }
            }
            if (printed) continue;

            // Check enemy positions - Bombermen
            for (const Bomberman* bomber : bombermen) {
                if (bomber->getPosition() == currentPos) {
                    std::cout << bomber->getRepr();
                    printed = true;
                    break;
                }
            }
            if (printed) continue;

            // Check troop positions
            for (const Troop* troop : troops) {
                if (troop->getPosition() == currentPos) {
                    std::cout << troop->getRepr();
                    printed = true;
                    break;
                }
            }
            if (printed) continue;

            // Check building positions
            for (const Building* building : board.getBuildings()) {
                if (building->collidesWith(currentPos) && building->isAlive()) {
                    Position topLeft = building->getPosition();
                    int centerX = topLeft.X + building->getSizeX() / 2;
                    int centerY = topLeft.Y + building->getSizeY() / 2;

                    if (x == centerX && y == centerY) {
                        std::cout << building->getRepr();
                    } else {
                        std::cout << " ";
                    }
                    printed = true;
                    break;
                }
            }

            // Empty space
            if (!printed) {
                std::cout << " ";
            }
        }
        std::cout << "·" << std::endl; // Right border
    }

    // Draw bottom border
    for (int x = 0; x <= boardWidth; x++) {
        std::cout << "·";
    }
    std::cout << std::endl;

    // Display game message if townhall is not found or destroyed
    TownHall* townhall = board.getTownHall();
    if (townhall == nullptr || townhall->getHealth() <= 0) {
        std::cout << "\nGAME OVER! Your Town Hall has been destroyed!" << std::endl;
        std::cout << "Press ESC to exit..." << std::endl;
    }
}

// Get a Barracks from the position next to player
Barrack* getAdjacentBarrack(const Player& player, const Board& board) {
    Position playerPos = player.getPosition();
    // Check all adjacent positions
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue; // Skip player position

            Position checkPos(playerPos.X + dx, playerPos.Y + dy);
            for (Building* building : board.getBuildings()) {
                if (building->collidesWith(checkPos)) {
                    Barrack* barrack = dynamic_cast<Barrack*>(building);
                    if (barrack != nullptr) {
                        return barrack;
                    }
                }
            }
        }
    }
    return nullptr;
}

// Main function
int main() {
    // Setup console for UTF-8 emoji display
    SetConsoleOutputCP(CP_UTF8);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);

    // Seed random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Display game instructions
    displayInstructions();

    // Initialize game components
    Board board(50, 20);
    Player player(Position(5, 5));
    board.setPlayer(&player);

    // Place initial town hall
    TownHall* townHall = new TownHall(Position(25, 10));
    board.AddBuilding(townHall);

    // Place initial resource buildings
    GoldMine* goldMine = new GoldMine(Position(20, 10));
    ElixirCollector* elixirCollector = new ElixirCollector(Position(30, 10));
    board.AddBuilding(goldMine);
    board.AddBuilding(elixirCollector);

    // Game state variables
    bool running = true;
    bool buildingMode = false;
    std::string currentBuildingType = "None";

    // Game entities
    std::vector<Raider*> raiders;
    std::vector<Bomberman*> bombermen;
    std::vector<Troop*> troops;

    // Game timers
    unsigned long lastRaiderSpawn = GetTickCount();
    unsigned long lastBombermanSpawn = GetTickCount();
    unsigned long lastEnemyUpdate = GetTickCount();
    unsigned long lastResourceUpdate = GetTickCount();
    unsigned long lastTroopUpdate = GetTickCount();

    // Game loop
    while (running) {
        // Check for player input
        if (_kbhit()) {
            int key = _getch();

            // Arrow key input (needs two reads)
            if (key == 224) {
                key = _getch();
                switch (key) {
                    case KEY_UP:    player.moving(board, raiders, 0, -1); break;
                    case KEY_DOWN:  player.moving(board, raiders, 0, 1); break;
                    case KEY_LEFT:  player.moving(board, raiders, -1, 0); break;
                    case KEY_RIGHT: player.moving(board, raiders, 1, 0); break;
                }
            } else {
                // Regular key input
                switch (key) {
                    case KEY_ESC:
                        running = false;
                        break;

                    case KEY_B: // Toggle building mode
                        buildingMode = !buildingMode;
                        currentBuildingType = buildingMode ? "Select building type" : "None";
                        break;

                    case KEY_W: // Build Wall
                        if (buildingMode && player.getResources().getGold() >= 10) {
                            Position wallPos = player.getPosition();
                            Wall* wall = new Wall(wallPos);
                            if (board.AddBuilding(wall)) {
                                player.getResources().spendGold(10);
                                currentBuildingType = "Wall";
                            } else {
                                delete wall;
                            }
                        }
                        break;

                    case KEY_G: // Build Gold Mine
                        if (buildingMode && player.getResources().getGold() >= 100) {
                            Position minePos = player.getPosition();
                            GoldMine* mine = new GoldMine(minePos);
                            if (board.AddBuilding(mine)) {
                                player.getResources().spendGold(100);
                                currentBuildingType = "Gold Mine";
                            } else {
                                delete mine;
                            }
                        }
                        break;

                    case KEY_E: // Build Elixir Collector
                        if (buildingMode && player.getResources().getElixir() >= 100) {
                            Position collectorPos = player.getPosition();
                            ElixirCollector* collector = new ElixirCollector(collectorPos);
                            if (board.AddBuilding(collector)) {
                                player.getResources().spendElixir(100);
                                currentBuildingType = "Elixir Collector";
                            } else {
                                delete collector;
                            }
                        }
                        break;

                    case KEY_T: // Build Town Hall (if none exists)
                        if (buildingMode && board.getTownHall() == nullptr &&
                            player.getResources().getGold() >= 200 &&
                            player.getResources().getElixir() >= 200) {
                            Position thPos = player.getPosition();
                            TownHall* th = new TownHall(thPos);
                            if (board.AddBuilding(th)) {
                                player.getResources().spendGold(200);
                                player.getResources().spendElixir(200);
                                currentBuildingType = "Town Hall";
                            } else {
                                delete th;
                            }
                        }
                        break;

                    case KEY_R: // Build Barrack
                        if (buildingMode && player.getResources().getGold() >= 150 &&
                            player.getResources().getElixir() >= 50) {
                            Position barrackPos = player.getPosition();
                            Barrack* barrack = new Barrack(barrackPos);
                            if (board.AddBuilding(barrack)) {
                                player.getResources().spendGold(150);
                                player.getResources().spendElixir(50);
                                currentBuildingType = "Barrack";
                            } else {
                                delete barrack;
                            }
                        }
                        break;

                    case KEY_A: // Train Archer
                        {
                            Barrack* barrack = getAdjacentBarrack(player, board);
                            if (barrack != nullptr && player.getResources().getElixir() >= 15) {
                                Archer* archer = new Archer(barrack->getPosition());
                                if (barrack->Train(archer)) {
                                    player.getResources().spendElixir(15);
                                } else {
                                    delete archer;
                                }
                            }
                        }
                        break;

                    case KEY_D: // Train Barbarian
                        {
                            Barrack* barrack = getAdjacentBarrack(player, board);
                            if (barrack != nullptr && player.getResources().getElixir() >= 3) {
                                Barbarian* barbarian = new Barbarian(barrack->getPosition());
                                if (barrack->Train(barbarian)) {
                                    player.getResources().spendElixir(3);
                                } else {
                                    delete barbarian;
                                }
                            }
                        }
                        break;
                }
            }
        }

        // Current time for updates
        unsigned long currentTime = GetTickCount();

        // Update resources (every 2 seconds)
        if (currentTime - lastResourceUpdate >= 2000) {
            for (Building* building : board.getBuildings()) {
                // Update gold mines
                GoldMine* goldMine = dynamic_cast<GoldMine*>(building);
                if (goldMine != nullptr) {
                    // Check if player is adjacent to collect resources
                    if (player.getPosition().manhattanDistance(goldMine->getPosition()) <= 1) {
                        Resources collected = goldMine->Collect();
                        player.getResources().addGold(collected.getGold());
                    }
                }

                // Update elixir collectors
                ElixirCollector* elixirCollector = dynamic_cast<ElixirCollector*>(building);
                if (elixirCollector != nullptr) {
                    // Check if player is adjacent to collect resources
                    if (player.getPosition().manhattanDistance(elixirCollector->getPosition()) <= 1) {
                        Resources collected = elixirCollector->Collect();
                        player.getResources().addElixir(collected.getElixir());
                    }
                }
            }

            lastResourceUpdate = currentTime;
        }

        // Spawn a Raider (every 12 seconds)
        if (currentTime - lastRaiderSpawn >= 12000) {
            // Spawn at a random edge position
            int x, y;
            int edge = rand() % 4; // 0: top, 1: right, 2: bottom, 3: left

            switch (edge) {
                case 0: // top
                    x = rand() % (board.getSizeX() - 2) + 1;
                    y = 1;
                    break;
                case 1: // right
                    x = board.getSizeX() - 2;
                    y = rand() % (board.getSizeY() - 2) + 1;
                    break;
                case 2: // bottom
                    x = rand() % (board.getSizeX() - 2) + 1;
                    y = board.getSizeY() - 2;
                    break;
                case 3: // left
                    x = 1;
                    y = rand() % (board.getSizeY() - 2) + 1;
                    break;
            }

            raiders.push_back(new Raider(Position(x, y)));
            lastRaiderSpawn = currentTime;
        }

        // Spawn a Bomberman (every 20 seconds)
        if (currentTime - lastBombermanSpawn >= 20000) {
            // Spawn at a random edge position
            int x, y;
            int edge = rand() % 4; // 0: top, 1: right, 2: bottom, 3: left

            switch (edge) {
                case 0: // top
                    x = rand() % (board.getSizeX() - 2) + 1;
                    y = 1;
                    break;
                case 1: // right
                    x = board.getSizeX() - 2;
                    y = rand() % (board.getSizeY() - 2) + 1;
                    break;
                case 2: // bottom
                    x = rand() % (board.getSizeX() - 2) + 1;
                    y = board.getSizeY() - 2;
                    break;
                case 3: // left
                    x = 1;
                    y = rand() % (board.getSizeY() - 2) + 1;
                    break;
            }

            bombermen.push_back(new Bomberman(Position(x, y)));
            lastBombermanSpawn = currentTime;
        }

        // Update enemies (every 1 second)
        if (currentTime - lastEnemyUpdate >= 1000) {
            // Update raiders
            for (auto raider_it = raiders.begin(); raider_it != raiders.end();) {
                (*raider_it)->Update(board);

                // Remove dead raiders
                if ((*raider_it)->getHealth() <= 0) {
                    delete *raider_it;
                    raider_it = raiders.erase(raider_it);
                } else {
                    ++raider_it;
                }
            }

            // Update bombermen
            for (auto bomber_it = bombermen.begin(); bomber_it != bombermen.end();) {
                (*bomber_it)->update(board);

                // Remove dead bombermen
                if ((*bomber_it)->getHealth() <= 0) {
                    delete *bomber_it;
                    bomber_it = bombermen.erase(bomber_it);
                } else {
                    ++bomber_it;
                }
            }

            lastEnemyUpdate = currentTime;
        }

        // Update troops (every 1 second)
        if (currentTime - lastTroopUpdate >= 1000) {
            // Update barracks to train troops
            for (Building* building : board.getBuildings()) {
                Barrack* barrack = dynamic_cast<Barrack*>(building);
                if (barrack != nullptr) {
                    barrack->Update(board);
                }
            }

            // Process troops
            for (auto troop_it = troops.begin(); troop_it != troops.end();) {
                // Find nearest enemy
                Entity* target = nullptr;
                int minDistance = INT_MAX;

                for (Raider* raider : raiders) {
                    int distance = (*troop_it)->getPosition().manhattanDistance(raider->getPosition());
                    if (distance < minDistance) {
                        minDistance = distance;
                        target = raider;
                    }
                }

                for (Bomberman* bomber : bombermen) {
                    int distance = (*troop_it)->getPosition().manhattanDistance(bomber->getPosition());
                    if (distance < minDistance) {
                        minDistance = distance;
                        target = bomber;
                    }
                }

                // Attack if in range, otherwise move towards target
                if (target != nullptr) {
                    if (minDistance <= (*troop_it)->getAttackRange()) {
                        target->takeDamage(1); // Simple attack
                    } else {
                        // Move towards target
                        Position currentPos = (*troop_it)->getPosition();
                        Position targetPos = target->getPosition();

                        int dx = 0, dy = 0;
                        if (currentPos.X < targetPos.X) dx = 1;
                        else if (currentPos.X > targetPos.X) dx = -1;

                        if (currentPos.Y < targetPos.Y) dy = 1;
                        else if (currentPos.Y > targetPos.Y) dy = -1;

                        // Only move in one direction at a time (prevent diagonal movement)
                        if (rand() % 2 == 0 && dx != 0) {
                            (*troop_it)->moving(board, dx, 0);
                        } else if (dy != 0) {
                            (*troop_it)->moving(board, 0, dy);
                        }
                    }
                }

                // Check if troop is dead
                if ((*troop_it)->Health <= 0) {
                    delete *troop_it;
                    troop_it = troops.erase(troop_it);
                } else {
                    ++troop_it;
                }
            }

            lastTroopUpdate = currentTime;
        }

        // Remove destroyed buildings
        auto& buildings = board.getBuildings();
        buildings.erase(
            std::remove_if(
                buildings.begin(),
                buildings.end(),
                [](Building* b) {
                    if (!b->isAlive()) {
                        delete b;
                        return true;
                    }
                    return false;
                }
            ),
            buildings.end()
        );

        // Check if town hall is destroyed (game over)
        TownHall* th = board.getTownHall();
        if (th == nullptr || th->getHealth() <= 0) {
            // Game over - draw one last time
            drawBoard(board, player, raiders, bombermen, troops, buildingMode, currentBuildingType);

            // Wait for ESC to exit
            while (_getch() != KEY_ESC) {}
            running = false;
            continue;
        }

        // Draw the game board
        drawBoard(board, player, raiders, bombermen, troops, buildingMode, currentBuildingType);

        // Small delay to prevent high CPU usage
        Sleep(50);
    }

    // Clean up game entities
    for (Raider* raider : raiders) {
        delete raider;
    }

    for (Bomberman* bomber : bombermen) {
        delete bomber;
    }

    for (Troop* troop : troops) {
        delete troop;
    }

    for (Building* building : board.getBuildings()) {
        delete building;
    }

    return 0;
}
