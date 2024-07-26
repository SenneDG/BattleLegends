#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QCompleter"
#include <qstringlistmodel.h>
#include <QLabel>
#include <QFileDialog>
#include <fstream>

MainWindow::MainWindow(std::shared_ptr<Controller> controller, std::shared_ptr<GameModel> gameModel, std::shared_ptr<LevelManager> levelManager  ,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,controller(controller)
    ,gameModel(gameModel)
    ,levelManager(levelManager)
{
    connect(levelManager.get(), &LevelManager::modelChanged, this, &MainWindow::handleModelChange);
    connect(gameModel->getProtagonist().get(), &MyProtagonist::healthChanged, this, &MainWindow::onHealthChanged);
    connect(gameModel->getProtagonist().get(), &MyProtagonist::energyChanged, this, &MainWindow::onEnergyChanged);
    connect(gameModel->getProtagonist().get(), &MyProtagonist::gameOver, this, &MainWindow::handleGameOver);


    ui->setupUi(this);

    //Command options
    QStringList commandList;
    commandList << "up" << "down" << "left" << "right" << "help" << "attack nearest enemy" << "take nearest health pack" ;

    //Autocompletion
    QCompleter* completer = new QCompleter(commandList,this);
    completer -> setCaseSensitivity(Qt::CaseInsensitive);

    // Create a QLineEdit widget for entering commands
    commandLineEdit = new QLineEdit(this);
    commandLineEdit->setPlaceholderText("Enter command...");
    commandLineEdit->setCompleter(completer);

    // Create a QPushButton for submitting the command
    submitButton = new QPushButton("Submit", this);

    // Connect the clicked signal of the submitButton to the onSubmitButtonClicked slot
    connect(submitButton, &QPushButton::clicked, this, &MainWindow::onSubmitButtonClicked);
    connect(this, &MainWindow::submitCommand, controller.get(), &Controller::handleCommand);

    toggleViewButton = new QPushButton("Toggle View", this);
    connect(toggleViewButton, &QPushButton::clicked, this, &MainWindow::toggleView);

    // Immortal Button
    immortalButton = new QPushButton("Immortal: False", this);
    connect(immortalButton, &QPushButton::clicked, this, &MainWindow::onImmortalButtonClicked);
    connect(this, &MainWindow::immortalToggle, levelManager->getCurrentGameModel().get(), &GameModel::onImmortalToggle);

    // Initialize the Textview and Graphicview with the provided gameModel
    textView = std::make_shared<TextView>(levelManager, this);
    graphicView = std::make_shared<Graphicview>(levelManager);
    pathfindingView = std::make_shared<Pathfindingview>(levelManager);

    // Create the stacked widget
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(graphicView.get());
    stackedWidget->addWidget(textView.get());
    stackedWidget->addWidget(pathfindingView.get());

    // Create health and energy progress bars
    healthBar = new QProgressBar(this);
    energyBar = new QProgressBar(this);

    healthBar -> setValue(100);
    energyBar -> setValue(100);

    healthLabel = new QLabel("Health:", this);
    energyLabel = new QLabel("Energy:", this);

    // Create heuristic cost and animation delay sliders
    animationDelaySlider = new QSlider(Qt::Horizontal, this);
    animationDelaySlider->setRange(10, 1000);  // Set the range as needed

    animationDelayLabel = new QLabel("Animation Delay: " + QString::number(10), this);

    heuristicCostSlider = new QSlider(Qt::Horizontal, this);
    heuristicCostSlider->setRange(0, 100);  // Set the range as needed

    heuristicCostLabel = new QLabel("Heuristic Cost: " + QString::number(0), this);

    connect(animationDelaySlider, &QSlider::valueChanged, this, &MainWindow::onAnimationDelayChanged);
    connect(this, &MainWindow::changeAnimationDelay, levelManager->getCurrentGameModel().get(), &GameModel::onAnimationDelayChanged);

    connect(heuristicCostSlider, &QSlider::valueChanged, this, &MainWindow::onHeuristicCostChanged);
    connect(this, &MainWindow::changeHeuristicCost, levelManager->getCurrentGameModel().get(), &GameModel::onHeuristicCostChanged);

    // Create pushbutton for toggling autoplay
    autoplayToggleButton = new QPushButton("Autoplay", this);
    connect(autoplayToggleButton, &QPushButton::clicked, this, &MainWindow::toggleAutoplay);
    connect(this, &MainWindow::autoplayToggle, controller.get(), &Controller::handleAutoPlay);

    // Create a layout to organize widgets
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(commandLineEdit);
    inputLayout->addWidget(submitButton);

    QHBoxLayout *barsLayout = new QHBoxLayout;
    barsLayout->addWidget(healthLabel);
    barsLayout->addWidget(healthBar);
    barsLayout->addWidget(energyLabel);
    barsLayout->addWidget(energyBar);

    QHBoxLayout *slidersLayout = new QHBoxLayout;
    slidersLayout->addWidget(animationDelayLabel);
    slidersLayout->addWidget(animationDelaySlider);
    slidersLayout->addWidget(heuristicCostLabel);
    slidersLayout->addWidget(heuristicCostSlider);

    QHBoxLayout *toggleViewLayout = new QHBoxLayout;
    toggleViewLayout->addWidget(immortalButton);
    toggleViewLayout->addWidget(toggleViewButton);
    toggleViewLayout->addWidget(autoplayToggleButton);

    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->addLayout(slidersLayout);
    centerLayout->addLayout(barsLayout);
    centerLayout->addWidget(stackedWidget);
    centerLayout->addLayout(toggleViewLayout);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(centerLayout);

    // Create a central widget and set the layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    startNewGame();
}

void MainWindow::onSubmitButtonClicked()
{
    QString enteredText = commandLineEdit->text();
    if (enteredText == "help") {
        displayHelp();
    } else {
        // Emit the submitCommand signal for other commands
        emit submitCommand(enteredText);
    }
}

void MainWindow::displayHelp()
{
    QString helpMessage =
        "Available Commands\n\n"
        "You can either type the following commands:\n"
        "------------------------\n"
        "• up    - Move protagonist up\n"
        "• down  - Move protagonist down\n"
        "• left  - Move protagonist left\n"
        "• right - Move protagonist right\n"
        "• autoplay  - use the autoplay function\n"
        "• goto(x,y)  - go to a certain position\n"
        "• help  - Display this help message\n"
        "------------------------\n"
        "Or use the arrows on your keyboard to move your protagonist\n"
        "You can also click somewhere with your mouse to navigate (not in textview)";

    QMessageBox helpBox;
    helpBox.setWindowTitle("Commands help");
    helpBox.setText(helpMessage);
    helpBox.exec();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onHealthChanged(int health)
{
    // Assuming health ranges from 0 to 100
    healthBar->setValue(health);
}

void MainWindow::onEnergyChanged(int energy)
{
    // Assuming energy ranges from 0 to 100
    energyBar->setValue(energy);
}

void MainWindow::handleGameOver() {

    std::cout << "in gameover" << std::endl;

    QPixmap pixmap(":/images/images/gameover.jpg");

    // Create and show the game over window
    QMessageBox gameOverBox;
    gameOverBox.setIconPixmap(pixmap);
    connect(&gameOverBox, &QMessageBox::accepted, this, &MainWindow::close);
    connect(&gameOverBox, &QMessageBox::accepted, &gameOverBox, &QMessageBox::close);

    gameOverBox.exec();
}

void MainWindow::toggleView() {
    int currentIndex = stackedWidget->currentIndex();

    int nextIndex = -1;

    // Determine the next index based on the current index
    if (currentIndex == stackedWidget->indexOf(textView.get())) {
        nextIndex = stackedWidget->indexOf(graphicView.get());
    } else if (currentIndex == stackedWidget->indexOf(graphicView.get())) {
        nextIndex = stackedWidget->indexOf(pathfindingView.get());
    } else if (currentIndex == stackedWidget->indexOf(pathfindingView.get())) {
        nextIndex = stackedWidget->indexOf(textView.get());
    }

    // Set the next index
    if (nextIndex != -1) {
        stackedWidget->setCurrentIndex(nextIndex);
    }
}

void MainWindow::onAnimationDelayChanged(int value)
{
    animationDelayLabel->setText("Animation Delay: " + QString::number(value));
    emit changeAnimationDelay(value);
}

void MainWindow::onHeuristicCostChanged(int value)
{
    heuristicCostLabel->setText("Heuristic Cost: " + QString::number(value));

    float distWeight = static_cast<float>(value) / 100.0f;
    emit changeHeuristicCost(distWeight);
}

void MainWindow::toggleAutoplay()
{
    emit autoplayToggle();
}

void MainWindow::onImmortalButtonClicked()
{
    ignoreGameOverSignal = !ignoreGameOverSignal;
    emit immortalToggle(ignoreGameOverSignal);
    immortalButton->setText(QString("Immortal: ") + (ignoreGameOverSignal ? "True" : "False"));
}

void MainWindow::handleModelChange(std::shared_ptr<GameModel> newModel){
    if(!(gameModel == newModel)){
        disconnect(gameModel->getProtagonist().get(), &MyProtagonist::gameOver, this, &MainWindow::handleGameOver);
        disconnect(this, &MainWindow::changeAnimationDelay, levelManager->getCurrentGameModel().get(), &GameModel::onAnimationDelayChanged);
        disconnect(this, &MainWindow::immortalToggle, levelManager->getCurrentGameModel().get(), &GameModel::onImmortalToggle);
        disconnect(this, &MainWindow::changeHeuristicCost, levelManager->getCurrentGameModel().get(), &GameModel::onHeuristicCostChanged);
        gameModel = newModel;
        connect(gameModel->getProtagonist().get(), &MyProtagonist::gameOver, this, &MainWindow::handleGameOver);
        connect(this, &MainWindow::changeHeuristicCost, levelManager->getCurrentGameModel().get(), &GameModel::onHeuristicCostChanged);
        connect(this, &MainWindow::immortalToggle, levelManager->getCurrentGameModel().get(), &GameModel::onImmortalToggle);
        connect(this, &MainWindow::changeAnimationDelay, levelManager->getCurrentGameModel().get(), &GameModel::onAnimationDelayChanged);
    }
}

void MainWindow::startNewGame() {

    // Show the main game widgets and hide the start screen
    stackedWidget->setCurrentWidget(graphicView.get());

    // Show elements specific to the game view
    commandLineEdit->show();
    animationDelaySlider->show();
    animationDelayLabel->show();
    heuristicCostSlider->show();
    heuristicCostLabel->show();
    submitButton->show();
    healthBar->show();
    healthLabel->show();
    energyBar->show();
    energyLabel->show();
    toggleViewButton->show();
    immortalButton->show();
    autoplayToggleButton->show();
}

