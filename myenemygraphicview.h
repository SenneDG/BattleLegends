#ifndef MYENEMYGRAPHICVIEW_H
#define MYENEMYGRAPHICVIEW_H

#include <QGraphicsPixmapItem>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QImage>
#include <QObject>
#include "world.h"
#include <iostream>


class MyEnemyGraphicview : public QObject
{
    Q_OBJECT
public:
    explicit MyEnemyGraphicview(std::shared_ptr<Enemy> enemy, int gridWidth);
    void loadPic();


    virtual std::shared_ptr<QPixmap> returnStatusEnemy(const std::shared_ptr<Enemy>& enemy);

protected:
    const std::shared_ptr<Enemy> enemy;
    int gridWidth;
    std::shared_ptr<QPixmap> enemy1;
    std::shared_ptr<QPixmap> enemy2;
    std::shared_ptr<QPixmap> enemy3;
    std::shared_ptr<QPixmap> enemyDead;
};
#endif // MYENEMYGRAPHICVIEW_H
