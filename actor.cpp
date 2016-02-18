#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////////////////////////
// Actor Implementation
////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int IID, int x, #include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

const int BOULDER = 0;
const int SQUIRT = 1;
const int BARREL_OF_OIL = 2;
const int GOLD_NUGGET = 3;
const int SONAR_KIT = 4;
const int WATER_POOL = 5;
const int REGULAR_PROTESTOR = 6;
const int HARDCORE_PROTESTOR = 7;

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
{
    return;
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
:Actor(IID_PLAYER, 30,60, right, true, 1.0, 0, sw), m_health(100)
{}

FrackMan::~FrackMan(){return;}



void FrackMan::doSomething()
{
    int ch;
    if(getWorld()->getKey(ch) == true)
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT: move(left); break;
            case KEY_PRESS_RIGHT: move(right); break;
            case KEY_PRESS_DOWN: move(down); break;
            case KEY_PRESS_UP: move(up); break;
                
        }
    }
}

void FrackMan::getAnnoyed(){return;}

void FrackMan::move(Direction dir)
{
    switch(dir)
    {
        case left:
            if(getWorld()->isThereActor(getX()-1, getY())
               || getWorld()->isThereActor(getX()-1, getY()+3))
                break;
            setDirection(left);
            if(getX() > 0)
                moveTo(getX()-1, getY());
            else
                moveTo(getX(), getY());
            break;
        case right:
            if(getWorld()->isThereActor(getX()+4, getY()) || getWorld()->isThereActor(getX()+4, getY()+3))
                break;
            setDirection(right);
            if(getX() < 60)
                moveTo(getX()+1, getY());
            else
                moveTo(getX(), getY());
            break;
        case down:
            if(getWorld()->isThereActor(getX(), getY()-1) || getWorld()->isThereActor(getX()+3, getY()-1))
                break;
            setDirection(down);
            if(getY() > 0)
                moveTo(getX(), getY()-1);
            else
                moveTo(getX(), getY());
            break;
        case up:
            if(getWorld()->isThereActor(getX(), getY()+4) || getWorld()->isThereActor(getX()+3, getY()+4))
                break;
            setDirection(up);
            if(getY() < 60)
                moveTo(getX(), getY()+1);
            else
                moveTo(getX(), getY());
            break;
        case none:
            break;
    }
}


void FrackMan::react(int Actor)
{
    if(Actor == BOULDER)
        setDead();
    if(Actor == SQUIRT)
        return;
    if(Actor == BARREL_OF_OIL)
        return;
    if(Actor == GOLD_NUGGET)
        return;
    if(Actor == SONAR_KIT)
        return;
    if(Actor == WATER_POOL)
        return;
    if(Actor == REGULAR_PROTESTOR)
        return;
    if(Actor == HARDCORE_PROTESTOR)
        return;
}



////////////////////////////////////////////////////////////////////////////////
// Boulder Implementation
////////////////////////////////////////////////////////////////////////////////

Boulder::Boulder(int x, int y, StudentWorld* sw)
:Actor(IID_BOULDER, x,y, down, true, 1.0, 1, sw), m_state(STABLE)
{}

void Boulder::doSomething()
{
    if(m_state != STABLE)
        return;
    int x = getX(), y = getY();
    if(getWorld()->isThereDirt(x, y-1))
        return;
    while(! getWorld()->isThereDirt(getX(), getY()-1))
          moveTo(getX(), getY()-1);
            if(getWorld()->getPlayer()->getX() == getX() && getWorld()->getPlayer()->getY() == getY())
                getWorld()->getPlayer()->react(BOULDER);
           
    
    
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














int y, Direction dir, bool isVisible, float size, unsigned  int depth,  StudentWorld* sw)
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














