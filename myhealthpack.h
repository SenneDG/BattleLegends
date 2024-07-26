#ifndef MYHEALTHPACK_H
#define MYHEALTHPACK_H

#include <cstdlib>
#include <ctime>
#include <sstream>

#include "world.h"

class MyHealthpack :  public QObject, public Tile
{
   Q_OBJECT

public:
    MyHealthpack(const std::shared_ptr<Tile> baseTile);

    int getHealingPower() const { return healingPower;}
    bool getConsumed() const { return consumed;}
    void setConsumed();

signals:
    void consumedChanged();

private:
    void setHealingPower(const int& power) {healingPower = power;}

    int healingPower;
    int minHealingPower;
    int maxHealingPower;
    bool consumed {false};
};

#endif // MYHEALTHPACK_H
