#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include <QString>
#include <deque>
#include <QObject>

#include "controller.h"
#include "gamemodel.h"
#include "world.h"

class LevelManager : public QObject
{
    Q_OBJECT
public:
    LevelManager(int nrOfLevels, QString filename);

    std::shared_ptr<GameModel> getCurrentGameModel();
    std::shared_ptr<Controller> getController();

    int getCurrentLevel() const {return currentLevel;}
    int getNrOfLevels() const {return nrOfLevels;}
    const QString getFileName() const {return worldFile;}

private:
    const unsigned int nrOfLevels;
    const QString worldFile;
    std::shared_ptr<Controller> controller;
    std::vector<std::shared_ptr<GameModel>> levelModels;
    int currentLevel;  // Index of the current model in the window

    bool loadModelForLevel(int levelID);
    void connectToCurrentModelSignal();

public slots:
    void handleLevelChange(int levelChange);

signals:
    void modelChanged(std::shared_ptr<GameModel> newModel);
};

#endif // LEVELMANAGER_H
