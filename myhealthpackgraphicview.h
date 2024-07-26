#ifndef MYHEALTHPACKGRAPHICVIEW_H
#define MYHEALTHPACKGRAPHICVIEW_H
#include <QGraphicsPixmapItem>
#include "myhealthpack.h"


class MyHealthpackGraphicview: public MyHealthpack
{
    Q_OBJECT
public:
    explicit MyHealthpackGraphicview(std::shared_ptr<MyHealthpack> healthpack, int gridWidth);
    ~MyHealthpackGraphicview() override = default;

    void loadPic();
    QPixmap* returnStatusHealthpack(const std::shared_ptr<MyHealthpack>& healthpack);

protected:
    const std::shared_ptr<MyHealthpack> healthpack;
    int gridWidth;
    std::shared_ptr<QPixmap> healthpackGraphicviewUnused;
    std::shared_ptr<QPixmap> healthpackGraphicviewUsed;
};

#endif // MYHEALTHPACKGRAPHICVIEW_H
