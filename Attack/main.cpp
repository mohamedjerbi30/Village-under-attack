#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <typeinfo>
#include <string>
#include <fcntl.h>
#include <io.h>

#include "Board.h"
#include "Player.h"
#include "Building.h"
#include "Wall.h"
#include "TownHall.h"
#include "Raider.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "Barrack.h"
#include "Archer.h"
#include "Barbarian.h"

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define SPACE 32
#define G_KEY 103
#define E_KEY 101
#define W_KEY 119
#define C_KEY 99
#define T_KEY 116
#define B_KEY 98
#define A_KEY 97
#define R_KEY 114

// Variables globales pour le rendu
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;
std::wstring* screenBuffer = nullptr;
int bufferWidth = 0;
int bufferHeight = 0;

// Fonctions pour gérer l'affichage
void gotoXY(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void initScreenBuffer(int width, int height) {
    bufferWidth = width;
    bufferHeight = height;

    if (screenBuffer != nullptr) {
        delete[] screenBuffer;
    }

    screenBuffer = new std::wstring[height];
    for (int y = 0; y < height; y++) {
        screenBuffer[y] = std::wstring(width, L' ');
    }
}

void setChar(int x, int y, wchar_t ch) {
    if (x >= 0 && x < bufferWidth && y >= 0 && y < bufferHeight) {
        screenBuffer[y][x] = ch;
    }
}

void setChar(int x, int y, const std::wstring& str) {
    if (x >= 0 && x < bufferWidth && y >= 0 && y < bufferHeight && !str.empty()) {
        screenBuffer[y][x] = str[0];
    }
}

void setString(int x, int y, const std::wstring& str) {
    if (y >= 0 && y < bufferHeight) {
        for (int i = 0; i < static_cast<int>(str.length()) && x + i < bufferWidth; i++) {
            screenBuffer[y][x + i] = str[i];
        }
    }
}

void renderScreen() {
    static CHAR_INFO* charBuffer = nullptr;
    static COORD bufferSize = { 0, 0 };
    static COORD bufferCoord = { 0, 0 };
    static SMALL_RECT writeRegion = { 0, 0, 0, 0 };

    // Redimensionner le buffer si nécessaire
    if (charBuffer == nullptr || bufferSize.X != bufferWidth || bufferSize.Y != bufferHeight) {
        delete[] charBuffer;
        charBuffer = new CHAR_INFO[bufferWidth * bufferHeight];
        bufferSize.X = bufferWidth;
        bufferSize.Y = bufferHeight;
        writeRegion.Right = bufferWidth - 1;
        writeRegion.Bottom = bufferHeight - 1;
    }

    // Copier le contenu du buffer dans le charBuffer
    for (int y = 0; y < bufferHeight; y++) {
        for (int x = 0; x < bufferWidth; x++) {
            int index = y * bufferWidth + x;
            charBuffer[index].Char.UnicodeChar = screenBuffer[y][x];
            charBuffer[index].Attributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        }
    }

    // Écrire d'un coup sur la console
    WriteConsoleOutputW(console, charBuffer, bufferSize, bufferCoord, &writeRegion);
}

// Updated drawBoard function to include troops
void drawBoard(const Board& board, const Player& player, bool placingWalls, const std::vector<Troop*>& troops) {
    int offsetX = 15;
    int boardWidth = board.getSizeX();
    int infoX = offsetX + boardWidth + 5;

    // Ressources
    setString(infoX, 0, L"💰 Gold: " + std::to_wstring(player.getResources().getGold()) + L"    ");
    setString(infoX, 1, L"🔮 Elixir: " + std::to_wstring(player.getResources().getElixir()) + L"    ");

    // Contrôles
    setString(infoX, 3, L"🎮 Contrôles:");
    setString(infoX, 4, L"⬅️➡️⬆️⬇️ : Déplacer");
    setString(infoX, 5, L"G: 🪙 Mine d'or (0 or, 100 élixir)");
    setString(infoX, 6, L"E: 💧 Collecteur d'élixir (100 or, 0 élixir)");
    setString(infoX, 7, L"W: 🧱 Mur (10 or, 0 élixir)");
    setString(infoX, 8, L"B: 🏕️ Caserne (200 or, 0 élixir)");
    setString(infoX, 9, L"A: 🏹 Archer (0 or, 15 élixir)");
    setString(infoX, 10, L"R: ⚔️ Barbare (0 or, 10 élixir)");
    setString(infoX, 11, L"C: 📦 Collecter les ressources");
    setString(infoX, 12, L"T: 🏛 Construire l'hôtel de ville");

    // Dessiner le plateau
    for (int y = 0; y < board.getSizeY(); y++) {
        for (int x = 0; x < board.getSizeX(); x++) {
            bool printed = false;
            int screenX = offsetX + x;
            int screenY = y + 4;

            if (y == 0 || y == board.getSizeY() - 1 || x == 0 || x == board.getSizeX() - 1) {
                setChar(screenX, screenY, L'·');
                continue;
            }

            // Draw player
            if (player.getPosition().X == x && player.getPosition().Y == y) {
                setChar(screenX, screenY, player.getRepr()[0]);
                printed = true;
            }
            // Draw enemies
            else {
                for (const Enemy* enemy : board.getEnemies()) {
                    if (enemy->getPosition().X == x && enemy->getPosition().Y == y) {
                        setChar(screenX, screenY, enemy->getRepr()[0]);
                        printed = true;
                        break;
                    }
                }
            }

            // Draw troops if not already occupied
            if (!printed) {
                for (const Troop* troop : troops) {
                    if (troop->getPosition().X == x && troop->getPosition().Y == y) {
                        setChar(screenX, screenY, troop->getRepr()[0]);
                        printed = true;
                        break;
                    }
                }
            }

            // Draw buildings if not already occupied
            if (!printed) {
                for (const Building* b : board.getBuildings()) {
                    if (b->collidesWith(Position(x, y))) {
                        Position topLeft = b->getPosition();
                        int centerX = topLeft.X;
                        int centerY = topLeft.Y;
                        if (x == centerX && y == centerY) {
                            setChar(screenX, screenY, b->getRepr()[0]);
                        } else {
                            setChar(screenX, screenY, L' ');
                        }
                        printed = true;
                        break;
                    }
                }
            }

            if (!printed) setChar(screenX, screenY, L' ');
        }
    }

    setString(infoX, 14, L"📊 Statut:");
    setString(infoX, 15, L"Bâtiments: " + std::to_wstring(board.getBuildings().size()) + L"    ");
    setString(infoX, 16, L"Troupes: " + std::to_wstring(troops.size()) + L"    ");
    setString(infoX, 17, L"Ennemis: " + std::to_wstring(board.getEnemies().size()) + L"    ");

    if (board.isGameOver()) {
        setString(infoX, 19, L"💀 GAME OVER - L'hôtel de ville a été détruit!");
    }

    // Affichage du mode mur
    setString(0, board.getSizeY() + 3, placingWalls ? L"🧱 Mode construction de mur: ACTIVÉ" : L"🧱 Mode construction de mur: DÉSACTIVÉ");
}

void spawnEnemies(Board& board, int& enemyTimer) {
    enemyTimer++;
    if (enemyTimer >= 50) {
        enemyTimer = 0;
        int side = rand() % 4;
        int x, y;

        switch (side) {
            case 0: x = 1 + rand() % (board.getSizeX() - 2); y = 1; break;
            case 1: x = board.getSizeX() - 2; y = 1 + rand() % (board.getSizeY() - 2); break;
            case 2: x = 1 + rand() % (board.getSizeX() - 2); y = board.getSizeY() - 2; break;
            case 3: x = 1; y = 1 + rand() % (board.getSizeY() - 2); break;
        }

        Raider* raider = new Raider(Position(x, y));
        board.AddEnemy(raider);
    }
}

void configureConsoleForUnicode() {
    // Set UTF-8 output code page
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Set stdout and stdin to handle UTF-8 properly
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);

    // Set font to Segoe UI Emoji
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    wcscpy_s(cfi.FaceName, L"Segoe UI Emoji");
    cfi.dwFontSize.Y = 18;  // Optional: increase font size for better emoji rendering
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

// Function to find a barrack near the player
Barrack* findNearestBarrack(const Board& board, const Player& player) {
    for (Building* building : board.getBuildings()) {
        Barrack* barrack = dynamic_cast<Barrack*>(building);
        if (barrack) {
            // Check if player is adjacent to the barrack
            Position playerPos = player.getPosition();
            Position barrackPos = barrack->getPosition();

            int dx = abs(playerPos.X - barrackPos.X);
            int dy = abs(playerPos.Y - barrackPos.Y);

            if (dx <= 2 && dy <= 2) {
                return barrack;
            }
        }
    }
    return nullptr;
}

// Updated function to handle player input including troop training
void handlePlayerInput(Board& board, Player& player, bool& placingWalls, std::vector<Troop*>& troops) {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) {
            key = _getch();

            Position before = player.getPosition();

            if (key == KEY_UP) player.moving(board, 0, -1);
            else if (key == KEY_DOWN) player.moving(board, 0, 1);
            else if (key == KEY_LEFT) player.moving(board, -1, 0);
            else if (key == KEY_RIGHT) player.moving(board, 1, 0);

            // Place wall if moving and wall mode is active
            if (placingWalls && player.getResources().canAfford(10, 0)) {
                Wall* newWall = new Wall(player.getPosition());
                if (board.AddBuilding(newWall)) {
                    player.getResources().spendGold(10);
                } else {
                    delete newWall;
                }
            }
        } else {
            if (key == G_KEY && player.getResources().canAfford(0, 100)) {
                GoldMine* newGM = new GoldMine(player.getPosition());
                if (board.AddBuilding(newGM)) player.getResources().spendElixir(100);
                else delete newGM;
            }
            else if (key == E_KEY && player.getResources().canAfford(100, 0)) {
                ElixirCollector* newEC = new ElixirCollector(player.getPosition());
                if (board.AddBuilding(newEC)) player.getResources().spendGold(100);
                else delete newEC;
            }
            else if (key == W_KEY) {
                placingWalls = !placingWalls; // Toggle wall mode
            }
            else if (key == C_KEY) player.collectResources(board);
            else if (key == T_KEY) {
                TownHall* newTH = new TownHall(player.getPosition());
                if (!board.AddBuilding(newTH)) delete newTH;
            }
            else if (key == B_KEY && player.getResources().canAfford(200, 0)) {
                Barrack* newBarrack = new Barrack(player.getPosition());
                if (board.AddBuilding(newBarrack)) player.getResources().spendGold(200);
                else delete newBarrack;
            }
            else if (key == A_KEY && player.getResources().canAfford(0, 15)) {
                // Find the nearest barrack
                Barrack* barrack = findNearestBarrack(board, player);
                if (barrack) {
                    // Train an archer
                    Position archerPos = barrack->getPosition();
                    // Place archer adjacent to barrack
                    archerPos.X += 1;

                    Archer* archer = new Archer(archerPos);
                    troops.push_back(archer);
                    player.getResources().spendElixir(15);
                }
            }
            else if (key == R_KEY && player.getResources().canAfford(0, 10)) {
                // Find the nearest barrack
                Barrack* barrack = findNearestBarrack(board, player);
                if (barrack) {
                    // Train a barbarian
                    Position barbarianPos = barrack->getPosition();
                    // Place barbarian adjacent to barrack
                    barbarianPos.X += 1;

                    Barbarian* barbarian = new Barbarian(barbarianPos);
                    troops.push_back(barbarian);
                    player.getResources().spendElixir(10);
                }
            }
        }
    }
}

// Function to update troops
void updateTroops(Board& board, std::vector<Troop*>& troops) {
    // Remove dead troops
    auto it = troops.begin();
    while (it != troops.end()) {
        if (!(*it)->isAlive()) {
            delete *it;
            it = troops.erase(it);
        } else {
            // Update living troops
            (*it)->Update(board);
            ++it;
        }
    }
}

// Updated function to update game state including troops
void updateGameState(Board& board, std::vector<Troop*>& troops, int& resourceTimer) {
    resourceTimer++;
    if (resourceTimer >= 10) {
        resourceTimer = 0;
        for (Building* building : board.getBuildings()) {
            building->Update();
        }
    }

    // Update troops
    updateTroops(board, troops);

    // Mise à jour du plateau (ennemis, bâtiments détruits, etc.)
    board.Update();
}

// Fonction pour initialiser le jeu
void initializeGame(Board& board, Player& player) {
    board.setPlayer(&player);

    int centerX = board.getSizeX() / 2;
    int centerY = board.getSizeY() / 2;

    // Création des bâtiments initiaux
    GoldMine* gm = new GoldMine(Position(centerX - 4, centerY));
    ElixirCollector* ec = new ElixirCollector(Position(centerX + 2, centerY));
    TownHall* th = new TownHall(Position(centerX, centerY + 5));
    Barrack* barrack = new Barrack(Position(centerX, centerY - 5));

    // Ajout des bâtiments au plateau
    board.AddBuilding(gm);
    board.AddBuilding(ec);
    board.AddBuilding(th);
    board.AddBuilding(barrack);
}

void cleanupTroops(std::vector<Troop*>& troops) {
    for (Troop* troop : troops) {
        delete troop;
    }
    troops.clear();
}

int main() {
    // Configuration optimisée pour l'Unicode et les émojis
    configureConsoleForUnicode();

    std::wcout << L"✅ Jeu initialisé avec support des émojis 🎮\n";
    std::wcout << L"   Appuyez sur une touche pour commencer... ⌨️\n";
    _getch();

    // Cacher le curseur
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(console, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(console, &cursorInfo);

    // Initialisation du générateur de nombre aléatoire
    srand(static_cast<unsigned int>(time(nullptr)));

    // Création et initialisation des objets principaux du jeu
    Board board(70, 20);
    Player player(Position(5, 5));
    std::vector<Troop*> troops;

    // Initialiser le jeu
    initializeGame(board, player);

    // Variables de jeu
    bool placingWalls = false;
    int enemyTimer = 0;
    int resourceTimer = 0;

    // Initialiser le buffer d'écran
    initScreenBuffer(board.getSizeX() + 70, board.getSizeY() + 20);

    // Variables pour la boucle de jeu et la gestion du temps
    DWORD lastUpdateTime = GetTickCount();
    const DWORD targetFrameTime = 50; // 20 FPS

    // Boucle principale du jeu
    while (!board.isGameOver()) {
        // Gestion du timing pour maintenir un framerate constant
        DWORD currentTime = GetTickCount();
        DWORD elapsedTime = currentTime - lastUpdateTime;

        if (elapsedTime < targetFrameTime) {
            Sleep(targetFrameTime - elapsedTime);
            currentTime = GetTickCount();
            elapsedTime = currentTime - lastUpdateTime;
        }
        lastUpdateTime = currentTime;

        // Gérer les entrées du joueur
        handlePlayerInput(board, player, placingWalls, troops);

        // Mettre à jour l'état du jeu
        updateGameState(board, troops, resourceTimer);

        // Faire apparaître des ennemis
        spawnEnemies(board, enemyTimer);

        // Effacer le buffer
        for (int y = 0; y < bufferHeight; y++) {
            for (int x = 0; x < bufferWidth; x++) {
                screenBuffer[y][x] = L' ';
            }
        }

        // Dessiner le plateau
        drawBoard(board, player, placingWalls, troops);

        // Afficher le rendu
        renderScreen();
    }

    // Afficher le message de fin de jeu
    setString(30, board.getSizeY() + 6, L"💀 GAME OVER - Appuyez sur une touche pour quitter...");
    renderScreen();
    _getch();

    // Libérer les ressources
    delete[] screenBuffer;
    cleanupTroops(troops);

    return 0;
}
