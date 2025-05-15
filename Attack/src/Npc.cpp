#include "Npc.h"

Npc::Npc(Position position, int initialHealth, const char* emoji)
    : Entity(position, emoji), health(initialHealth) {}


void Npc::loseHealth(int amount) {
    health = (health - amount > 0) ? health - amount : 0;
}

void Npc::moveTowards(const Board& board, Position destination) {
    Position currentPos = getPosition();
    int dx = 0, dy = 0;

    if (currentPos.X < destination.X) dx = 1;
    else if (currentPos.X > destination.X) dx = -1;

    if (currentPos.Y < destination.Y) dy = 1;
    else if (currentPos.Y > destination.Y) dy = -1;

    moving(board, dx, dy);
}

void Npc::Update(const Board& board) {

    moveTowards(board, Position(0, 0));
}

Npc::~Npc()
{
    //dtor
}
