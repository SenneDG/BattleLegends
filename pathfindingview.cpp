#include "pathfindingview.h"
#include <QImage>
#include <QString>
#include <iostream>

Pathfindingview::Pathfindingview(std::shared_ptr<LevelManager> levelManager)
    :controller(levelManager->getController()),
    levelManager(levelManager)
{
    gameModel = levelManager -> getCurrentGameModel();
    modelSceneMap = std::map<std::shared_ptr<GameModel>, std::shared_ptr<QGraphicsScene>>();

    currentScene = std::make_shared<QGraphicsScene>();
    view = std::make_shared<QGraphicsView>();
    ProtItem = std::make_shared<QGraphicsPixmapItem>();
    modelSceneMap[gameModel] = currentScene;
    loadMap();
}

void Pathfindingview::loadMap()
{
    gameModel = levelManager -> getCurrentGameModel();
    tiles = gameModel->getTiles();
    protagonist = gameModel->getProtagonist();

    connect(gameModel->getProtagonist().get(), &MyProtagonist::posChanged, this, &Pathfindingview::handleProtagonistPositionChanged);
    connect(levelManager.get(), &LevelManager::modelChanged, this, &Pathfindingview::handleModelChanged);
    connect(gameModel.get(), &GameModel::poisonedTilesUpdated, this, &Pathfindingview::PoisonedTilesUpdated);

    auto enemies = gameModel->getEnemies();
    for (const auto& enemy : enemies) {
        connect(enemy.get(), &MyEnemy::dead, this, &Pathfindingview::handleEnemyDead);
    }

    auto healthpacks = gameModel->getHealthpacks();
    for (const auto& healthpack : healthpacks) {
        connect(healthpack.get(), &MyHealthpack::consumedChanged, this, &Pathfindingview::handleHealthpackConsumed);
    }

    numCols = gameModel->getWorld()->getCols();
    numRows = gameModel->getWorld()->getRows();

    loadScene();
    loadEnemy();
    loadHealthpack();
    updateProtagonist();
    setScene(currentScene.get());
}

/*
 * This method will load the full map with smaller tile sizes.
 * But can be used to visualize the pathfinding in a better way
 * Protagonist, healthpacks and enemies will be very small
 */
void Pathfindingview::loadScene()
{
    setGridWidth(1);
    setGridHeight(1);

    QImage originalImage(levelManager->getFileName());
    QPixmap pixelatedPixmap = QPixmap::fromImage(originalImage);
    QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(pixelatedPixmap);
    currentScene->addItem(pixmapItem);

    std::pair<int,int> upDoor = gameModel->getUpDoor();
    std::pair<int,int> downDoor = gameModel->getDownDoor();

    if (levelManager->getCurrentLevel() != levelManager->getNrOfLevels() - 1) {
        QGraphicsRectItem* doorItem = new QGraphicsRectItem();
        doorItem->setRect(upDoor.first * gridWidth, upDoor.second * gridHeight, gridWidth, gridHeight);
        doorItem->setBrush(QBrush(QColor(128, 0, 128)));
        doorItem->setPen(Qt::NoPen);
        currentScene->addItem(doorItem);
    }

    if (levelManager->getCurrentLevel() != 0) {
        QGraphicsRectItem* doorItem2 = new QGraphicsRectItem();
        doorItem2->setRect(downDoor.first * gridWidth, downDoor.second * gridHeight, gridWidth, gridHeight);
        doorItem2->setBrush(QBrush(QColor(255, 182, 193)));
        doorItem2->setPen(Qt::NoPen);
        currentScene->addItem(doorItem2);
    }
}

void Pathfindingview::updateProtagonist()
{
    if (currentScene->items().contains(ProtItem.get()))
    {
        ProtItem->setPos(protagonist->getXPos() * gridWidth, protagonist->getYPos() * gridHeight);
    }
    else
    {
        ProtItem = std::make_shared<QGraphicsPixmapItem>();
        ProtItem->setPos(protagonist->getXPos() * gridWidth, protagonist->getYPos() * gridHeight);
        ProtItem->setZValue(3);

        QPixmap blackPixmap(gridWidth, gridHeight);
        blackPixmap.fill(Qt::red);

        ProtItem->setPixmap(blackPixmap);
        currentScene->addItem(ProtItem.get());
    }

    int centerPosX = protagonist->getXPos() * gridWidth + gridWidth / 2;
    int centerPosY = protagonist->getYPos() * gridHeight + gridHeight / 2;
    this->centerOn(centerPosX, centerPosY);
}

void Pathfindingview::loadHealthpack()
{
    for (auto& healthPack : gameModel->getHealthpacks()) {
        QGraphicsRectItem* healthpackItem = new QGraphicsRectItem();

        healthpackItem->setRect(0, 0, gridWidth, gridHeight);

        if (!healthPack->getConsumed()) {
            healthpackItem->setBrush(QBrush(Qt::green));
        } else {
            healthpackItem->setBrush(QBrush(Qt::gray));
        }

        healthpackItem->setPen(QPen(Qt::transparent));
        healthpackItem->setPos(healthPack->getXPos() * gridWidth, healthPack->getYPos() * gridHeight);
        healthpackItem->setZValue(2);
        currentScene->addItem(healthpackItem);
    }
}

void Pathfindingview::updateHealthpack()
{
    for (auto& healthPack : gameModel->getHealthpacks())
    {
        if (healthPack->getXPos() == protagonist->getXPos() && healthPack->getYPos() == protagonist->getYPos())
        {
            QGraphicsRectItem* healthpackItem = new QGraphicsRectItem();

            healthpackItem->setRect(0, 0, gridWidth, gridHeight);

            if (!healthPack->getConsumed()) {
                healthpackItem->setBrush(QBrush(Qt::green));
            }
            else
            {
                healthpackItem->setBrush(QBrush(Qt::gray));
            }

            QPen transparentPen(Qt::transparent);
            healthpackItem->setPen(transparentPen);
            healthpackItem->setPos(healthPack->getXPos() * gridWidth, healthPack->getYPos() * gridHeight);
            healthpackItem->setZValue(2);
            currentScene->addItem(healthpackItem);
        }
    }
}

void Pathfindingview::loadEnemy()
{
    for (auto& enemy : gameModel->getEnemies()) {
        QGraphicsRectItem* enemyItem = new QGraphicsRectItem();

        enemyItem->setRect(0, 0, gridWidth, gridHeight);

        if (enemy->getDefeated()) {
            enemyItem->setBrush(QBrush(QColor(128, 128, 128)));
        } else if (std::shared_ptr<MyXEnemy> xEnemy = std::dynamic_pointer_cast<MyXEnemy>(enemy)) {
            enemyItem->setBrush(QBrush(QColor(0, 0, 255, 128)));
        } else if (std::shared_ptr<MyPEnemy> pEnemy = std::dynamic_pointer_cast<MyPEnemy>(enemy)) {
            enemyItem->setBrush(QBrush(QColor(255, 165, 0, 128)));
        } else if (std::shared_ptr<MyEnemy> regularEnemy = std::dynamic_pointer_cast<MyEnemy>(enemy)) {
            enemyItem->setBrush(QBrush(QColor(255, 255, 0, 128)));
        }

        enemyItem->setPen(Qt::NoPen);
        enemyItem->setPos(enemy->getXPos() * gridWidth, enemy->getYPos() * gridHeight);
        enemyItem->setZValue(2);
        currentScene->addItem(enemyItem);
    }
}

void Pathfindingview::updateEnemy()
{
    for (auto& enemy : gameModel->getEnemies()) {
        QGraphicsRectItem* enemyItem = new QGraphicsRectItem();

        enemyItem->setRect(0, 0, gridWidth, gridHeight);

        if (enemy->getDefeated()) {
            enemyItem->setBrush(QBrush(QColor(128, 128, 128)));
        } else if (std::shared_ptr<MyXEnemy> xEnemy = std::dynamic_pointer_cast<MyXEnemy>(enemy)) {
            enemyItem->setBrush(QBrush(QColor(0, 0, 255, 128)));
        } else if (std::shared_ptr<MyPEnemy> pEnemy = std::dynamic_pointer_cast<MyPEnemy>(enemy)) {
            enemyItem->setBrush(QBrush(QColor(255, 165, 0, 128)));
        } else if (std::shared_ptr<MyEnemy> regularEnemy = std::dynamic_pointer_cast<MyEnemy>(enemy)) {
            enemyItem->setBrush(QBrush(QColor(255, 255, 0, 128)));
        }

        enemyItem->setPen(Qt::NoPen);
        enemyItem->setPos(enemy->getXPos() * gridWidth, enemy->getYPos() * gridHeight);
        enemyItem->setZValue(2);
        currentScene->addItem(enemyItem);
    }
}

void Pathfindingview::PoisonedTilesUpdated(const std::vector<std::pair<int, int>>& poisonedTilePositions)
{
    // Remove poison items associated with tiles that are no longer poisoned
    for (auto& poisonItems : poisonedTileItemsVec) {
        for (auto it = poisonItems.begin(); it != poisonItems.end();) {
            if (std::find(poisonedTilePositions.begin(), poisonedTilePositions.end(), it->first) == poisonedTilePositions.end()) {
                currentScene->removeItem(it->second);
                delete it->second;
                it = poisonItems.erase(it);
            } else {
                ++it;
            }
        }
    }

    poisonedTilePositionsVec.push_back(poisonedTilePositions);
    std::map<std::pair<int,int>, QGraphicsPixmapItem*> poisonItems;

    for (auto& pos : poisonedTilePositions) {
        if (std::find(poisonedTilePositionsVec.back().begin(), poisonedTilePositionsVec.back().end(), pos) == poisonedTilePositionsVec.back().end()) {
            QGraphicsRectItem* poisonItem = new QGraphicsRectItem();
            poisonItem->setRect(pos.first * gridWidth, pos.second * gridHeight, gridWidth, gridHeight);
            poisonItem->setBrush(Qt::green);
            poisonItem->setZValue(1);
            currentScene->addItem(poisonItem);
        }
    }

    poisonedTileItemsVec.push_back(poisonItems);
    update();
}

void Pathfindingview::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
    controller->handleKeyPress(event);

    // Clear the existing path
    clearPath();
    updateProtagonist();
}

void Pathfindingview::handleProtagonistPositionChanged()
{
    // Clear the existing path
    clearPath();

    // Draw the new path
    drawPath();
    updateProtagonist();
}

void Pathfindingview::handleEnemyDead()
{
    updateEnemy();
}

void Pathfindingview::handleHealthpackConsumed()
{
    updateHealthpack();
}

void Pathfindingview::handleModelChanged(std::shared_ptr<GameModel> newModel)
{
    updateProtagonist();
    auto it = modelSceneMap.find(newModel);
    if (it != modelSceneMap.end())
    {
        currentScene = it->second;
    }
    else
    {
        std::shared_ptr<QGraphicsScene> scene = std::make_shared<QGraphicsScene>();
        modelSceneMap[newModel] = scene;
        currentScene = modelSceneMap[newModel];
    }
    std::cout << "Map size: " << modelSceneMap.size() << std::endl;
    gameModel = newModel;

    setScene(currentScene.get());
    loadMap();
}

void Pathfindingview::clearPath()
{
    QList<QGraphicsItem*> items = currentScene->items();
    for (QGraphicsItem* item : items)
    {
        if (item->data(Qt::UserRole) == PathItemType) {
            currentScene->removeItem(item);
            delete item;
        }
    }
}

void Pathfindingview::drawPath()
{
    if (gameModel) {
        const std::vector<std::pair<int, int>>& path = gameModel->getPath();
        QBrush pathBrush(Qt::red);
        QPen transparentPen(Qt::transparent);

        for (const auto& position : path) {
            int x = position.first * gridWidth;
            int y = position.second * gridHeight;

            QGraphicsRectItem* pathRect = new QGraphicsRectItem(x, y, gridWidth, gridHeight);
            pathRect->setBrush(pathBrush);
            pathRect->setPen(transparentPen);
            pathRect->setData(Qt::UserRole, PathItemType);

            currentScene->addItem(pathRect);
        }
    }
}

void Pathfindingview::mousePressEvent(QMouseEvent *event)
{
    // Map the mouse click coordinates to the scene coordinates
    QPointF scenePos = mapToScene(event->pos());

    // Check if the scene contains the mapped position
    if (currentScene->sceneRect().contains(scenePos))
    {
        // Calculate the grid position based on the mapped scene coordinates
        int clickedCol = scenePos.x() / gridWidth;
        int clickedRow = scenePos.y() / gridHeight;

        gameModel->gotoNewPosition(clickedCol, clickedRow);
    }
    else
    {
        qDebug() << "Mouse click outside the scene boundaries";
    }
}

void Pathfindingview::wheelEvent(QWheelEvent* event)
{
    setTransformationAnchor(AnchorUnderMouse);
    int delta = event->angleDelta().y();
    double scaleFactor = 1.5;

    // Zoom in
    if (delta > 0) {
        scale(scaleFactor, scaleFactor);
    }
    // Zoom out
    else if (delta < 0) {
        scale(1/scaleFactor, 1/scaleFactor);
    }
}
