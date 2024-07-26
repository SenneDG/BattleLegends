#ifndef MYENEMYTEXTVIEW_H
#define MYENEMYTEXTVIEW_H

#include "world.h"
#include <iostream>
#include "myxenemy.h"
#include <QObject>
#include <QString>
#include <QColor>
#include <memory>

class Enemy;

class MyEnemyTextview : public QObject
{
    Q_OBJECT

public:
    explicit MyEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance);
    virtual ~MyEnemyTextview() = default;

    std::pair<QString, QColor> const getDisplayInfo(){return {displayString, color};};
    std::shared_ptr<Enemy> const getEnemy(){return enemyInstance;};

    static std::shared_ptr<MyEnemyTextview> createEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance); //factory method to create proper textview enemies


public slots:
    void handleEnemyDead();

protected:
    std::shared_ptr<Enemy> enemyInstance;
    QString displayString;
    QColor color;
};

#endif // MYENEMYTEXTVIEW_H
