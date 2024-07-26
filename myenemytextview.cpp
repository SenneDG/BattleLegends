#include "myenemytextview.h"
#include "myxenemytextview.h"
#include "mypenemytextview.h"

MyEnemyTextview::MyEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance)
    :enemyInstance(enemyInstance),
    displayString("E"),
    color(Qt::black)
{
    //Check if enemy is already dead (switching between models)
    if(enemyInstance -> getDefeated() )
    {
        displayString = "X";
        color = Qt::red;
    }

    Enemy::connect(enemyInstance.get(), &Enemy::dead, this, &MyEnemyTextview::handleEnemyDead);
}

void MyEnemyTextview::handleEnemyDead()
{
    displayString = "X";// Display 'X' for dead enemies
    color = QColor(Qt::red);
}


//factory method to instantiate enenmytextviews
std::shared_ptr<MyEnemyTextview> MyEnemyTextview::createEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance)
{
    if (std::shared_ptr<PEnemy> pEnemy = std::dynamic_pointer_cast<PEnemy>(enemyInstance))
    {
        return std::make_shared<PEnemyTextview>(pEnemy);
    }
    else if (std::shared_ptr<MyXEnemy> xEnemy = std::dynamic_pointer_cast<MyXEnemy>(enemyInstance))
    {
        return std::make_shared<MyXEnemyTextview>(xEnemy);
    }
    else
    {
        // Add more cases for other enemy types as needed
        return std::make_shared<MyEnemyTextview>(enemyInstance);
    }
}
