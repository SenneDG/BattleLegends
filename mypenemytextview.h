#ifndef MYPENEMYTEXTVIEW_H
#define MYPENEMYTEXTVIEW_H

#include "myenemytextview.h"

class PEnemyTextview : public MyEnemyTextview
{
public:
    explicit PEnemyTextview(const std::shared_ptr<Enemy>& enemyInstance);
    ~PEnemyTextview() override = default;
};


#endif // MYPENEMYTEXTVIEW_H
