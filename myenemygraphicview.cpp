#include "myenemygraphicview.h"
#include "myxenemygraphicview.h"
#include "mypenemygraphicview.h"
#include "myxenemy.h"

MyEnemyGraphicview::MyEnemyGraphicview(std::shared_ptr<Enemy> enemy, int gridWidth)
    :enemy(enemy), gridWidth(gridWidth)
{
    enemy1 = std::make_shared<QPixmap>();
    enemy2 = std::make_shared<QPixmap>();
    enemy3 = std::make_shared<QPixmap>();
    enemyDead = std::make_shared<QPixmap>();
    loadPic();
}

std::shared_ptr<QPixmap> MyEnemyGraphicview::returnStatusEnemy(const std::shared_ptr<Enemy>& enemy){
    if (std::dynamic_pointer_cast<PEnemy>(enemy)){
        MyPEnemyGraphicview* Penemy = new MyPEnemyGraphicview(enemy,gridWidth);
        return Penemy->returnStatusEnemy(enemy);
    }
    else if (std::dynamic_pointer_cast<MyXEnemy>(enemy)){
        MyXEnemyGraphicview* Xenemy = new MyXEnemyGraphicview(enemy,gridWidth);
        return Xenemy->returnStatusEnemy(enemy);
    }
    else if (enemy-> getDefeated()==false){
        if(enemy->getValue()<25){
            return std::move(enemy1);
        }
        else if(enemy->getValue()<75){
            return std::move(enemy2);
        }
        else{
            return std::move(enemy3);
        }
    }
    else
        return std::move(enemyDead);

}


void MyEnemyGraphicview::loadPic(){
    QPixmap enemy1QP(":/images/images/enemy.png");
    enemy1 = std::make_shared<QPixmap>(enemy1QP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap enemy2QP(":/images/images/enemy2.png");
    enemy2 = std::make_shared<QPixmap>(enemy2QP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap enemy3QP(":/images/images/enemy3.png");
    enemy3 = std::make_shared<QPixmap>(enemy3QP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap enemydeadQP(":/images/images/Dead.png");
    enemyDead = std::make_shared<QPixmap>(enemydeadQP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));


}
