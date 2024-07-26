#include "myprotagonist.h"

MyProtagonist::MyProtagonist()
    : Protagonist()
{
}

void MyProtagonist::handleHealthChange(const float& damage)
{
    float newHealth = this->getHealth() - damage;

    if (newHealth <= 0 && !isImmortal)
    {
        std::cout << "Protagonist is dead" << std::endl;
        this->setHealth(0);
        emit gameOver();

    } else if(!isImmortal)
    {
        this->setHealth(newHealth);
        this->setEnergy(100);

        float health = this->getHealth();
        std::cout << "Protagonist health: " << health << std::endl;
    }
}

void MyProtagonist::useHealthPack(const std::shared_ptr<MyHealthpack> & healthpack)
{
    if(!healthpack -> getConsumed())
    {
        int newHealth = this -> getHealth() + healthpack -> getHealingPower();
        std::cout << "healing power: " << healthpack -> getHealingPower() << "sum: " << newHealth << std::endl;

        if( newHealth < 100 )
        {
            this -> setHealth(newHealth);
            std::cout << "new health set " << newHealth << std::endl;
        }
        else
        {
            std::cout << "Health already 100 " << newHealth << std::endl;
            this -> setHealth(100);
        }
        healthpack -> setConsumed();
    }
    else
    {
        std::cout << "healthpack was already consumed" << std::endl;
    }

    std::cout << "health after : " << this -> getHealth() << std::endl;
}

void MyProtagonist::moveProtagonist(int xPos, int yPos)
{
    this->setPos(xPos, yPos);
}
