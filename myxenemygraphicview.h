#ifndef MYXENEMYGRAPHICVIEW_H
#define MYXENEMYGRAPHICVIEW_H

#include "myenemygraphicview.h"

class MyXEnemyGraphicview: public MyEnemyGraphicview
{
    Q_OBJECT
public:
    explicit MyXEnemyGraphicview(std::shared_ptr<Enemy> enemy, int gridWidth);
    // Function to generate health pack items
    std::shared_ptr<QPixmap> returnStatusEnemy(const std::shared_ptr<Enemy>& enemy) override;
    void loadPics();

protected:
    std::shared_ptr<MyEnemyGraphicview> enemy;
    std::shared_ptr<QPixmap> xEnemy;
    std::shared_ptr<QPixmap> xEnemyDead;
};
#endif // MYXENEMYGRAPHICVIEW_H
