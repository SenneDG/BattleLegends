#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "levelmanager.h"
#include "protagonisttextview.h"
#include "myenemytextview.h"
#include "gamemodel.h"
#include "controller.h"
#include "myhealthpacktextview.h"
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

class TextView : public QWidget
{
    Q_OBJECT

public:
    TextView(const std::shared_ptr<LevelManager>& levelManager, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void handlePoisonedTilesUpdated(const std::vector<std::pair<int, int>>& poisonedTilePositions);
    void handleDisplayInfoChanged();
    void handleModelChanged( std::shared_ptr<GameModel> newModel);

private:
    void drawPath(QPainter& painter);
    void initializeTextview();

    std::shared_ptr<Controller> controller;
    std::shared_ptr<GameModel> gameModel;
    std::shared_ptr<LevelManager> levelManager;

    std::shared_ptr<ProtagonistTextview> protagonistTextview;
    std::vector<std::shared_ptr<MyEnemyTextview>> enemyTextviews;
    std::vector<std::shared_ptr<MyHealthpackTextview>> healthpackTextviews;

    std::vector<std::pair<int, int>> poisonedTilePositions_;

    const int startRow = 0;
    const int startCol = 0;
    const int tileSize = 40;
};

#endif // TEXTVIEW_H
