#ifndef ENTITY_H
#define ENTITY_H

#include "Position.h"
#include <string>

class Entity {
protected:
    Position position;
    const char* repr; // Changed from std::wstring to const char*

public:
    Entity();
    Entity(Position position, const char* repr);
    virtual ~Entity() = default;

    Position getPosition() const;
    void setPosition(const Position& pos);

    const char* getRepr() const; // Changed return type

    virtual void Update() {};
};

#endif // ENTITY_H
