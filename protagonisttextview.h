#ifndef PROTAGONIST_TEXTVIEW_H
#define PROTAGONIST_TEXTVIEW_H

#include "world.h"
#include <QString>
#include <QColor>
#include <QObject>
#include <QTimer>

class ProtagonistTextview : public QObject
{
    Q_OBJECT
public:
    explicit ProtagonistTextview(const std::shared_ptr<Protagonist>& protagonistInstance);

    std::pair<QString, QColor> getDisplayInfo() const;

signals:
    void displayInfoChanged();

private slots:
    void handlePosChanged(int x, int y);
    void handleHealthChanged(int h);
    void handleEnergyChanged(int e);
    void resetColor();

private:
    std::shared_ptr<Protagonist> protagonistInstance;
    QString displayString;
    QColor color;
    QTimer colorTimer;
};

#endif // PROTAGONIST_TEXTVIEW_H

