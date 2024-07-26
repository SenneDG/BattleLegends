#ifndef MYPENEMY_H
#define MYPENEMY_H

#include "world.h"
#include "myenemy.h"

class MyPEnemy : public PEnemy, public MyEnemy
{
public:
    MyPEnemy(int xPos, int yPos, float strength);

    void attack() override;
};

#endif // MYPENEMY_H
