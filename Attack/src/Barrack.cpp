#include "Barrack.h"
#include "Board.h"

Barrack::Barrack(Position position)
    : Building(position, 10, 200, 0, 5, 3, 1), capacity(5) // health, repr, costGold, costElixir, sizeX, sizeY, maxInstances
{
}

bool Barrack::Train(Troop* troop) {
    if (trainingQueue.size() >= static_cast<size_t>(capacity))
        return false;

    trainingQueue.push(troop);
    return true;
}

void Barrack::Update(Board& board) {
    if (!trainingQueue.empty()) {
        Troop* trained = trainingQueue.front();
        trainingQueue.pop();
        board.AddEntity(trained);
    }
}
