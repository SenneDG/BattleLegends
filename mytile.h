#ifndef MYTILE_H
#define MYTILE_H

#include "world.h"

class MyTile : public Tile
{
public:
    MyTile(int xPos, int yPos, float tileWeight);

    float f = 0.0f;
    float g = 0.0f;
    float h = 0.0f;
    bool visited = false;
    MyTile* prev = nullptr;

    const float& getPoisonLevel() const { return poisonLevel;}
    void setPoisonLevel(const float& newPoisonLevel) { poisonLevel = newPoisonLevel;}

private:
    float poisonLevel;
};

#endif // MYTILE_H
