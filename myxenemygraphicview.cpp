#include "myxenemygraphicview.h"

MyXEnemyGraphicview::MyXEnemyGraphicview(std::shared_ptr<Enemy> enemy, int gridWidth)
    : MyEnemyGraphicview(enemy,gridWidth)
{
    xEnemy = std::make_shared<QPixmap>();
    xEnemyDead = std::make_shared<QPixmap>();
    loadPics();
}


std::shared_ptr<QPixmap> MyXEnemyGraphicview::returnStatusEnemy(const std::shared_ptr<Enemy>& enemy){
    if (enemy->getDefeated()==false){
//        return std::move(xEnemy);
        return xEnemy;
    }
    else{
//        return std::move(xEnemyDead);
        return xEnemyDead;
    }
}

void MyXEnemyGraphicview::loadPics()
{
    QPixmap XenemyQP(":/images/images/xenemy.png");
    xEnemy =  std::make_shared<QPixmap>(XenemyQP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap XenemydeadQP(":/images/images/Dead.png");
    xEnemyDead =  std::make_shared<QPixmap>(XenemydeadQP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
}
