#ifndef MYPROTAGONISTGRAPHICSVIEW_H
#define MYPROTAGONISTGRAPHICSVIEW_H

#include "myprotagonist.h"
#include "qpixmap.h"
#include "world.h"
#include <QString>
#include <QColor>
#include <QObject>
#include <QTimer>
#include "graphicview.h"
#include <QString>
#include <QColor>
#include <QObject>
#include <QTimer>



class myprotagonistgraphicsview: public MyProtagonist
{
    Q_OBJECT
public:
    myprotagonistgraphicsview(int gridwidth);
    void loadPic();
    std::shared_ptr<QPixmap> protAlive;
    std::shared_ptr<QPixmap> protPoison;
    std::shared_ptr<QPixmap> protHealth;
    std::shared_ptr<QPixmap> protAttack;
    int gridwidth;
    std::shared_ptr<GameModel> GameModel;
    std::shared_ptr<QPixmap> returnStatusProt(std::shared_ptr<Protagonist>& prot, int status);
private:
    void applyColorFilter(QPixmap& pixmap, const QColor& color);
};



#endif // MYPROTAGONISTGRAPHICSVIEW_H
