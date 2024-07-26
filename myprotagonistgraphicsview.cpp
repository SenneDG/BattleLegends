#include "myprotagonistgraphicsview.h"
#include "qpainter.h"
#include "qpixmap.h"

myprotagonistgraphicsview::myprotagonistgraphicsview(int gridwidth)
    :gridwidth(gridwidth)
{
    loadPic();
}

std::shared_ptr<QPixmap> myprotagonistgraphicsview::returnStatusProt(std::shared_ptr<Protagonist>& prot,int status)
{
    if(status==0)return protAlive;
    else if(status==1)return protHealth;
    else if(status==2)return protAttack;
    else if(status==3)return protPoison;
    else return protAlive;
}


void myprotagonistgraphicsview::loadPic()
{
    QPixmap Mario(":/images/images/mario.png");
    protAlive = std::make_shared<QPixmap>(Mario.scaled(gridwidth, gridwidth, Qt::KeepAspectRatio));
    protPoison = std::make_shared<QPixmap>(*protAlive);
    protHealth = std::make_shared<QPixmap>(*protAlive);
    protAttack = std::make_shared<QPixmap>(*protAlive);
    // Apply color filters
    applyColorFilter(*protAttack, Qt::red);
    applyColorFilter(*protPoison, QColorConstants::Svg::purple);
    applyColorFilter(*protHealth, Qt::green);
}

void myprotagonistgraphicsview::applyColorFilter(QPixmap& pixmap, const QColor& color) {
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    painter.end();
}
