#include "gamemodel.h"
#include "pathfinder_class.h"

Comparator<MyTile> sortlV = [](const MyTile& a, const MyTile& b) -> bool {
    return a.h > b.h;
};

GameModel::GameModel(World* gameWorld, std::shared_ptr<MyProtagonist> newProtagonist, QObject* parent)
    : QObject(parent),
    gameWorld(gameWorld)
{
    initializeProtagonist(newProtagonist);
    initializeWorld();
}

void GameModel::moveTo(int xPos, int yPos)
{
    int targetX = protagonist->getXPos() + xPos;
    int targetY = protagonist->getYPos() + yPos;

    // Check if the target position is within the boundaries of the world
    if (targetX < 0 || targetX >= gameWorld->getCols() || targetY < 0 || targetY >= gameWorld->getRows()) {
        // The target position is outside the world boundaries, handle this case as needed
        std::cout << "Cannot move outside the world boundaries." << std::endl;
        return;
    }

    // check if level up
    if(targetX == upDoor.first && targetY == upDoor.second){
        std::cout << "level change" << std::endl;
        emit levelChanged(1);
    }

    // check if level down
    if(targetX == downDoor.first && targetY == downDoor.second){
            std::cout << "level change -1" << std::endl;
            emit levelChanged(-1);
        }

    std::for_each(enemies.begin(), enemies.end(), [&](const auto& enemy) {
        if (auto xEnemy = std::dynamic_pointer_cast<MyXEnemy>(enemy)) {
            xEnemy->updateAttackStatus(targetX, targetY);
        }
    });

    protagonist->moveProtagonist(protagonist -> getXPos() + xPos, protagonist -> getYPos() + yPos);

    //Update energy of protagonist
    float newEnergy = protagonist -> getEnergy() - getTileAt(targetX, targetY)->getValue();
    if(newEnergy > 0 && !isImmortal){
        protagonist->setEnergy( newEnergy);
        std::cout << "energy set " << newEnergy << std::endl;
    }
    else if( targetX != upDoor.first && targetX != downDoor.first && targetY != upDoor.second && targetY != downDoor.second && !isImmortal){
        emit protagonist->gameOver();
        std::cout << "gameOver" << std::endl;
    }


    // Check if there's an enemy at the target position
    auto enemyIt = std::find_if(enemies.begin(), enemies.end(), [targetX, targetY](const auto& enemy) {
        return enemy->getXPos() == targetX && enemy->getYPos() == targetY;
    });

    if (enemyIt != enemies.end()) {
        (*enemyIt)->attack();
    }

    // Check if there's a health pack at the target position
    auto healthPackIt = std::find_if(healthpacks.begin(), healthpacks.end(), [targetX, targetY](const auto& healthPack) {
        return healthPack->getXPos() == targetX && healthPack->getYPos() == targetY;
    });

    if (healthPackIt != healthpacks.end()) {
        protagonist->useHealthPack(*healthPackIt);
    }
}

void GameModel::handlePoisonLevelUpdated(float newPoisonLevel, const PEnemy* pEnemy) {
    const float highPoisonThreshold = 50.0f;
    const float mediumPoisonThreshold = 30.0f;
    const float resetPoisonThreshold = 10.0f;

    int range = (newPoisonLevel >= highPoisonThreshold) ? 1 :
                    (newPoisonLevel >= mediumPoisonThreshold) ? 2 : 3;

    std::cout << "Poison level updated: " << newPoisonLevel << ", Range: " << range << std::endl;

    std::vector<std::pair<int, int>> poisonedTilePositions;

    int pEnemyXPos = pEnemy->getXPos();
    int pEnemyYPos = pEnemy->getYPos();

    // Calculate the range boundaries
    int minX = std::max(0, pEnemyXPos - range);
    int maxX = std::min(gameWorld->getCols() - 1, pEnemyXPos + range);
    int minY = std::max(0, pEnemyYPos - range);
    int maxY = std::min(gameWorld->getRows() - 1, pEnemyYPos + range);

    for (int tileX = minX; tileX <= maxX; ++tileX) {
        for (int tileY = minY; tileY <= maxY; ++tileY) {
            // Calculate the Manhattan distance between the PEnemy and the tile
            int distance = std::abs(pEnemyXPos - tileX) + std::abs(pEnemyYPos - tileY);

            // Set the poison level if the tile is within the specified range
            if (distance <= range) {
                auto tileIndex = tileY * gameWorld->getCols() + tileX;
                std::dynamic_pointer_cast<MyTile>(tiles[tileIndex])->setPoisonLevel(newPoisonLevel);
                poisonedTilePositions.emplace_back(tileX, tileY);

                // Check if the protagonist is on the current tile
                if (protagonist->getXPos() == tileX && protagonist->getYPos() == tileY) {
                    float poisonLevel = std::dynamic_pointer_cast<MyTile>(tiles[tileIndex])->getPoisonLevel();
                    if (poisonLevel > 0.0f) {
                        // Apply damage to the protagonist based on poison level
                        protagonist->handleHealthChange(poisonLevel / 10);
                        std::cout << "Protagonist took damage from poison: " << poisonLevel << std::endl;
                    }
                }
            }
        }
    }

    emit poisonedTilesUpdated(poisonedTilePositions);

    if (newPoisonLevel <= resetPoisonThreshold) {
        resetAllTilesPoisonLevels();
        emit poisonedTilesUpdated({});
    }
}

void GameModel::resetAllTilesPoisonLevels() {
    for (const auto& tile : tiles) {
        std::dynamic_pointer_cast<MyTile>(tile)->setPoisonLevel(0.0f);
    }
}

std::vector<int> GameModel::calculatePath(const MyTile *start, const MyTile *dest)
{
    for (MyTile& node : pathNodes) {
        node.f = 0.0f;
        node.g = 0.0f;
        node.h = 0.0f;
        node.visited = false;
        node.prev = nullptr;
    }

    PathFinder<MyTile, MyTile> pathfinder(pathNodes, start, dest, sortlV, gameWorld->getCols(), distWeight);

    // Call A_star to get the list of moves
    std::vector<int> path = pathfinder.A_star();

    if (path.empty()) return { -1 };
    else return path;
}

bool GameModel::gotoNewPosition(const int& newX, const int& newY)
{
    clearPath();

    MyTile* start = getTileAt(protagonist->getXPos(), protagonist->getYPos());
    MyTile* dest = getTileAt(newX, newY);

    if (!dest) {
        std::cerr << "Error: Destination tile is null. (X: " << newX << ", Y: " << newY << ")" << std::endl;
        return false;
    }

    std::vector<int> moves = calculatePath(start, dest);
    if (moves[0] == -1) return false;

    for (int move : moves) {
        if (gameOverSignalReceived) return false;

        // Calculate the new position based on the move
        int xPos = 0, yPos = 0;
        switch (move) {
        case 0:  yPos = -1; break;  // Move Up
        case 1:  yPos = -1; xPos = -1; break;  // Move Up-Left
        case 2:  xPos = -1; break;  // Move Left
        case 3:  yPos = 1; xPos = -1; break;   // Move Down-Left
        case 4:  yPos = 1; break;   // Move Down
        case 5:  yPos = 1; xPos = 1; break;    // Move Down-Right
        case 6:  xPos = 1; break;   // Move Right
        case 7:  yPos = -1; xPos = 1; break;   // Move Up-Right
        default:
            std::cerr << "Invalid move: " << move << std::endl;
            continue; // Skip invalid moves
        }

        moveTo(xPos, yPos);

        // Add the new position to the path
        path.push_back({protagonist->getXPos(), protagonist->getYPos()});

        // animation delay
        QEventLoop loop;
        QTimer::singleShot(animationDelay, &loop, &QEventLoop::quit);
        loop.exec();
    }

    return true;
}

float GameModel::calculateTotalTileWeightAlongPath(const std::vector<int>& path) {
    float totalWeight = 0.0f;

    // Calculate the new position based on the move
    int xPos = 0, yPos = 0;

    for (int move : path) {
        switch (move) {
        case 0:  yPos -= 1; break;  // Move Up
        case 1:  yPos -= 1; xPos += 1; break;  // Move Up-Left
        case 2:  xPos -= 1; break;  // Move Left
        case 3:  yPos += 1; xPos -= 1; break;   // Move Down-Left
        case 4:  yPos += 1; break;   // Move Down
        case 5:  yPos += 1; xPos += 1; break;    // Move Down-Right
        case 6:  xPos += 1; break;   // Move Right
        case 7:  yPos -= 1; xPos += 1; break;   // Move Up-Right
        default:
            std::cerr << "Invalid move: " << move << std::endl;
            continue; // Skip invalid moves
        }

        // Get the tile at the new position
        MyTile* tile = getTileAt(protagonist->getXPos() + xPos, protagonist->getYPos() + yPos);

        // Check if the tile exists
        if (!tile) continue;
        totalWeight += tile->getValue();
    }

    return totalWeight;
}

void GameModel::printHealthPackPositions() const
{
    std::cout << "healthpack Positions:\n";
    for (const auto& healthpack : healthpacks) {
        std::cout << "X: " << healthpack->getXPos() << ", Y: " << healthpack->getYPos() << std::endl;
    }
    std::cout << "End of healthpack Positions\n";
}

void GameModel::printEnemyPositions() const
{
    std::cout << "Enemy Positions:\n";
    for (const auto& enemy : enemies) {
        std::cout << "X: " << enemy->getXPos() << ", Y: " << enemy->getYPos() << std::endl;
        std::cout << "Defeated: " << enemy->getDefeated() << std::endl;
    }
    std::cout << "End of Enemy Positions\n";
}

void GameModel::printPEnemies() const {
    std::cout << "Printing PEnemies:" << std::endl;
    for (const auto& pEnemy : enemies) {
        // Check if it's a PEnemy
        if (PEnemy* pEnemyPtr = dynamic_cast<PEnemy*>(pEnemy.get())) {
            std::cout << "PEnemy at position (" << pEnemyPtr->getXPos() << ", " << pEnemyPtr->getYPos() << ")" << std::endl;
            std::cout << "   Poison Level: " << pEnemyPtr->getPoisonLevel() << std::endl;
        }
    }
    std::cout << "End of PEnemies list." << std::endl;
}

void GameModel::printMyXEnemies() const {
    std::cout << "Printing MyXEnemies:" << std::endl;
    for (const auto& xEnemy : enemies) {
        if (MyXEnemy* xEnemyPtr = dynamic_cast<MyXEnemy*>(xEnemy.get())) {
            std::cout << "MyXEnemy at position (" << xEnemyPtr->getXPos() << ", " << xEnemyPtr->getYPos() << ")" << std::endl;
            std::cout << "   Strength: " << xEnemyPtr->getValue() << std::endl;
        }
    }
    std::cout << "End of MyXEnemies list." << std::endl;
}

MyTile* GameModel::getTileAt(const int& xPos, const int& yPos) const
{
    for (auto& tile : tiles)
    {
        if (tile->getXPos() == xPos && tile->getYPos() == yPos)
        {
            return tile.get();
        }
    }
    return nullptr;
}

void GameModel::initializeProtagonist(const std::shared_ptr<MyProtagonist>& newProtagonist)
{
    if(!newProtagonist) protagonist = std::make_shared<MyProtagonist>();
    else protagonist = newProtagonist;
}

void GameModel::initializeWorld()
{
    initializeEnemies();
    initializeTiles();
    initializeHealthPacks();
    generateDoors();
    initializePathNodes();
}

void GameModel::initializeEnemies()
{
    std::vector<std::unique_ptr<Enemy>> rawEnemies = gameWorld->getEnemies();
    for (auto &enemy : rawEnemies)
    {
        if (auto pEnemy = dynamic_cast<PEnemy*>(enemy.get()))
        {
            auto myPEnemy = std::make_shared<MyPEnemy>(pEnemy->getXPos(), pEnemy->getYPos(), pEnemy->getValue());
            connect(myPEnemy.get(), &MyPEnemy::poisonLevelUpdated, this, [this, myPEnemy](float newPoisonLevel)
                    {
                        handlePoisonLevelUpdated(newPoisonLevel, myPEnemy.get());
                    });
            enemies.emplace_back(std::move(myPEnemy));
        }
        else if (auto normalEnemy = dynamic_cast<Enemy*>(enemy.get()))
        {
            auto myEnemy = std::make_shared<MyEnemy>(normalEnemy->getXPos(), normalEnemy->getYPos(), normalEnemy->getValue());
            connect(myEnemy.get(), &MyEnemy::enemyDidDamage, protagonist.get() , &MyProtagonist::handleHealthChange);
            enemies.emplace_back(std::move(myEnemy));
        }
        else
        {
            // Handle other types of enemies or log an error
            std::cerr << "Unknown enemy type encountered." << std::endl;
        }
    }

    // Replace some percentage of the MyEnemy to MyXEnemy type
    replaceEnemiesWithXEnemies();
}

void GameModel::initializeTiles()
{
    std::vector<std::unique_ptr<Tile>> rawTiles = gameWorld->getTiles();
    // Convert unique_ptrs to shared_ptrs
    for (auto &tile : rawTiles)
    {
        tiles.emplace_back(std::make_shared<MyTile>(tile->getXPos(), tile->getYPos(), tile->getValue()));
    }
}

void GameModel::initializePathNodes()
{
    // Create PathNodes from Tiles
    for (const auto &tile : tiles)
    {
        MyTile pathNode(tile->getXPos(), tile->getYPos(), tile->getValue());
        pathNodes.emplace_back(pathNode);
    }
}

void GameModel::initializeHealthPacks()
{
    std::vector<std::unique_ptr<Tile>> rawHealthPacks = gameWorld->getHealthPacks();
    for (auto &healthpack : rawHealthPacks)
    {
        healthpacks.emplace_back(std::make_shared<MyHealthpack>(std::move(healthpack)));
    }
}

void GameModel::replaceEnemiesWithXEnemies()
{
    auto nrOfXEnemies = std::max(static_cast<size_t>(1), static_cast<size_t>(0.05 * enemies.size()));

    for (auto it = enemies.begin(); it != enemies.end() && nrOfXEnemies > 0; ++it)
    {
        if (MyEnemy *regularEnemy = dynamic_cast<MyEnemy *>(it->get()))
        {
            auto xEnemy = std::make_shared<MyXEnemy>(regularEnemy->getXPos(), regularEnemy->getYPos(), regularEnemy->getValue());
            connect(xEnemy.get(), &MyXEnemy::enemyDidDamage, protagonist.get(), &MyProtagonist::handleHealthChange);
            it = enemies.erase(it);
            it = enemies.insert(it, std::make_move_iterator(&xEnemy), std::make_move_iterator(&xEnemy + 1));
            --nrOfXEnemies;
        }
    }
}

void GameModel::generateDoors()
{
    std::pair<int,int> Cor1;
    std::pair<int,int> Cor2;

    do {
        Cor1 = randCor();
    } while (checkCoincidence(Cor1));

    do {
        Cor2 = randCor();
    } while (checkCoincidence(Cor2) || Cor1 == Cor2);

    upDoor = Cor1;
    downDoor = Cor2;

    // set the doortile values to infinity so the tile will never be chosen by A_star algorithm
    getTileAt(Cor1.first, Cor1.second)->setValue(1000.0f);
    getTileAt(Cor2.first, Cor2.second)->setValue(1000.0f);
}

bool GameModel::checkCoincidence(const std::pair<int,int>& cor) const
{
    for (const auto& enemy : enemies)
    {
        if (enemy->getXPos() == cor.first && enemy->getYPos() == cor.second)
        {
            return true;
        }
    }

    for (const auto& healthpack : healthpacks)
    {
        if (healthpack->getXPos() == cor.first && healthpack->getYPos() == cor.second)
        {
            return true;
        }
    }

    return false;
}

std::pair<int,int> GameModel::randCor()
{
    int randNum1 = std::rand() % getRows() + 0;
    int randNum2 = std::rand() % getCols() + 0;
    std::pair<int,int> cor = std::make_pair(randNum1, randNum2);

    return cor;
}

void GameModel::onAnimationDelayChanged(int & value)
{
    animationDelay = value;
}

void GameModel::onHeuristicCostChanged(float & value)
{
    distWeight = value;
}

void GameModel::onImmortalToggle(bool enabled)
{
    isImmortal = enabled;
    protagonist->setIsImmortal(enabled);
}

void GameModel::onGameOver()
{
    gameOverSignalReceived = true;
}
