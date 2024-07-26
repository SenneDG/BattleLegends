#ifndef MYENEMY_H
#define MYENEMY_H

#include "world.h"

class MyEnemy : public Enemy
{
    Q_OBJECT

public:
    MyEnemy(int xPosition, int yPosition, float strength);

    virtual void attack();

signals:
    void enemyDidDamage(const float& damage);
};

#endif // MYENEMY_H
