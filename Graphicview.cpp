#include "graphicview.h"
#include <QGraphicsView>
#include "levelmanager.h"
#include <QPainter>
#include "myprotagonistgraphicsview.h"

Graphicview::Graphicview(std::shared_ptr<LevelManager> levelManager)
    :controller(levelManager->getController()),
    levelManager(levelManager)
{
    gameModel = levelManager -> getCurrentGameModel();

    tile1 = std::make_shared<QPixmap>();
    tile2 = std::make_shared<QPixmap>();
    tile3 = std::make_shared<QPixmap>();
    doorUp = std::make_shared<QPixmap>();
    doorDown = std::make_shared<QPixmap>();
    modelSceneMap = std::map<std::shared_ptr<GameModel>, std::shared_ptr<QGraphicsScene>>();

    currentScene = std::make_shared<QGraphicsScene>();
    view = std::make_shared<QGraphicsView>();
    ProtItem = std::make_shared<QGraphicsPixmapItem>();
    modelSceneMap[gameModel] = currentScene;
    loadMap();
    status = 0;
}

void Graphicview::loadMap()
{
    gameModel = levelManager -> getCurrentGameModel();
    tiles = gameModel->getTiles();
    protagonist = gameModel->getProtagonist();

    connect(gameModel->getProtagonist().get(), &MyProtagonist::posChanged, this, &Graphicview::handleProtagonistPositionChanged);
    connect(levelManager.get(), &LevelManager::modelChanged, this, &Graphicview::handleModelChanged);
    connect(gameModel.get(), &GameModel::poisonedTilesUpdated, this, &Graphicview::PoisonedTilesUpdated);

    auto enemies = gameModel->getEnemies();
    for (const auto& enemy : enemies) {
        connect(enemy.get(), &MyEnemy::dead, this, &Graphicview::handleEnemyDead);
    }

    auto healthpacks = gameModel->getHealthpacks();
    for (const auto& healthpack : healthpacks) {
        connect(healthpack.get(), &MyHealthpack::consumedChanged, this, &Graphicview::handleHealthpackConsumed);
    }

    numCols = gameModel->getWorld()->getCols();
    numRows = gameModel->getWorld()->getRows();

    drawScene();
    loadEnemy();
    loadHealthpack();



    // Fit the view to the scene

    updateProtagonist();
    setScene(currentScene.get());
    int centerPosX = protagonist->getXPos() * gridWidth + gridWidth / 2;
    int centerPosY = protagonist->getYPos() * gridHeight + gridHeight / 2;
    this->centerOn(centerPosX, centerPosY);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

/*
 * This method will load only a part of the map.
 * This can be used to play the game more smoothly on big maps
 * Protagonist, healthpacks and enemies will be large
 */
void Graphicview::drawScene()
{
    setGridWidth(25);
    setGridHeight(25);

    loadTileG();

    // Set the visible area in the view's viewport
    int visibleRange = 30;
    int startX = std::max(0, protagonist->getXPos() - visibleRange);
    int endX = std::min(numCols - 1, protagonist->getXPos() + visibleRange);
    int startY = std::max(0, protagonist->getYPos() - visibleRange);
    int endY = std::min(numRows - 1, protagonist->getYPos() + visibleRange);

    // Overlay with QColors, with this it is way faster
    // Iterate through the tiles and create graphical items for each tile
    for (int row = startY; row <= endY; ++row) {
        for (int col = startX; col <= endX; ++col) {

            // Get the index in the Tiles vector based on row and col
            size_t index = row * numCols + col;

            if (index < tiles.size()) {
                int scaledGrayValue = static_cast<int>(tiles[index]->getValue() * 255.0f);
                scaledGrayValue = std::clamp(scaledGrayValue, 0, 255);

                QGraphicsRectItem* rectItem = new QGraphicsRectItem();

                // Interpolate color based on scaledGrayValue
                QColor color;

                if (scaledGrayValue <= 80) {
                    // Beach (light brown to yellow transition)
                    QColor beachColor1(244, 164, 96);
                    QColor beachColor2(255, 255, 0);
                    color = lerpColor(beachColor1, beachColor2, scaledGrayValue / 100.0);

                } else if (scaledGrayValue <= 160) {
                    // Grass (green to dark green transition)
                    QColor grassColor1(0, 128, 0);
                    QColor grassColor2(0, 64, 0);
                    color = lerpColor(grassColor1, grassColor2, (scaledGrayValue - 100) / 100.0);

                } else if (scaledGrayValue <= 240) {
                    // Stone area (gray to dark gray transition)
                    QColor stoneColor1(128, 128, 128);
                    QColor stoneColor2(64, 64, 64);
                    color = lerpColor(stoneColor1, stoneColor2, (scaledGrayValue - 200) / 40.0);

                } else if (scaledGrayValue > 240) {
                    // White
                    color = QColor(Qt::black);
                }

                rectItem->setRect(col * gridWidth, row * gridHeight, gridWidth, gridHeight);
                rectItem->setBrush(QBrush(color));
                currentScene->addItem(rectItem);
            }
        }
    }
    updateProtagonist();

    // Overlay with QPixmaps images, but this makes it very slow

//    for (int row = startY; row <= endY; ++row) {
//        for (int col = startX; col <= endX; ++col) {

//            // Get the index in the Tiles vector based on row and col
//            size_t index = row * numCols + col;

//            if (index < tiles.size()) {
//                int scaledGrayValue = static_cast<int>(tiles[index]->getValue() * 255.0f);
//                scaledGrayValue = std::clamp(scaledGrayValue, 0, 255);

//                QGraphicsPixmapItem* rectItem = new QGraphicsPixmapItem();

//                if (scaledGrayValue >= 0 && scaledGrayValue <= 100) {
//                    rectItem->setPixmap(*tile3);

//                } else if (scaledGrayValue <= 240) {
//                    rectItem->setPixmap(*tile2);

//                } else if (scaledGrayValue >= 240) {
//                    rectItem->setPixmap(*tile1);

//                }
//                rectItem->setPos(col * gridWidth, row * gridHeight);
//                currentScene->addItem(rectItem);
//            }
//        }
//    }

    int centerPosX = protagonist->getXPos() * gridWidth + gridWidth / 2;
    int centerPosY = protagonist->getYPos() * gridHeight + gridHeight / 2;
    this->centerOn(centerPosX, centerPosY);
    std::pair<int,int> upDoor = gameModel->getUpDoor();
    std::pair<int,int> downDoor = gameModel->getDownDoor();
    if (levelManager->getCurrentLevel() != levelManager->getNrOfLevels()-1)
    {
        QGraphicsPixmapItem* doorItem = new QGraphicsPixmapItem();
        doorItem->setPixmap(*doorUp);

        doorItem->setPos(upDoor.first * gridWidth + (gridWidth - doorUp->width()) / 2, upDoor.second * gridHeight + (gridHeight - doorUp->height()) / 2);
        currentScene->addItem(doorItem);
    }

    if (levelManager->getCurrentLevel()!=0)
    {
        QGraphicsPixmapItem* doorItem2 = new QGraphicsPixmapItem();
        doorItem2->setPixmap(*doorDown);
        doorItem2->setPos(downDoor.first * gridWidth + (gridWidth - doorDown->width()) / 2, downDoor.second * gridHeight + (gridHeight - doorDown->height()) / 2);
        currentScene->addItem(doorItem2);
        currentScene->addItem(doorItem2);
    }

}

QColor Graphicview::lerpColor(const QColor& color1, const QColor& color2, float t) {
    int r = color1.red() + t * (color2.red() - color1.red());
    int g = color1.green() + t * (color2.green() - color1.green());
    int b = color1.blue() + t * (color2.blue() - color1.blue());
    return QColor(r, g, b);
}

void Graphicview::updateProtagonist()
{
    QGraphicsPixmapItem* protItem= new QGraphicsPixmapItem();
    myprotagonistgraphicsview* prot = new myprotagonistgraphicsview(gridWidth);

    if (currentScene->items().contains(ProtItem.get()))
    {
        ProtItem->setPixmap(*prot->returnStatusProt(protagonist,status));
        ProtItem->setPos(protagonist->getXPos() * gridWidth, protagonist->getYPos() * gridHeight);
    } else
    {
        ProtItem = std::make_shared<QGraphicsPixmapItem>(protItem);
        ProtItem->setPos(protagonist->getXPos() * gridWidth, protagonist->getYPos() * gridHeight);
        ProtItem->setZValue(3);
        currentScene->addItem(ProtItem.get());
    }

    int centerPosX = protagonist->getXPos() * gridWidth + gridWidth / 2;
    int centerPosY = protagonist->getYPos() * gridHeight + gridHeight / 2;
    this->centerOn(centerPosX, centerPosY);
}

void Graphicview::loadHealthpack()
{
    for (auto& healthPack : gameModel->getHealthpacks()) {
        QGraphicsPixmapItem* healthpackItem = new QGraphicsPixmapItem();
        MyHealthpackGraphicview* healthpack = new MyHealthpackGraphicview(healthPack, gridWidth);

        healthpackItem->setPixmap(*healthpack->returnStatusHealthpack(healthPack));
        healthpackItem->setPos(healthPack->getXPos() * gridWidth, healthPack->getYPos() * gridHeight);

        healthpackItem->setZValue(2);
        currentScene->addItem(healthpackItem);
    }
}

void Graphicview::updateHealthpack()
{
    for (auto& healthPack : gameModel->getHealthpacks())
    {
        if (healthPack->getXPos()== protagonist->getXPos() && healthPack->getYPos()== protagonist->getYPos())
        {
            QGraphicsPixmapItem* healthpackItem = new QGraphicsPixmapItem();
            MyHealthpackGraphicview* healthpack = new MyHealthpackGraphicview(healthPack, gridWidth);

            healthpackItem->setPixmap(*healthpack->returnStatusHealthpack(healthPack));
            healthpackItem->setPos(healthPack->getXPos() * gridWidth, healthPack->getYPos() * gridHeight);
            healthpackItem->setZValue(2);
            currentScene->addItem(healthpackItem);
            status = 1;
        }
    }
}

void Graphicview::loadEnemy()
{
    for (auto& enemy : gameModel->getEnemies())
    {
        QGraphicsPixmapItem* enemyItem = new QGraphicsPixmapItem();
        MyEnemyGraphicview* enemyGraph = new MyEnemyGraphicview(enemy,gridWidth);

        enemyItem->setPixmap(*enemyGraph->returnStatusEnemy(enemy));
        enemyItem->setPos(enemy->getXPos() * gridWidth, enemy->getYPos() * gridHeight);
        enemyItem->setZValue(2);
        currentScene->addItem(enemyItem);
    }
}

void Graphicview::updateEnemy()
{
    for (auto& enemy : gameModel->getEnemies())
    {
        if (enemy->getXPos()==protagonist->getXPos() && enemy->getYPos()==protagonist->getYPos())
        {
            QGraphicsPixmapItem* enemyItem = new QGraphicsPixmapItem();
            // Create a new QGraphicsPixmapItem for each enemy
            MyEnemyGraphicview* enemyGraph = new MyEnemyGraphicview(enemy,gridWidth);
            // Create a new QGraphicsPixmapItem for each health pack
            enemyItem->setPixmap(*enemyGraph->returnStatusEnemy(enemy));
            enemyItem->setPos(enemy->getXPos() * gridWidth, enemy->getYPos() * gridHeight);
            enemyItem->setZValue(2);
            currentScene->addItem(enemyItem);
            if (std::dynamic_pointer_cast<PEnemy>(enemy)){
                status = 3;
            }
            else status = 2;
        }
    }
}

void Graphicview::PoisonedTilesUpdated(const std::vector<std::pair<int, int>>& poisonedTilePositions)
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

    // Store the poisoned tile positions
    poisonedTilePositionsVec.push_back(poisonedTilePositions);
    std::map<std::pair<int,int>, QGraphicsPixmapItem*> poisonItems;

    for (auto& pos : poisonedTilePositions) {
        if (poisonItems.find(pos) == poisonItems.end()) {
            QGraphicsPixmapItem* poisonItem = new QGraphicsPixmapItem();
            poisonItem->setPixmap(*Poison);
            poisonItem->setPos(pos.first * gridWidth, pos.second * gridHeight);
            poisonItem->setZValue(1);
            currentScene->addItem(poisonItem);
            poisonItems[pos] = poisonItem;
        }
    }
    poisonedTileItemsVec.push_back(poisonItems);
    update();
}

void Graphicview::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
    controller->handleKeyPress(event);

    // Clear the existing path
    clearPath();
    drawScene();
    updateProtagonist();
}

void Graphicview::handleProtagonistPositionChanged()
{
    // Clear the existing path
    clearPath();

    // Draw the new path
    drawScene();
    drawPath();
    bool t = false;
    for (const auto& poisonedTiles : poisonedTilePositionsVec) {
        for (const auto& pos : poisonedTiles) {
            if (protagonist->getXPos() == pos.first && protagonist->getYPos() == pos.second) {
                // Protagonist is in a poisoned tile
                // Add your logic here
                t = true;
                return; // Assuming you want to stop checking once found in one poisoned tile
            }
        }
    }
    if(t==true)status = 3;
    else status = 0;

    updateProtagonist();
}

void Graphicview::handleEnemyDead()
{
    updateEnemy();
}

void Graphicview::handleHealthpackConsumed()
{
    updateHealthpack();
}

void Graphicview::handleModelChanged(std::shared_ptr<GameModel> newModel)
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

void Graphicview::clearPath()
{
    // Remove previous path items from the scene
    QList<QGraphicsItem*> items = currentScene->items();
    for (QGraphicsItem* item : items)
    {
        if (item->type() == QGraphicsRectItem::Type) {
            currentScene->removeItem(item);
            delete item;
        }
    }
}

void Graphicview::drawPath()
{
    if (gameModel) {
        const std::vector<std::pair<int, int>>& path = gameModel->getPath();
        QBrush pathBrush(Qt::red);

        for (const auto& position : path) {
            int x = position.first * gridWidth;
            int y = position.second * gridHeight;

            QGraphicsRectItem* pathRect = new QGraphicsRectItem(x, y, gridWidth, gridHeight);
            pathRect->setBrush(pathBrush);
            currentScene->addItem(pathRect);
        }
    }
}

void Graphicview::mousePressEvent(QMouseEvent *event)
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

void Graphicview::loadTileG()
{
    QPixmap Tile1QP(":/images/images/sand.png");
    tile1 = std::make_shared<QPixmap>(Tile1QP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap Tile2QP(":/images/images/grass.png");
    tile2 = std::make_shared<QPixmap>(Tile2QP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap Tile3QP(":/images/images/wall.png");
    tile3 = std::make_shared<QPixmap>(Tile3QP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap PoisonQP(":/images/images/posion.png");
    Poison = std::make_shared<QPixmap>(PoisonQP.scaled(gridWidth,gridWidth, Qt::KeepAspectRatio));
    QPixmap doorupQP(":/images/images/doorup.png");
    doorUp = std::make_shared<QPixmap>(doorupQP.scaled(gridWidth, gridHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QPixmap doordownQP(":/images/images/doordown.png");
    doorDown = std::make_shared<QPixmap>(doordownQP.scaled(gridWidth, gridHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
