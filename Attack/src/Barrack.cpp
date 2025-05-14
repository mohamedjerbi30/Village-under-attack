#include "Barrack.h"
#include "Board.h"

Barrack::Barrack()
    : Building(Position(0, 0), 5, 3, "🏕️", 200, 0, 1, 10),
      entities(), trainingQueue(), capacity(5) {}

Barrack::Barrack(Position position)
    : Building(position, 5, 3, "🏕️", 200, 0, 1, 10),
      entities(), trainingQueue(), capacity(5) {}

Barrack::~Barrack() {
    // Clean up any troops in the training queue
    while (!trainingQueue.empty()) {
        delete trainingQueue.front();
        trainingQueue.pop();
    }

    // Note: We don't delete entities as they are managed by the Board class
}

int Barrack::getCapacity() const {
    return capacity;
}

const std::vector<Troop*>& Barrack::getEntities() const {
    return entities;
}

bool Barrack::Train(Troop* troop) {
    if (trainingQueue.size() < capacity) {
        trainingQueue.push(troop);
        return true;
    }
    return false;
}

void Barrack::Update(Board& board) {
    if (!isDestroyed() && !trainingQueue.empty()) {
        // Train the next troop in the queue
        Troop* troop = trainingQueue.front();
        trainingQueue.pop();

        // Position the troop near the barrack
        Position troopPos = position;
        troopPos.X += 1; // Place to the right of the barrack

        troop->setPosition(troopPos);
        entities.push_back(troop);

        // Board will handle adding the troop to the game
        // This is handled in the main game loop
    }
}
