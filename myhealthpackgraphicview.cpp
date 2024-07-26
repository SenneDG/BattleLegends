#include "myhealthpackgraphicview.h"
#include <QGraphicsPixmapItem>

MyHealthpackGraphicview::MyHealthpackGraphicview(const std::shared_ptr<MyHealthpack> healthpack, int gridWidth)
    :MyHealthpack(healthpack), gridWidth(gridWidth)
{
    healthpackGraphicviewUnused = std::make_shared<QPixmap>();
    healthpackGraphicviewUsed = std::make_shared<QPixmap>();
    loadPic();
    returnStatusHealthpack(healthpack);
}

QPixmap* MyHealthpackGraphicview::returnStatusHealthpack(const std::shared_ptr<MyHealthpack>& healthpack)
 {
    if (healthpack->getConsumed()==false){
        return healthpackGraphicviewUnused.get();
    }
    else{
        return healthpackGraphicviewUsed.get();
    }

}

 void MyHealthpackGraphicview::loadPic(){
    QPixmap healthpackunusedQP(":/images/images/healthpack.png");
    healthpackGraphicviewUnused = std::make_shared<QPixmap>(healthpackunusedQP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));
    QPixmap healthpackusedQP(":/images/images/healthpackused.png");
    healthpackGraphicviewUsed = std::make_shared<QPixmap>(healthpackusedQP.scaled(gridWidth, gridWidth, Qt::KeepAspectRatio));

 }
