#include "myxenemytextview.h"

MyXEnemyTextview::MyXEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance)
    : MyEnemyTextview(enemyInstance)
{
    // Additional initialization specific to PEnemy
    displayString = "Xe";
    color.setNamedColor("purple");
}

