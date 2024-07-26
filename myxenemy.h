#ifndef MYXENEMY_H
#define MYXENEMY_H

#include <QTimer>
#include <QObject>
#include <QRandomGenerator>
#include <iostream>

#include "world.h"
#include "myenemy.h"

class MyXEnemy : public MyEnemy
{
    Q_OBJECT

public:
    MyXEnemy(int xPos, int yPos, float strength);

    int getRange() const { return range; }
    bool getIsAttacking() const { return isAttacking; }

    void attack() override;
    void rangedAttack(const int& xPos, const int& yPos);
    void updateAttackStatus(const int& xPos, const int& yPos);

signals:
    void shooted();

private slots:
    void shoot();

private:
    static const unsigned int range = 2;
    bool isAttacking = false;
    QTimer attackTimer;
};

#endif // MYXENEMY_H
