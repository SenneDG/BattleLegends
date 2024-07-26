// PEnemyTextview.cpp
#include "mypenemytextview.h"

PEnemyTextview::PEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance)
    : MyEnemyTextview(enemyInstance)
{
    // Additional initialization specific to PEnemy
    displayString = "Pe";
    color.setNamedColor("magenta");
}

