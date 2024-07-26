// ProtagonistTextview.cpp
#include "protagonisttextview.h"
#include <iostream>

ProtagonistTextview::ProtagonistTextview(const std::shared_ptr<Protagonist>& protagonistInstance)
    : protagonistInstance(protagonistInstance),
    displayString("P"),
    color(Qt::green),  // Default color, can be changed as needed
    colorTimer(this)
{
    // Connect the signals to the slots
    connect(protagonistInstance.get(), &Protagonist::posChanged, this, &ProtagonistTextview::handlePosChanged);
    connect(protagonistInstance.get(), &Protagonist::healthChanged, this, &ProtagonistTextview::handleHealthChanged);
    connect(protagonistInstance.get(), &Protagonist::energyChanged, this, &ProtagonistTextview::handleEnergyChanged);

    connect(&colorTimer, &QTimer::timeout, this, &ProtagonistTextview::resetColor);

}

std::pair<QString, QColor> ProtagonistTextview::getDisplayInfo() const
{
    return {displayString, color};
}

void ProtagonistTextview::handlePosChanged(int x, int y)
{
    // Handle position change, if needed
    emit displayInfoChanged();
}

void ProtagonistTextview::handleHealthChanged(int h)
{
    // Handle health change, and set the color to red
    if(h == 0)
    {
        displayString = "X";
    }
    else
    {
        color = Qt::red;
        // Start the timer to reset the color after 1000 milliseconds (1 second)
        colorTimer.start(300);
    }

    // Emit the signal to update the display
    std::cout << "protagonist health" << protagonistInstance -> getHealth() << "health" << h << std::endl;
    emit displayInfoChanged();

}

void ProtagonistTextview::handleEnergyChanged(int e)
{
    //no implementation provided (showing animation each time would not be usefull)
}

void ProtagonistTextview::resetColor()
{
    // Reset the color back to green
    color = Qt::green;

    // Stop the timer (it won't trigger anymore)
    colorTimer.stop();

    // Emit the signal to update the display
   emit displayInfoChanged();
}


