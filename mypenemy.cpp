#include "mypenemy.h"

MyPEnemy::MyPEnemy(int xPos, int yPos, float strength)
    : PEnemy(xPos, yPos, strength), MyEnemy(xPos, yPos, strength)
{

}

void MyPEnemy::attack()
{
    emit enemyDidDamage(PEnemy::value / 10);
    MyEnemy::setDefeated(true);
    this->poison();
}
