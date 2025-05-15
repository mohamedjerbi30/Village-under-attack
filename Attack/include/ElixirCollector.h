#ifndef ELIXIRCOLLECTOR_H
#define ELIXIRCOLLECTOR_H
#include "Resources.h"
#include <ResourceGenerator.h>


class ElixirCollector : public ResourceGenerator
{
    public:
        ElixirCollector(Position p);
        virtual ~ElixirCollector();
        Resources Collect();
        virtual const char* getRepr() const override;
};

#endif // ELIXIRCOLLECTOR_H

