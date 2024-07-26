#ifndef MYXENEMYTEXTVIEW_H
#define MYXENEMYTEXTVIEW_H


#include "myenemytextview.h"

class MyXEnemyTextview : public MyEnemyTextview
{
public:
    explicit MyXEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance);
    ~MyXEnemyTextview() override = default;
};

#endif // MYXENEMYTEXTVIEW_H
