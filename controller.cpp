#include "controller.h"

Controller::Controller(std::shared_ptr<GameModel> gameModel):
    gameModel(gameModel), myStrategy(std::make_shared<Strategy>(gameModel))
{
    keyActions[Qt::Key_Left] = [this]() { handleKeyLeft(); };
    keyActions[Qt::Key_Right] = [this]() { handleKeyRight(); };
    keyActions[Qt::Key_Up] = [this]() { handleKeyUp(); };
    keyActions[Qt::Key_Down] = [this]() { handleKeyDown(); };

    textActions["up"] = [this](const QString command) { handleKeyUp(); };
    textActions["down"] = [this](const QString command) { handleKeyDown(); };
    textActions["left"] = [this](const QString command) { handleKeyLeft(); };
    textActions["right"] = [this](const QString command) { handleKeyRight(); };
    textActions["attack nearest enemy"] = [this](const QString command) { handleAttackNearestEnemy(command); };
    textActions["take nearest health pack"] = [this](const QString command) { handleTakeNearestHealthpack(command); };

    connect(this, &Controller::toggleStrategy, myStrategy.get(), &Strategy::handleStrategy);
    connect(gameModel->getProtagonist().get(), &MyProtagonist::gameOver, gameModel.get(), &GameModel::onGameOver);
    connect(gameModel->getProtagonist().get(), &MyProtagonist::gameOver, myStrategy.get(), &Strategy::handleGameOver);
}

void Controller::handleAutoPlay()
{
    enabled = !enabled;
    emit toggleStrategy(enabled);
}

void Controller::handleCommand(const QString& command)
{
    if (command.contains("goto"))
    {
        handleGoTo(command);
    } else if (textActions.contains(command))
    {
        textActions[command](command);
    } else {
        std::cout << "Command not recognized." << std::endl;
    }
}

void Controller::handleKeyPress(QKeyEvent *event)
{
    std::cout << "Key pressed!" << std::endl;

    if (keyActions.contains(event->key()))
    {
        keyActions[event->key()]();
    } else
    {
        std::cout << "Keypress not recognized." << std::endl;
    }
}

void Controller::handleKeyLeft()
{
    gameModel -> moveTo(-1,0);
    std::cout << "moving left" << std::endl;
}

void Controller::handleKeyRight()
{
    gameModel -> moveTo(1,0);
    std::cout << "moving right" << std::endl;
}

void Controller::handleKeyUp()
{
    gameModel -> moveTo(0,-1);
    std::cout << "moving up" << std::endl;
}

void Controller::handleKeyDown()
{
    gameModel -> moveTo(0,1);
    std::cout << "moving down" << std::endl;
}

void Controller::handleGoTo(const QString& command) const
{
    static QRegularExpression regex("goto\\((\\d+),(\\d+)\\)");

    // Check if the command matches the pattern
    QRegularExpressionMatch match = regex.match(command);
    if (match.hasMatch())
    {
        std::cout << "Matching" << std::endl;

        for (int i = 0; i <= match.lastCapturedIndex(); ++i) {
            std::cout << "Captured group " << i << ": " << match.captured(i).toStdString() << std::endl;
        }

        int x = match.captured(1).toInt();
        int y = match.captured(2).toInt();

        std::cout << "Go to (" << x << "," << y << ")" << std::endl;
        gameModel->gotoNewPosition(x, y);
    } else
    {
        std::cout << "No match" << std::endl;
    }
}

void Controller::handleAttackNearestEnemy(const QString& command) const
{
    std::cout << command.toStdString() << std::endl;
    myStrategy->attackNearestEnemy();
}

void Controller::handleTakeNearestHealthpack(const QString& command) const
{
    std::cout << command.toStdString() << std::endl;
    myStrategy->takeNearestHealthpack();
}

void Controller::setModel(const std::shared_ptr<GameModel>& newModel)
{
    gameModel = newModel;
    myStrategy->setModel(newModel);
}



