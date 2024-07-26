#include "myenemy.h"

MyEnemy::MyEnemy(int xPosition, int yPosition, float strength)
    : Enemy(xPosition, yPosition, strength)
{
}

void MyEnemy::attack()
{
    emit enemyDidDamage(value / 10);
    setDefeated(true);
}
