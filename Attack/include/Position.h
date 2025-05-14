#ifndef POSITION_H
#define POSITION_H

class Position {
public:
    int X;
    int Y;

    Position();
    Position(int x, int y);

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};

#endif // POSITION_H
