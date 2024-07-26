#include "textview.h"
#include "gamemodel.h"
#include "controller.h"
#include <iostream>

TextView::TextView(const std::shared_ptr<LevelManager>& levelManager, QWidget *parent)
    : QWidget(parent), controller(levelManager -> getController()), levelManager(levelManager)
{
    setFocusPolicy(Qt::StrongFocus);
    initializeTextview();

    connect(levelManager.get(), &LevelManager::modelChanged, this, &TextView::handleModelChanged);
}

void TextView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!gameModel)
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    auto protagonist = gameModel->getProtagonist();
    if (!protagonist)
        return;

    // Get the poisoned tile positions
    const std::vector<std::pair<int, int>>& poisonedTilePositions = poisonedTilePositions_;

    int protagonistX = protagonist->getXPos();
    int protagonistY = protagonist->getYPos();

    // Calculate the center position of the TextView in terms of tiles
    int centerRow = height() / (2 * tileSize);
    int centerCol = width() / (2 * tileSize);

    // Calculate the starting row and column for the loop
    int startRow = protagonistY - centerRow;
    int startCol = protagonistX - centerCol;

    // Calculate the ending row and column for the loop
    int endRow = startRow + height() / tileSize;
    int endCol = startCol + width() / tileSize;

    // Ensure that the calculated start and end values are within the valid range
    startRow = std::max(0, startRow);
    startCol = std::max(0, startCol);
    endRow = std::min(gameModel->getRows(), endRow);
    endCol = std::min(gameModel->getCols(), endCol);

    std::pair<int, int> upDoor = gameModel -> getUpDoor();
    std::pair<int, int> downDoor = gameModel -> getDownDoor();

    drawPath(painter);

    // Draw the visible portion of the world
    for (int row = startRow; row < endRow; ++row)
    {
        for (int col = startCol; col < endCol; ++col)
        {
            QString content;

            if(upDoor.first == col && upDoor.second == row && levelManager->getCurrentLevel() != levelManager->getNrOfLevels()){
                content = 'U';
            }
            if(downDoor.first == col && downDoor.second == row && levelManager->getCurrentLevel() != 0){
                content = 'D';
            }

            // Check if there is a health pack at this position
            for (auto healthPackTextview : healthpackTextviews)
            {
                if (healthPackTextview -> getHealthpack() -> getXPos() == col && healthPackTextview -> getHealthpack() -> getYPos() == row)
                {
                    auto [displayString, textColor] = healthPackTextview -> getDisplayInfo();
                    content = displayString;
                    painter.setPen(textColor);
                    break;
                }
            }

            // Check if there is an enemy at this position
            for (auto enemyTextView : enemyTextviews)
            {
                if (enemyTextView->getEnemy()->getXPos() == col && enemyTextView->getEnemy()->getYPos() == row)
                {
                    auto [displayString, textColor] = enemyTextView->getDisplayInfo();
                    content = displayString;
                    painter.setPen(textColor);
                    break;
                }
            }

            // Check if protagonist at this position
            if (protagonistX == col && protagonistY == row)
            {
                auto [displayString, textColor] = protagonistTextview->getDisplayInfo();
                content = displayString;
                painter.setPen(textColor);
            }

            // Check if the tile is poisoned
            auto it = std::find(poisonedTilePositions.begin(), poisonedTilePositions.end(), std::make_pair(col, row));
            if (it != poisonedTilePositions.end())
            {
                // Draw a yellow "X" for poisoned tiles
                painter.setPen(QColor(Qt::yellow));
                painter.drawText((col - startCol + 0.5) * tileSize, (row - startRow + 1) * tileSize - tileSize / 4,
                                 tileSize, tileSize / 2, Qt::AlignCenter, "p");
                painter.setPen(QColor(Qt::black));
            }

            // Draw the remaining lines
            painter.drawText((col - startCol + 0.5) * tileSize, (row - startRow + 1) * tileSize - tileSize / 4,
                             tileSize, tileSize / 2, Qt::AlignCenter, content);
            painter.setPen(QColor(Qt::black));
            painter.drawText((col - startCol) * tileSize, (row - startRow) * tileSize,
                             tileSize, tileSize, Qt::AlignCenter, "+");
            painter.drawText((col - startCol + 0.5) * tileSize, (row - startRow) * tileSize,
                             tileSize, tileSize, Qt::AlignCenter, "---");
            painter.drawText((col - startCol) * tileSize, (row - startRow + 1) * tileSize - tileSize / 4,
                             tileSize, tileSize / 2, Qt::AlignCenter, "|");
        }
    }
}

void TextView::drawPath(QPainter& painter)
{
    if (gameModel) {
        // Draw the path
        const std::vector<std::pair<int, int>>& path = gameModel->getPath();
        painter.setPen(QColor(Qt::blue));  // Set the color for the path
        for (size_t i = 1; i < path.size(); ++i)
        {
            int x1 = (path[i - 1].first - startCol) * tileSize + tileSize / 2;
            int y1 = (path[i - 1].second - startRow) * tileSize + tileSize / 2;
            int x2 = (path[i].first - startCol) * tileSize + tileSize / 2;
            int y2 = (path[i].second - startRow) * tileSize + tileSize / 2;

            painter.drawLine(x1, y1, x2, y2);
        }
    }
}

void TextView::keyPressEvent(QKeyEvent *event)
{
    // clearing the path when the user moves with the keys
    gameModel->clearPath();

    if (controller)
        controller->handleKeyPress(event);
}

void TextView::handlePoisonedTilesUpdated(const std::vector<std::pair<int, int>>& poisonedTilePositions)
{
    // Store the poisoned tile positions
    poisonedTilePositions_ = poisonedTilePositions;

    // Redraw the TextView to update the display
    update();
}

void TextView::handleDisplayInfoChanged()
{
    update(); // This will trigger a paintEvent, causing the view to be updated
}

void TextView::handleModelChanged(std::shared_ptr<GameModel> newModel)
{
    gameModel = newModel;
    std::cout<< "new model in textview " << gameModel << std::endl;
    initializeTextview();
    repaint();
}

void TextView::initializeTextview(){
    enemyTextviews.clear();
    healthpackTextviews.clear();
    protagonistTextview.reset();

    gameModel = levelManager -> getCurrentGameModel();

    for ( auto& enemy : gameModel->getEnemies())
    {
        enemyTextviews.emplace_back(MyEnemyTextview::createEnemyTextview(enemy));
    }

    for ( auto& healthpack : gameModel -> getHealthpacks() )
    {
        healthpackTextviews.emplace_back(std::make_shared<MyHealthpackTextview>( healthpack ));
    }

    protagonistTextview = std::make_shared<ProtagonistTextview>(gameModel->getProtagonist());

    connect(protagonistTextview.get(), &ProtagonistTextview::displayInfoChanged, this, &TextView::handleDisplayInfoChanged);

    connect(gameModel.get(), &GameModel::poisonedTilesUpdated, this, &TextView::handlePoisonedTilesUpdated);
}
