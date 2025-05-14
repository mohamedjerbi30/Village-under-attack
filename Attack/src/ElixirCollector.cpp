#include "ElixirCollector.h"

ElixirCollector::ElixirCollector()
    : ResourceGenerator(Position(0, 0), 3, 3, L"🧪", L"🧬",
                         100, 0, 3, 200, 100, 5) {}

ElixirCollector::ElixirCollector(Position position)
    : ResourceGenerator(position, 3, 3, L"🧪", L"🧬",
                         100, 0, 3, 200, 100, 5) {}
