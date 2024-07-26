#include "myxenemy.h"

MyXEnemy::MyXEnemy(int xPos, int yPos, float strength) :
    MyEnemy(xPos, yPos, strength)
{
    connect(&attackTimer, SIGNAL(timeout()), this, SLOT(shoot()));
}

void MyXEnemy::shoot()
{
    if (isAttacking)
    {
        std::cout << "XEnemy shooted" << std::endl;
        emit enemyDidDamage(2.0f);
    }
}

void MyXEnemy::rangedAttack(const int& xPos, const int& yPos)
{
    const unsigned int distance = std::abs(xPos - getXPos()) + std::abs(yPos - getYPos());

    if (distance <= range && !getDefeated())
    {
        isAttacking = true;
        attackTimer.start(3000);
    } else
    {
        isAttacking = false;
        attackTimer.stop();
    }
}

void MyXEnemy::attack()
{
    emit enemyDidDamage(value / 10);
    isAttacking = false;
    setDefeated(true);
}

void MyXEnemy::updateAttackStatus(const int& xPos, const int& yPos)
{
    if (!getIsAttacking())
    {
        rangedAttack(xPos, yPos);
    } else
    {
        const unsigned int distance = std::abs(xPos - getXPos()) + std::abs(yPos - getYPos());
        if (distance > range || getDefeated())
        {
            std::cout << "protagonist not in range" << std::endl;
            isAttacking = false;
        }
    }
}
