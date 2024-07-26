#include "myhealthpack.h"

MyHealthpack::MyHealthpack(const std::shared_ptr<Tile> baseTile)
    :Tile(baseTile->getXPos(), baseTile->getYPos(), baseTile->getValue()), consumed(false)
{
    const int minHealingPower = 20;
    const int maxHealingPower = 50;

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Generate a random value between 10 and 20 for healingPower
    int randomValue = std::rand() % (maxHealingPower - minHealingPower + 1) + minHealingPower;
    setHealingPower(randomValue);
}

void MyHealthpack::setConsumed()
{
   consumed = true;
   emit consumedChanged();
}

