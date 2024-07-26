// MyHealthpackTextview.cpp

#include "MyHealthpackTextview.h"
#include <QColor>

MyHealthpackTextview::MyHealthpackTextview(const std::shared_ptr<MyHealthpack>& healthpack)
    :  displayString("H"), color(Qt::green), healthpackInstance(healthpack)
{
    //Check if healthpack was already consumed (when switching levels)
    if( healthpackInstance -> getConsumed() )
    {
        displayString = " ";
    }
    MyHealthpack::connect(healthpackInstance.get(), &MyHealthpack::consumedChanged, this, &MyHealthpackTextview::handleHealthpackConsumed);
}

std::pair<QString, QColor> MyHealthpackTextview::getDisplayInfo()
{
    // Return the current display information
    return std::make_pair(displayString, QColor("green"));
}

void MyHealthpackTextview::handleHealthpackConsumed()
{
    // Update display string when healthpack is consumed
    displayString = " ";
}

std::shared_ptr<MyHealthpack> MyHealthpackTextview::getHealthpack(){
    return healthpackInstance;
}


