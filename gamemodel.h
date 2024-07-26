#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include <QEventLoop>
#include <QTimer>
#include <QObject>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

#include "world.h"
#include "myenemy.h"
#include "myxenemy.h"
#include "mytile.h"
#include "myhealthpack.h"
#include "mypenemy.h"
#include "myprotagonist.h"

class GameModel : public QObject
{
    Q_OBJECT
public:
    GameModel(World* gameWorld, std::shared_ptr<MyProtagonist> = nullptr, QObject* parent = nullptr);

    void moveTo(const int xPos, const int yPos);
    void printEnemyPositions() const;
    void printHealthPackPositions() const;
    void printPEnemies() const;
    void printMyXEnemies() const;

    const std::shared_ptr<MyProtagonist>& getProtagonist() const;
    const std::vector<std::shared_ptr<MyEnemy>>& getEnemies() const;
    const std::vector<std::shared_ptr<MyHealthpack>>& getHealthpacks() const;
    const std::vector<std::shared_ptr<MyTile>>& getTiles() const;

    int getRows() const {return gameWorld->getRows();}
    int getCols() const {return gameWorld->getCols();}

    void setDistWeight(const float& newDistWeight){distWeight = newDistWeight;}
    void setAnimationDelay(const int& delay){animationDelay = delay;}

    World* getWorld() const {return gameWorld;}
    MyTile* getTileAt(const int& xPos, const int& yPos) const;

    std::vector<int> calculatePath(const MyTile* start, const MyTile* dest);
    bool gotoNewPosition(const int& newX, const int& newY);
    float calculateTotalTileWeightAlongPath(const std::vector<int>& path);

    void clearPath() {path.clear();}
    const std::vector<std::pair<int, int>>& getPath() const {return path;}
    const std::pair<int,int>& getUpDoor() const {return upDoor;}
    const std::pair<int,int>& getDownDoor() const {return downDoor;}

private:
    World* gameWorld;
    std::shared_ptr<MyProtagonist> protagonist;
    std::vector<std::shared_ptr<MyTile>> tiles;
    std::vector<std::shared_ptr<MyEnemy>> enemies ;
    std::vector<std::shared_ptr<MyHealthpack>> healthpacks;
    std::unique_ptr<MyPEnemy> myPEnemy;
    std::vector<MyTile> pathNodes;
    std::vector<std::pair<int, int>> path;

    float distWeight {0.0f};
    int animationDelay {10};
    bool isImmortal {false};
    bool gameOverSignalReceived {false};

    void resetAllTilesPoisonLevels();

    std::pair<int,int> upDoor;
    std::pair<int,int> downDoor;
    std::pair<int,int> randCor();

    void initializeProtagonist(const std::shared_ptr<MyProtagonist>& newProtagonist);
    void initializeWorld();
    void initializeEnemies();
    void initializeTiles();
    void initializePathNodes();
    void initializeHealthPacks();
    void replaceEnemiesWithXEnemies();
    void generateDoors();
    bool checkCoincidence(const std::pair<int,int>& cor) const;

signals:
    void poisonedTilesUpdated(const std::vector<std::pair<int, int>>& positions);
    void levelChanged(int levelChange);

private slots:
    void handlePoisonLevelUpdated(float poisonLevel, const PEnemy* pEnemy);

public slots:
    void onAnimationDelayChanged(int & value);
    void onHeuristicCostChanged(float & value);
    void onImmortalToggle(bool enabled);
    void onGameOver();
};

inline const std::shared_ptr<MyProtagonist>& GameModel::getProtagonist() const {
    return protagonist;
}

inline const std::vector<std::shared_ptr<MyEnemy>>& GameModel::getEnemies() const {
    return enemies;
}

inline const std::vector<std::shared_ptr<MyHealthpack>>& GameModel::getHealthpacks() const {
    return healthpacks;
}

inline const std::vector<std::shared_ptr<MyTile>>& GameModel::getTiles() const {
    return tiles;
}

#endif // GAMEMODEL_H
