#include "strategy.h"

Strategy::Strategy(std::shared_ptr<GameModel> gameModel)
    : gameModel(gameModel), enemies(gameModel->getEnemies()), healthpacks(gameModel->getHealthpacks()), protagonist(gameModel->getProtagonist())
{
}

void Strategy::handleStrategy(bool newvalue)
{
    strategyEnabled = newvalue;

    if(strategyEnabled) calculateBestPath();
    else followPath(protagonist->getXPos(), protagonist->getYPos());
}

bool Strategy::calculateBestPath()
{
    if (!strategyEnabled) {
        std::cout << "Strategy is disabled. Stopping." << std::endl;
        return false;
    }

    int remainingEnemies = std::count_if(enemies.begin(), enemies.end(), [](const auto& enemy) { return !enemy->getDefeated();});

    if (!healthpacks.empty() && protagonist->getHealth() < 50) {
        takeNearestHealthpack();
        return true;
    }

    if (remainingEnemies > 0 && protagonist->getEnergy() > 0) {
        attackNearestEnemy();
        return true;
    }

    if (remainingEnemies == 0) {
        std::cout << "All enemies defeated. Moving to the next level." << std::endl;
        auto doorTileCoordinates = gameModel->getUpDoor();
        if (doorTileCoordinates != std::make_pair(-1, -1))
        {
            std::cout << "Moving to doorTile at position: (" << doorTileCoordinates.first << ", " << doorTileCoordinates.second << ")" << std::endl;
            followPath(doorTileCoordinates.first, doorTileCoordinates.second);
            return true;
        }
        else
        {
            std::cout << "DoorTile coordinates not available." << std::endl;
            return false;
        }
    }

    std::cout << "No enemies, no health packs, and energy depleted. Disabling strategy." << std::endl;
    handleStrategy(false);
    return false;

}

float Strategy::calculateRequiredEnergyToEnemy(const std::shared_ptr<Enemy>& enemy) const
{
    MyTile* start = gameModel->getTileAt(protagonist->getXPos(), protagonist->getYPos());
    MyTile* dest = gameModel->getTileAt(enemy->getXPos(), enemy->getYPos());

    std::vector<int> path = gameModel->calculatePath(start, dest);
    float totalWeight = gameModel->calculateTotalTileWeightAlongPath(path);

    return totalWeight;
}

float Strategy::calculateRequiredEnergyToHealthPack(const std::shared_ptr<MyHealthpack>& healthPack) const
{
    MyTile* start = gameModel->getTileAt(protagonist->getXPos(), protagonist->getYPos());
    MyTile* dest = gameModel->getTileAt(healthPack->getXPos(), healthPack->getYPos());

    std::vector<int> path = gameModel->calculatePath(start, dest);
    float totalWeight = gameModel->calculateTotalTileWeightAlongPath(path);

    return totalWeight;
}

void Strategy::attackNearestEnemy()
{
    auto nearestEnemyIter = std::min_element(enemies.begin(), enemies.end(), DistanceComparator<MyEnemy>(protagonist));
    if (nearestEnemyIter != enemies.end()) {
        std::shared_ptr<MyEnemy> nearestEnemy = *nearestEnemyIter;

        if(!nearestEnemy->getDefeated())
        {
            followPath(nearestEnemy->getXPos(), nearestEnemy->getYPos());
        }
        nearestEnemy->setDefeated(true);
    } else {
        std::cout << "No suitable enemy found." << std::endl;
    }
}

void Strategy::takeNearestHealthpack()
{
    auto nearestHealthpackIter = std::min_element(healthpacks.begin(), healthpacks.end(), DistanceComparator<MyHealthpack>(protagonist));
    if (nearestHealthpackIter != healthpacks.end()) {
        std::shared_ptr<MyHealthpack> nearestHealthpack = *nearestHealthpackIter;

        if(!nearestHealthpack->getConsumed())
        {
            followPath(nearestHealthpack->getXPos(), nearestHealthpack->getYPos());
        }
        nearestHealthpack->setConsumed();
    } else {
        std::cout << "No suitable health pack found." << std::endl;
    }
}

void Strategy::followPath(const int& xPos, const int& yPos)
{
    bool status = gameModel->gotoNewPosition(xPos, yPos);
    if (!status){
        std::cout << "Pathfinding could not find a path, autoplay disabled" << std::endl;
        setStrategyEnabled(false);
    }

    if (strategyEnabled) {
        calculateBestPath();
    }
}

void Strategy::handleGameOver(){
    setStrategyEnabled(false);
}

void Strategy::setModel(std::shared_ptr<GameModel> newModel)
{
    gameModel = newModel;
    enemies = gameModel->getEnemies();
    healthpacks = gameModel->getHealthpacks();
    protagonist = gameModel->getProtagonist();
}


