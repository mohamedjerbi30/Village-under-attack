#ifndef GOLDMINE_H
#define GOLDMINE_H
#include "Resources.h"

#include <ResourceGenerator.h>


class GoldMine : public ResourceGenerator
{
    public:
        GoldMine(Position p);
        virtual ~GoldMine();
        Resources Collect();
        const char* getRepr() const override;
};

#endif // GOLDMINE_H

