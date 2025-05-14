#ifndef ELIXIR_COLLECTOR_H
#define ELIXIR_COLLECTOR_H

#include "ResourceGenerator.h"

class ElixirCollector : public ResourceGenerator {
public:
    ElixirCollector();
    ElixirCollector(Position position);
};

#endif // ELIXIR_COLLECTOR_H
