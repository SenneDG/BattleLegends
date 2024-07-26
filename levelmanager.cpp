#include "levelmanager.h"
#include "qpixmap.h"
#include <QObject>

LevelManager::LevelManager( int nrOfLevels, QString filename):
    nrOfLevels(nrOfLevels), worldFile(filename), currentLevel(0)
{
    levelModels.reserve(nrOfLevels);

    loadModelForLevel(currentLevel);
    loadModelForLevel(currentLevel + 1);
    connectToCurrentModelSignal();
}
bool LevelManager::loadModelForLevel(int levelID)
{
    if( levelID == static_cast<int>(levelModels.size()) )
    {
        //This means this level is the last one in the vector, so we load the next level already (creating a world and a new model)
        World* newWorld = new World();
        QPixmap image(worldFile);

        int width = image.width();
        int height = image.height();

        int percentOfEnemies = std::sqrt(width*height)/5;
        int percentOfHealthpacks = std::sqrt(width*height)/4;

        float result = 1.0 - static_cast<float>(levelID) /nrOfLevels;
        if (result == 0){
            result = 0.01;
        }

        newWorld ->createWorld(worldFile, percentOfEnemies * (levelID*0.25 + 1), percentOfHealthpacks * result);

//        newWorld ->createWorld(worldFile, 5, 2);
        std::shared_ptr<GameModel> newModel;
        if(levelModels.size() != 0){
           newModel = std::make_shared<GameModel>(newWorld, levelModels[levelID - 1]->getProtagonist());
        }
        else{
           newModel = std::make_shared<GameModel>(newWorld);
        }
        levelModels.push_back(newModel);
        //Set protagonist of the previous level to the current level
    }
    if( !controller )
    {
        controller = std::make_shared<Controller>(getCurrentGameModel());
        controller -> setModel(getCurrentGameModel());
    }
    return true;
}

void LevelManager::handleLevelChange(int levelChange)
{
    int newLevel = currentLevel + levelChange;

    // Check if the new level is within the valid range [0, nrOfLevels - 1]
    if (newLevel >= 0 && newLevel < nrOfLevels)
    {
        // Disconnect the current model's signal from the slot
        disconnect(levelModels[currentLevel].get(), &GameModel::levelChanged, this, &LevelManager::handleLevelChange);

        // Load the model for the new level
        if (loadModelForLevel(newLevel + levelChange))
        {
            // Set the current level to the new level
            currentLevel = newLevel;

            // Emit a signal indicating the model change
            emit modelChanged(levelModels[currentLevel]);
            controller -> setModel(levelModels[currentLevel]);
            std::cout << "new level emitted levelmanager" << levelModels[currentLevel] << std::endl;
            // Connect the signal from the new model to the handleLevelChange slot
            connectToCurrentModelSignal();
        }
        else
        {
            // Handle the case where loading the model failed
            // You may want to log an error or take appropriate action
            qDebug() << "Failed to load model for level" << newLevel;
        }
    }
    else
    {
        // Handle the case where the new level is outside the valid range
        // You may want to log an error or take appropriate action
        qDebug() << "Invalid level index" << newLevel;
    }
}

void LevelManager::connectToCurrentModelSignal()
{
    // Connect the signal from the current model to the handleLevelChange slot
    connect(levelModels[currentLevel].get(), &GameModel::levelChanged, this, &LevelManager::handleLevelChange);
}

std::shared_ptr<GameModel> LevelManager::getCurrentGameModel()
{
    return levelModels[currentLevel];
}

std::shared_ptr<Controller> LevelManager::getController(){
    return controller;
}
