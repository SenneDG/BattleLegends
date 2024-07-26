#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QSlider>
#include <QLabel>
#include <qtextedit.h>
#include <memory>
#include <QMessageBox>
#include <QStackedWidget>
#include <QToolBar>
#include "textview.h"
#include "graphicview.h"
#include "gamemodel.h"
#include "controller.h"
#include "pathfindingview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(std::shared_ptr<Controller> controller = nullptr, std::shared_ptr<GameModel> gameModel = nullptr, std::shared_ptr<LevelManager> levelManager = nullptr, QWidget *parent = nullptr);
    ~MainWindow();

    std::shared_ptr<Pathfindingview> getPathfindingView(){return pathfindingView;}

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Controller> controller;
    std::shared_ptr<GameModel> gameModel;
    std::shared_ptr<LevelManager> levelManager;
    std::shared_ptr<TextView> textView;
    std::shared_ptr<Graphicview> graphicView;
    std::shared_ptr<Pathfindingview> pathfindingView;

    QStackedWidget *stackedWidget;
    World* world;
    QLineEdit* commandLineEdit;
    QFrame *mainFrame;
    QPushButton* submitButton;
    QProgressBar* healthBar;
    QProgressBar* energyBar;
    QLabel* healthLabel;
    QLabel* energyLabel;
    QSlider *animationDelaySlider;
    QLabel *animationDelayLabel;
    QSlider *heuristicCostSlider;
    QLabel *heuristicCostLabel;
    QPushButton* autoplayToggleButton;
    QPushButton* toggleViewButton;
    QPushButton* immortalButton;

    bool ignoreGameOverSignal = false;
    void displayHelp();

private slots:
    void onSubmitButtonClicked();
    void onHealthChanged(int health);
    void onEnergyChanged(int energy);
    void handleGameOver();
    void handleModelChange(std::shared_ptr<GameModel> newModel);
    void toggleView();
    void onAnimationDelayChanged(int value);
    void onHeuristicCostChanged(int value);
    void toggleAutoplay();
    void startNewGame();
    void onImmortalButtonClicked();

signals:
    void changeAnimationDelay(int & value);
    void changeHeuristicCost(float & distWeight);
    void submitCommand(QString command);
    void autoplayToggle();
    void immortalToggle(bool enabled);
};

#endif // MAINWINDOW_H
