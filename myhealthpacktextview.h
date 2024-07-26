#ifndef MYHEALTHPACKTEXTVIEW_H
#define MYHEALTHPACKTEXTVIEW_H

#include <QString>
#include "myhealthpack.h"

class MyHealthpackTextview : public QObject
{
    Q_OBJECT
public:
    MyHealthpackTextview(const std::shared_ptr<MyHealthpack>& healthpack );
    std::pair<QString, QColor> getDisplayInfo();
    std::shared_ptr<MyHealthpack> getHealthpack();

public slots:
    void handleHealthpackConsumed();

private:
    QString displayString;
    QColor color;
    std::shared_ptr<MyHealthpack> healthpackInstance;
};

#endif // MYHEALTHPACKTEXTVIEW_H
