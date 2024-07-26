#ifndef PATHFINDINGVIEW_H
#define PATHFINDINGVIEW_H

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QImage>
#include <QWheelEvent>
#include "controller.h"
#include "world.h"
#include "myxenemy.h"
#include "myhealthpack.h"
#include "myhealthpackgraphicview.h"
#include "myenemygraphicview.h"
#include "mypenemygraphicview.h"
#include "levelmanager.h"

class Pathfindingview : public QGraphicsView
{
public:
    explicit Pathfindingview(std::shared_ptr<LevelManager> levelManager);
    void loadMap();
    void loadScene();
    void clearPath();
    void drawPath();
    void mousePressEvent(QMouseEvent *event);

    int gridWidth;
    int gridHeight;

    void setGridWidth(int newGridWidth){gridWidth = newGridWidth;}
    void setGridHeight(int newGridHeight){gridHeight = newGridHeight;}

    void updateProtagonist();
    void loadHealthpack();
    void updateHealthpack();
    void updateEnemy();
    void handleModelChanged(std::shared_ptr<GameModel> newModel);

    void loadEnemy();

    std::shared_ptr<QGraphicsScene> currentScene;
    std::shared_ptr<QGraphicsView> view;

    void wheelEvent(QWheelEvent* event) override;

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
    static const int PathItemType = QGraphicsRectItem::UserType + 1;

    QPixmap marioPixmap;
    QPixmap healthpackmap;
    QPixmap enemymap;
    int numCols;
    int numRows;

    std::vector<std::vector<std::pair<int, int>>> poisonedTilePositionsVec;
    std::vector<std::map<std::pair<int, int>, QGraphicsPixmapItem*>> poisonedTileItemsVec;
};

#endif // PATHFINDINGVIEW_H
