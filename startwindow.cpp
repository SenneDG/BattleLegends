#include "startwindow.h"
#include "mainwindow.h"
#include "ui_startwindow.h"
#include "levelmanager.h"
#include <QFileDialog>
#include <iostream>

StartWindow::StartWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StartWindow)
{
    ui->setupUi(this);

    setStyleSheet("StartWindow {"
                  "  background-image: url(:/images/images/background.jpg);"
                  "  background-repeat: no-repeat;"
                  "  background-position: center;"
                  "  background-size: cover;"
                  "}"
                  "QPushButton#startGameButton {"
                  "  background-color: #3498db;"  // Blue background color
                  "  color: white;"
                  "  border: 2px solid #3498db;"  // Blue border
                  "  border-radius: 10px;"
                  "  height: 60px;"  // Adjust the height as needed
                  "  margin-top: 20px;"  // Add margin for gap
                  "}"
                  "QPushButton#startGameButton:hover {"
                  "  background-color: #2980b9;"  // Darker blue on hover
                  "}");

    // Create a label with the text "Battle Legends"
    QLabel* titleLabel = new QLabel("Battle Legends", this);
    titleLabel->setStyleSheet("QLabel { color: yellow; font-size: 24px; }");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setGeometry(0, 50, width(), 50);

    connect(ui->startGameButton, SIGNAL(clicked()), this, SLOT(startGameClicked()));
}

StartWindow::~StartWindow()
{
    delete ui;
}

void StartWindow::startGameClicked()
{
    // Open a file dialog to get the selected file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.png);;All Files (*)"));
    std::cout << "StartWindow: " << filePath.toStdString() << std::endl;

    if (!filePath.isEmpty()) {
        // Create an instance of LevelManager based on the selected file
        std::shared_ptr<LevelManager> levelManager = std::make_shared<LevelManager>(3, filePath);

        // Disconnect the signal to prevent multiple calls
        disconnect(ui->startGameButton, SIGNAL(clicked()), this, SLOT(startGameClicked()));

        // Create an instance of MainWindow with the necessary parameters
        MainWindow* mainWindow = new MainWindow(levelManager->getController(), levelManager->getCurrentGameModel(), levelManager);

        // Show the main window
        mainWindow->show();

        // Close the current (Start) window if needed
        this->close();
    }
}


