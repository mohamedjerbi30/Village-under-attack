// TownHall.h
#ifndef TOWNHALL_H
#define TOWNHALL_H

#include "Resources.h"  // Assurez-vous que ce fichier est correct
#include "Building.h"   // Inclure Building.h avant TownHall, car TownHall h�rite de Building

class TownHall : public Building {
public:
    TownHall(Position p);  // Constructeur avec un param�tre Position
    virtual ~TownHall();
    Position getPosition();   // Destructeur virtuel
};

#endif // TOWNHALL_H

