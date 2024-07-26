#ifndef MYPENEMYGRAPHICVIEW_H
#define MYPENEMYGRAPHICVIEW_H

#include "myenemygraphicview.h"

class MyPEnemyGraphicview: public MyEnemyGraphicview
{
    Q_OBJECT
public:
    explicit MyPEnemyGraphicview(std::shared_ptr<Enemy> enemy, int gridWidth);
    // Function to generate health pack items
    std::shared_ptr<QPixmap> returnStatusEnemy(const std::shared_ptr<Enemy>& enemy) override;
    void loadPic();
    bool poisoned;

public slots:
    void setPoisoned();

protected:
    std::shared_ptr<MyEnemyGraphicview> enemy;
    std::shared_ptr<QPixmap> pEnemy;
    std::shared_ptr<QPixmap> pEnemyDead;
};

#endif // MYPENEMYGRAPHICVIEW_H

