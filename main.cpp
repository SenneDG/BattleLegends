#include <QApplication>
#include "startwindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    StartWindow startWindow;
    startWindow.show();

    return a.exec();
}




