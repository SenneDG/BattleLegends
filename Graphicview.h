#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QImage>
#include "controller.h"
#include "world.h"
#include "myxenemy.h"
#include "myhealthpack.h"
#include "myhealthpackgraphicview.h"
#include "myenemygraphicview.h"
#include "mypenemygraphicview.h"
#include "levelmanager.h"


class Graphicview : public QGraphicsView
{
public:
    explicit Graphicview(std::shared_ptr<LevelManager> levelManager);
    void loadTileG();
    void clearPath();
    void drawPath();
    void mousePressEvent(QMouseEvent *event);
    void addColorBasedOnGrayscale(QGraphicsPixmapItem* pixmapItem, const QImage& originalImage);
    void loadMap();
    std::shared_ptr<QPixmap> tile1;
    std::shared_ptr<QPixmap> tile2;
    std::shared_ptr<QPixmap> tile3;
    std::shared_ptr<QPixmap> Poison;
    std::shared_ptr<QPixmap> doorUp;
    std::shared_ptr<QPixmap> doorDown;
    int gridWidth;
    int gridHeight;
    int status;

    void setGridWidth(int newGridWidth){gridWidth = newGridWidth;}
    void setGridHeight(int newGridHeight){gridHeight = newGridHeight;}

    void updateProtagonist();
    void loadHealthpack();
    void updateHealthpack();
    void updateEnemy();
    void handleModelChanged(std::shared_ptr<GameModel> newModel);
    void drawScene();
    void loadScene();
    QColor lerpColor(const QColor& color1, const QColor& color2, float t);

    void loadEnemy();
    std::shared_ptr<QGraphicsScene> currentScene;
    std::shared_ptr<QGraphicsView> view;

public slots:
    void handleProtagonistPositionChanged();
    void handleEnemyDead();
    void handleHealthpackConsumed();
    void PoisonedTilesUpdated(const std::vector<std::pair<int, int>>& poisonedTilePositions);
    void keyPressEvent(QKeyEvent *event);

private:
    std::shared_ptr<Controller> controller;
    std::shared_ptr<GameModel> gameModel;
    std::vector<std::shared_ptr<MyTile>> tiles;
    std::shared_ptr<Protagonist> protagonist;
    std::shared_ptr<MyXEnemy> XEnemy;
    std::shared_ptr<QGraphicsPixmapItem> ProtItem;
    std::shared_ptr<LevelManager> levelManager;
    std::map<std::shared_ptr<GameModel>, std::shared_ptr<QGraphicsScene>> modelSceneMap;

    QPixmap marioPixmap;
    QPixmap healthpackmap;
    QPixmap enemymap;
    int numCols;
    int numRows;

    std::vector<std::vector<std::pair<int, int>>> poisonedTilePositionsVec;
    std::vector<std::map<std::pair<int, int>, QGraphicsPixmapItem*>> poisonedTileItemsVec;
};

#endif // GRAPHICVIEW_H
