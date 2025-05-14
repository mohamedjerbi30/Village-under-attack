#ifndef GOLD_MINE_H
#define GOLD_MINE_H

#include "ResourceGenerator.h"

class GoldMine : public ResourceGenerator {
public:
    GoldMine();
    GoldMine(Position position);
};

#endif // GOLD_MINE_H
