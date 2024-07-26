#include "mypenemygraphicview.h"

MyPEnemyGraphicview::MyPEnemyGraphicview(std::shared_ptr<Enemy> enemy, int gridWidth)
    : MyEnemyGraphicview(enemy,gridWidth)
{
    pEnemy = std::make_shared<QPixmap>();
    pEnemyDead = std::make_shared<QPixmap>();
    poisoned = false;
    loadPic();
}

std::shared_ptr<QPixmap> MyPEnemyGraphicview::returnStatusEnemy(const std::shared_ptr<Enemy>& enemy){
    if (enemy->getDefeated()==false){
//        return std::move(pEnemy);
        return pEnemy;
    }
    else{
//        return std::move(pEnemyDead);
        return pEnemyDead;
    }
}


void MyPEnemyGraphicview::loadPic()
{
    QPixmap PenemyQP(":/images/images/poisonenemy.png");
    pEnemy =  std::make_shared<QPixmap>(PenemyQP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap PenemydeadQP(":/images/images/Dead.png");
    pEnemyDead =  std::make_shared<QPixmap>(PenemydeadQP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
}

void MyPEnemyGraphicview::setPoisoned(){
    std::cout << "start poisoning" << std::endl;
    poisoned = true;

}
