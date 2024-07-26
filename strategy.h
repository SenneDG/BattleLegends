#ifndef STRATEGY_H
#define STRATEGY_H

#include "gamemodel.h"
#include "mytile.h"

class Strategy : public QObject
{
    Q_OBJECT
public:
    Strategy(std::shared_ptr<GameModel> gameModel);

    void attackNearestEnemy();
    void takeNearestHealthpack();
    void setModel(std::shared_ptr<GameModel> gameModel);
    void setStrategyEnabled(const bool& newEnabled){strategyEnabled = newEnabled;}

private:
    std::shared_ptr<GameModel> gameModel;
    std::vector<std::shared_ptr<MyEnemy>> enemies ;
    std::vector<std::shared_ptr<MyHealthpack>> healthpacks;
    std::shared_ptr<MyProtagonist> protagonist;

    bool calculateBestPath();
    float calculateRequiredEnergyToEnemy(const std::shared_ptr<Enemy>& enemy) const;
    float calculateRequiredEnergyToHealthPack(const std::shared_ptr<MyHealthpack>& healthPack) const;
    void followPath(const int& xPos, const int& yPos);
    bool strategyEnabled {false};

public slots:
    void handleStrategy(bool enabled);
    void handleGameOver();
};

template <typename T>
class DistanceComparator
{
public:
    std::shared_ptr<MyProtagonist> protagonist;
    DistanceComparator(std::shared_ptr<MyProtagonist> protagonist) : protagonist(protagonist) {}

    bool operator()(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs) const
    {
        if constexpr (std::is_same_v<T, MyEnemy>)
        {
            if (lhs->getDefeated() || rhs->getDefeated())
                return rhs->getDefeated();
        }
        else if constexpr (std::is_same_v<T, MyHealthpack>)
        {
            if (lhs->getConsumed() || rhs->getConsumed())
                return rhs->getConsumed();
        }

        float distance_lhs = calculateDistanceToProtagonist(lhs);
        float distance_rhs = calculateDistanceToProtagonist(rhs);
        return distance_lhs < distance_rhs;
    }

    float calculateDistanceToProtagonist(const std::shared_ptr<T>& item) const
    {
        float deltaX = item->getXPos() - protagonist->getXPos();
        float deltaY = item->getYPos() - protagonist->getYPos();
        return std::sqrt(deltaX * deltaX + deltaY * deltaY);
    }
};

#endif // STRATEGY_H
