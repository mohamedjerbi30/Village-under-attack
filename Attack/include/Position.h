#ifndef POSITION_H
#define POSITION_H
#include<cmath>
class Position
{   public:
    int X;
    int Y;
    public:
        Position(int x=0,int y=0);
        virtual ~Position();
        friend bool operator==(const Position& p1, const Position& p2);
        friend bool operator!=(const Position& p1, const Position& p2);
        friend Position operator+(const Position& p1, const Position& p2);
        int manhattanDistance(const Position& other) const;

};
#endif // POSITION_H

