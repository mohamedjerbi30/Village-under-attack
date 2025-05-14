#ifndef ENTITY_H
#define ENTITY_H

#include "Position.h"
#include <string>

class Entity {
protected:
    Position position;
    std::wstring repr; // Utilisation de wstring pour les emojis UTF-8

public:
    Entity();
    Entity(Position position, std::wstring repr);
    virtual ~Entity() = default;

    Position getPosition() const;
    void setPosition(const Position& pos);

    std::wstring getRepr() const;

    virtual void Update() {};
};

#endif // ENTITY_H
