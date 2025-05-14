#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <conio.h>
#include <windows.h>
#ifdef _WIN32

#endif

#include "Board.h"
#include "Player.h"
#include "TownHall.h"
#include "GoldMine.h"
#include "ElixirCollector.h"
#include "Wall.h"
#include "Raider.h"
#include "Position.h"

// Configuration du jeu
const int BOARD_WIDTH = 60;
const int BOARD_HEIGHT = 20;
const int INFO_PANEL_WIDTH = 30;
const int TOTAL_WIDTH = BOARD_WIDTH + INFO_PANEL_WIDTH;
const int SPAWN_ENEMY_INTERVAL = 20; // Intervalle en ticks pour faire apparaître un ennemi
const int UPDATE_INTERVAL_MS = 200;  // Intervalle de mise à jour en millisecondes

// Pour Windows, on configure la console pour afficher les emojis UTF-8
void setupConsole() {
#ifdef _WIN32
    // Activer la prise en charge Unicode pour la console Windowss
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // Configuration des polices pour l'affichage des emojis
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
#endif
}

// Effacer l'écran de manière portable
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Placer le curseur à une position spécifique
void setCursorPosition(int x, int y) {
#ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    printf("\033[%d;%dH", y + 1, x + 1);
#endif
}

// Cache le curseur
void hideCursor() {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
#else
    printf("\033[?25l");
#endif
}

// Affiche le curseur
void showCursor() {
#ifdef _WIN32
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
#else
    printf("\033[?25h");
#endif
}

// Dessiner la bordure du jeu
void drawBorder(int width, int height) {
    // Coin supérieur gauche
    setCursorPosition(0, 0);
    std::cout << "┌";

    // Bordure supérieure
    for (int x = 1; x < width - 1; x++) {
        setCursorPosition(x, 0);
        std::cout << "─";
    }

    // Coin supérieur droit
    setCursorPosition(width - 1, 0);
    std::cout << "┐";

    // Bordure gauche
    for (int y = 1; y < height - 1; y++) {
        setCursorPosition(0, y);
        std::cout << "│";
    }

    // Bordure droite
    for (int y = 1; y < height - 1; y++) {
        setCursorPosition(width - 1, y);
        std::cout << "│";
    }

    // Coin inférieur gauche
    setCursorPosition(0, height - 1);
    std::cout << "└";

    // Bordure inférieure
    for (int x = 1; x < width - 1; x++) {
        setCursorPosition(x, height - 1);
        std::cout << "─";
    }

    // Coin inférieur droit
    setCursorPosition(width - 1, height - 1);
    std::cout << "┘";

    // Séparation entre la zone de jeu et le panneau d'information
    for (int y = 0; y < height; y++) {
        setCursorPosition(BOARD_WIDTH, y);
        if (y == 0) {
            std::cout << "┬";
        } else if (y == height - 1) {
            std::cout << "┴";
        } else {
            std::cout << "│";
        }
    }
}

// Initialisation du plateau de jeu
void initializeBoard(Board& board) {
    // Position centrale pour l'hôtel de ville
    int centerX = BOARD_WIDTH / 2;
    int centerY = BOARD_HEIGHT / 2;

    // Ajouter l'hôtel de ville
    TownHall* townHall = new TownHall(Position(centerX - 2, centerY - 2));
    board.AddBuilding(townHall);

    // Ajouter une mine d'or
    GoldMine* goldMine = new GoldMine(Position(centerX - 10, centerY - 3));
    board.AddBuilding(goldMine);

    // Ajouter un collecteur d'élixir
    ElixirCollector* elixirCollector = new ElixirCollector(Position(centerX + 5, centerY - 3));
    board.AddBuilding(elixirCollector);
}

// Afficher le plateau de jeu
void renderBoard(const Board& board) {
    // Nettoyage de la zone de jeu (sans effacer les bordures)
    for (int y = 1; y < BOARD_HEIGHT - 1; y++) {
        for (int x = 1; x < BOARD_WIDTH - 1; x++) {
            setCursorPosition(x, y);
            std::cout << " ";
        }
    }

    // Affichage des bâtiments
    for (Building* building : board.getBuildings()) {
        if (building->isDestroyed()) continue;

        Position pos = building->getPosition();
        int sizeX = building->getSizeX();
        int sizeY = building->getSizeY();

        // Afficher le bâtiment
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                // Vérifier que la position est dans les limites de la carte
                if (pos.X + x > 0 && pos.Y + y > 0 &&
                    pos.X + x < BOARD_WIDTH - 1 && pos.Y + y < BOARD_HEIGHT - 1) {
                    setCursorPosition(pos.X + x, pos.Y + y);

                    // Afficher différemment selon la position dans le bâtiment
                    if (x == sizeX / 2 && y == sizeY / 2) {
                        std::cout << building->getRepr();
                    } else {
                        std::cout << "*";
                    }
                }
            }
        }
    }

    // Affichage des ennemis
    for (Enemy* enemy : board.getEnemies()) {
        if (!enemy->isAlive()) continue;

        Position pos = enemy->getPosition();
        // Vérifier que la position est dans les limites de la carte
        if (pos.X > 0 && pos.Y > 0 && pos.X < BOARD_WIDTH - 1 && pos.Y < BOARD_HEIGHT - 1) {
            setCursorPosition(pos.X, pos.Y);
            std::cout << enemy->getRepr();
        }
    }

    // Affichage du joueur
    Player* player = board.getPlayer();
    if (player) {
        Position pos = player->getPosition();
        if (pos.X > 0 && pos.Y > 0 && pos.X < BOARD_WIDTH - 1 && pos.Y < BOARD_HEIGHT - 1) {
            setCursorPosition(pos.X, pos.Y);
            std::cout << player->getRepr();
        }
    }
}

// Afficher le panneau d'informations
void renderInfoPanel(const Board& board, int score) {
    int startX = BOARD_WIDTH + 2;
    int startY = 2;

    setCursorPosition(startX, startY);
    std::cout << "========= INFO =========";

    // Afficher le score
    setCursorPosition(startX, startY + 2);
    std::cout << "Score: " << score;

    // Afficher les ressources du joueur
    Player* player = board.getPlayer();
    if (player) {
        setCursorPosition(startX, startY + 4);
        std::cout << "Gold: " << player->getResources().getGold();

        setCursorPosition(startX, startY + 5);
        std::cout << "Elixir: " << player->getResources().getElixir();
    }

    // Afficher l'état de l'hôtel de ville
    bool hasTownHall = false;
    for (Building* building : board.getBuildings()) {
        TownHall* townHall = dynamic_cast<TownHall*>(building);
        if (townHall && !townHall->isDestroyed()) {
            hasTownHall = true;
            setCursorPosition(startX, startY + 7);
            std::cout << "TownHall: " << townHall->getHealth() << " HP";
            break;
        }
    }

    if (!hasTownHall) {
        setCursorPosition(startX, startY + 7);
        std::cout << "TownHall: DESTROYED";
    }

    // Compter les entités
    int goldMines = 0;
    int elixirCollectors = 0;
    int walls = 0;
    int raiders = 0;

    for (Building* building : board.getBuildings()) {
        if (dynamic_cast<GoldMine*>(building) && !building->isDestroyed()) goldMines++;
        if (dynamic_cast<ElixirCollector*>(building) && !building->isDestroyed()) elixirCollectors++;
        if (dynamic_cast<Wall*>(building) && !building->isDestroyed()) walls++;
    }

    for (Enemy* enemy : board.getEnemies()) {
        if (dynamic_cast<Raider*>(enemy) && enemy->isAlive()) raiders++;
    }

    setCursorPosition(startX, startY + 9);
    std::cout << "Entity counts:";
    setCursorPosition(startX, startY + 10);
    std::cout << "- Gold Mines: " << goldMines;
    setCursorPosition(startX, startY + 11);
    std::cout << "- Elixir Collectors: " << elixirCollectors;
    setCursorPosition(startX, startY + 12);
    std::cout << "- Walls: " << walls;
    setCursorPosition(startX, startY + 13);
    std::cout << "- Raiders: " << raiders;

    // Instructions
    setCursorPosition(startX, startY + 15);
    std::cout << "== COMMANDES ==";
    setCursorPosition(startX, startY + 16);
    std::cout << "Flèches: Déplacer";
    setCursorPosition(startX, startY + 17);
    std::cout << "G: Mine d'or (0g, 100e)";
    setCursorPosition(startX, startY + 18);
    std::cout << "E: Collecteur élixir (100g, 0e)";
    setCursorPosition(startX, startY + 19);
    std::cout << "W: Mur (10g, 0e)";
    setCursorPosition(startX, startY + 20);
    std::cout << "C: Collecter ressources";
    setCursorPosition(startX, startY + 21);
    std::cout << "Q: Quitter";
}

// Faire apparaître un ennemi
void spawnEnemy(Board& board) {
    int side = rand() % 4; // 0: haut, 1: droite, 2: bas, 3: gauche
    Position spawnPos;

    switch (side) {
        case 0: // Haut
            spawnPos = Position(1 + rand() % (BOARD_WIDTH - 3), 1);
            break;
        case 1: // Droite
            spawnPos = Position(BOARD_WIDTH - 2, 1 + rand() % (BOARD_HEIGHT - 3));
            break;
        case 2: // Bas
            spawnPos = Position(1 + rand() % (BOARD_WIDTH - 3), BOARD_HEIGHT - 2);
            break;
        case 3: // Gauche
            spawnPos = Position(1, 1 + rand() % (BOARD_HEIGHT - 3));
            break;
    }

    Raider* raider = new Raider(spawnPos);
    board.AddEnemy(raider);
}

// Mettre à jour les structures de ressources
void updateResourceBuildings(Board& board) {
    for (Building* building : board.getBuildings()) {
        ResourceGenerator* resourceGen = dynamic_cast<ResourceGenerator*>(building);
        if (resourceGen) {
            resourceGen->Update();
        }
    }
}

// Fonction principale
int main() {
    // Configuration pour l'affichage des emojis
    setupConsole();

    // Initialisation du générateur aléatoire
    srand(static_cast<unsigned int>(time(nullptr)));

    // Initialisation du jeu
    Board board(BOARD_WIDTH, BOARD_HEIGHT);

    // Placer le joueur au centre
    Player* player = new Player(Position(BOARD_WIDTH / 2, BOARD_HEIGHT / 2 + 5));
    board.setPlayer(player);

    // Initialiser le plateau avec quelques bâtiments
    initializeBoard(board);

    // Variables de jeu
    bool running = true;
    int score = 0;
    int tickCounter = 0;

    // Cache le curseur pendant le jeu
    hideCursor();

    // Boucle principale du jeu
    while (running && !board.isGameOver()) {
        // Effacer l'écran et dessiner les bordures
        clearScreen();
        drawBorder(TOTAL_WIDTH, BOARD_HEIGHT);

        // Mettre à jour les générateurs de ressources
        updateResourceBuildings(board);

        // Traiter les entrées utilisateur (non-bloquant)
        if (_kbhit()) {
            char key = _getch();

            switch (key) {
                case 'q':
                case 'Q':
                    running = false;
                    break;

                case 'g':
                case 'G':
                    // Construire une mine d'or
                    if (player->getResources().canAfford(0, 100)) {
                        GoldMine* goldMine = new GoldMine(player->getPosition());
                        if (board.AddBuilding(goldMine)) {
                            player->getResources().spendElixir(100);
                            score += 50;
                        } else {
                            delete goldMine;
                        }
                    }
                    break;

                case 'e':
                case 'E':
                    // Construire un collecteur d'élixir
                    if (player->getResources().canAfford(100, 0)) {
                        ElixirCollector* elixirCollector = new ElixirCollector(player->getPosition());
                        if (board.AddBuilding(elixirCollector)) {
                            player->getResources().spendGold(100);
                            score += 50;
                        } else {
                            delete elixirCollector;
                        }
                    }
                    break;

                case 'w':
                case 'W':
                    // Construire un mur
                    if (player->getResources().canAfford(10, 0)) {
                        Wall* wall = new Wall(player->getPosition());
                        if (board.AddBuilding(wall)) {
                            player->getResources().spendGold(10);
                            score += 5;
                        } else {
                            delete wall;
                        }
                    }
                    break;

                case 'c':
                case 'C':
                    // Collecter des ressources
                    player->collectResources(board);
                    break;

                case -32: // Code pour les touches flèches sur Windows
                case 224: // Code alternatif pour les touches flèches
                {
                    char arrowKey = _getch();
                    // Déplacer le joueur selon la flèche pressée
                    switch (arrowKey) {
                        case 72: // Flèche haut
                            player->moving(board, 0, -1);
                            break;
                        case 80: // Flèche bas
                            player->moving(board, 0, 1);
                            break;
                        case 75: // Flèche gauche
                            player->moving(board, -1, 0);
                            break;
                        case 77: // Flèche droite
                            player->moving(board, 1, 0);
                            break;
                    }
                    break;
                }
            }
        }

        // Mettre à jour le jeu
        board.Update();

        // Faire apparaître un ennemi selon l'intervalle défini
        if (++tickCounter >= SPAWN_ENEMY_INTERVAL) {
            spawnEnemy(board);
            tickCounter = 0;
        }

        // Afficher le plateau et le panneau d'informations
        renderBoard(board);
        renderInfoPanel(board, score);

        // Pause pour éviter une utilisation trop intensive du CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(UPDATE_INTERVAL_MS));
    }

    // Fin du jeu, afficher un message
    clearScreen();
    setCursorPosition(BOARD_WIDTH / 2 - 10, BOARD_HEIGHT / 2);

    if (board.isGameOver()) {
        std::cout << "GAME OVER! Votre score final: " << score;
    } else {
        std::cout << "Jeu terminé. Votre score: " << score;
    }

    setCursorPosition(BOARD_WIDTH / 2 - 15, BOARD_HEIGHT / 2 + 2);
    std::cout << "Appuyez sur une touche pour quitter...";

    // Réafficher le curseur
    showCursor();

    // Attendre une touche avant de quitter
    _getch();

    // Nettoyage des ressources
    delete player;

    return 0;
}
