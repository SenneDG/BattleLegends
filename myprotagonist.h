#ifndef MYPROTAGONIST_H
#define MYPROTAGONIST_H

#pragma once

#include <QObject>
#include <iostream>

#include "world.h"
#include "myhealthpack.h"

class MyProtagonist : public Protagonist
{
    Q_OBJECT

public:
    MyProtagonist();

    void handlePoisonLevelUpdated(float newPoisonLevel);
    void useHealthPack(const std::shared_ptr<MyHealthpack> & healthpack);
    void moveProtagonist(int xPos, int yPos);
    void setIsImmortal(bool newValue){isImmortal = newValue;}

signals:
    void gameOver();

public slots:
    void handleHealthChange(const float& damage);

private:
    float health {100.0f};
    float energy {100.0f};
    bool isImmortal = false;
};

#endif // MYPROTAGONIST_H
