#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QKeyEvent>
#include <QString>
#include <QMap>
#include <QRegularExpression>
#include <iostream>

#include "gamemodel.h"
#include "strategy.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller(std::shared_ptr<GameModel> gameModel);

    void handleKeyPress(QKeyEvent *event);
    void setModel(const std::shared_ptr<GameModel>& gameModel);

private:
    void handleKeyRight();
    void handleKeyLeft();
    void handleKeyUp();
    void handleKeyDown();
    void printHelp();
    void handleGoTo(const QString& command) const;
    void handleAttackNearestEnemy(const QString& command) const;
    void handleTakeNearestHealthpack(const QString& command) const;

    QMap<int, std::function<void()>> keyActions;
    QMap<QString, std::function<void(const QString)>> textActions;
    std::shared_ptr<GameModel> gameModel;
    std::shared_ptr<Strategy> myStrategy;
    bool enabled = false;

signals:
    void toggleStrategy(bool& enabled);

public slots:
    void handleCommand(const QString& command);
    void handleAutoPlay();
};

#endif // CONTROLLER_H
