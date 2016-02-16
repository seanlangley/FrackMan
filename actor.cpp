#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////////////////////////
// Actor Implementation
////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int IID, int x, int y, Direction dir, bool isVisible, float size, unsigned  int depth,  StudentWorld* sw)
:GraphObject(IID, x,y, dir, size, depth), m_world(sw)
{
    if(sw == nullptr)
        exit(1);
    setVisible(isVisible);
}

Actor::~Actor()
{return;
//    setVisible(false);
    
}


////////////////////////////////////////////////////////////////////////////////
// Dirt Implementation
////////////////////////////////////////////////////////////////////////////////

Dirt::Dirt(int x, int y, StudentWorld* sw)
:Actor(IID_DIRT, x,y, right, true, .25, 3,  sw)
{}

Dirt::~Dirt(){return;}



////////////////////////////////////////////////////////////////////////////////
// FrackMan Implementation
////////////////////////////////////////////////////////////////////////////////

FrackMan::FrackMan(StudentWorld* sw)
:Actor(IID_PLAYER, 30,60, right, true, 1.0, 0, sw), m_health(10)
{}

FrackMan::~FrackMan(){return;}


void FrackMan::doSomething()
{
    int ch;
    if(getWorld()->getKey(ch) == true)
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                setDirection(left);
                if(getX() > 0)
                    moveTo(getX()-1, getY());
                else
                    moveTo(getX(), getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                if(getX() < 60)
                    moveTo(getX()+1, getY());
                else
                    moveTo(getX(), getY());
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                if(getY() > 0)
                    moveTo(getX(), getY()-1);
                else
                    moveTo(getX(), getY());
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                if(getY() < 60)
                    moveTo(getX(), getY()+1);
                else
                    moveTo(getX(), getY());
                break;
                
        }
    }
}






void FrackMan::getAnnoyed(){return;}

////////////////////////////////////////////////////////////////////////////////
// Boulder Implementation
////////////////////////////////////////////////////////////////////////////////

Boulder::Boulder(int x, int y, StudentWorld* sw)
:Actor(IID_BOULDER, x,y, down, true, 1.0, 1, sw), m_isAlive(true)
{}

void Boulder::doSomething()
{
    if(! m_isAlive)
        return;
    
    
    
    
}

////////////////////////////////////////////////////////////////////////////////
// Squirt Implementation
////////////////////////////////////////////////////////////////////////////////
Squirt::Squirt(int x, int y, Direction dir, StudentWorld* sw)
:Actor(IID_WATER_SPURT, x,y, dir, true ,1.0, 1, sw)
{
    
}

////////////////////////////////////////////////////////////////////////////////
// Barrel of Oil Implementation
////////////////////////////////////////////////////////////////////////////////
BarrelOfOil::BarrelOfOil(int x, int y, StudentWorld* sw)
:Actor(IID_BARREL, x,y, right, false, 1.0, 2, sw)
{
    
}


////////////////////////////////////////////////////////////////////////////////
// Gold Nugget Implementation
////////////////////////////////////////////////////////////////////////////////
GoldNugget::GoldNugget(int x, int y, bool isVisible, StudentWorld* sw)
:Actor(IID_GOLD, x,y, right, isVisible, 1.0, 2, sw)
{
    
}


////////////////////////////////////////////////////////////////////////////////
// Sonar Kit Implementation
////////////////////////////////////////////////////////////////////////////////
SonarKit::SonarKit(int x, int y, StudentWorld* sw)
:Actor(IID_SONAR, x,y, right, true, 1.0, 2, sw)
{
    
}


////////////////////////////////////////////////////////////////////////////////
// Water Pool Implementation
////////////////////////////////////////////////////////////////////////////////
WaterPool::WaterPool(int x, int y, StudentWorld* sw)
:Actor(IID_WATER_POOL, x,y, right, true, 1.0, 2, sw)
{
    
}


////////////////////////////////////////////////////////////////////////////////
// Regular Protestor Implementation
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
// Hardcore Protestor Implementation
////////////////////////////////////////////////////////////////////////////////














