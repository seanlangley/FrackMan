#include "Actor.h"
#include "StudentWorld.h"


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



////////////////////////////////////////////////////////////////////////////////
// Actor Implementation
////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int IID, int x, int y, Direction dir, float size, unsigned  int depth,  StudentWorld* sw)
:GraphObject(IID, x,y, dir, size, depth), m_world(sw), m_dead(false)
{
    if(sw == nullptr)
        exit(1);
    setVisible(true);
}

Actor::~Actor()
{
    return;
}


////////////////////////////////////////////////////////////////////////////////
// Dirt Implementation
////////////////////////////////////////////////////////////////////////////////

Dirt::Dirt(int x, int y, StudentWorld* sw)
:Actor(IID_DIRT, x,y, right, .25, 3,  sw)
{}

Dirt::~Dirt(){return;}



////////////////////////////////////////////////////////////////////////////////
// FrackMan Implementation
////////////////////////////////////////////////////////////////////////////////

FrackMan::FrackMan(StudentWorld* sw)
:Actor(IID_PLAYER, 30,60, right, 1.0, 0, sw), m_health(100), m_score(0)
{}

FrackMan::~FrackMan(){return;}

void FrackMan::setDead()
{
    m_health = 0;
    getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
}

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
            if(getWorld()->isThereBoulder(getX()-1, getY()) || getWorld()->isThereBoulder(getX()-1, getY()+3))
                break;
            setDirection(left);
            if(getX() > 0)
                moveTo(getX()-1, getY());
            else
                moveTo(getX(), getY());
            break;
        case right:
            if(getWorld()->isThereBoulder(getX()+4, getY()) || getWorld()->isThereBoulder(getX()+4, getY()+3))
                break;
            setDirection(right);
            if(getX() < 60)
                moveTo(getX()+1, getY());
            else
                moveTo(getX(), getY());
            break;
        case down:
            if(getWorld()->isThereBoulder(getX(), getY()-1) || getWorld()->isThereBoulder(getX()+3, getY()-1))
                break;
            setDirection(down);
            if(getY() > 0)
                moveTo(getX(), getY()-1);
            else
                moveTo(getX(), getY());
            break;
        case up:
            if(getWorld()->isThereBoulder(getX(), getY()+4) || getWorld()->isThereBoulder(getX()+3, getY()+4))
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
    {
        setDead();
    }
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

bool FrackMan::isWithinRadius(Actor* actor)
{
    int x = actor->getX();
    int y = actor->getY();
    for(int k = 0; k < 4; k++)
        for(int i = 0; i < 4; i++)
            if((x+4+k == getX() && x+4+y == getY()) || (x-k == getX() && y-i == getY()))
                return true;
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// Boulder Implementation
////////////////////////////////////////////////////////////////////////////////

Boulder::Boulder(int x, int y, StudentWorld* sw)
:Actor(IID_BOULDER, x,y, down, 1.0, 1, sw), m_state(STABLE), m_count(0)
{}

void Boulder::doSomething()
{
    if(m_state != STABLE)
        return;
    int x = getX(), y = getY();
    if(getWorld()->isThereDirt(x, y-1))
        return;
    
    if(! getWorld()->isThereDirt(getX(), getY()-1))
       {
           if(m_count < 29)
           {
               m_count++;
               return;
           }
           
           else if(m_count == 29)
           {
               getWorld()->playSound(SOUND_FALLING_ROCK);
               m_count++;
               
           }
           else if(m_count > 29)
           {
               moveTo(getX(), getY()-1);
           }
           
       }
    setDead();

}

////////////////////////////////////////////////////////////////////////////////
// Squirt Implementation
////////////////////////////////////////////////////////////////////////////////
Squirt::Squirt(int x, int y, Direction dir, StudentWorld* sw)
:Actor(IID_WATER_SPURT, x,y, dir, 1.0, 1, sw)
{
    
}

////////////////////////////////////////////////////////////////////////////////
// Barrel of Oil Implementation
////////////////////////////////////////////////////////////////////////////////
BarrelOfOil::BarrelOfOil(int x, int y, StudentWorld* sw)
:Actor(IID_BARREL, x,y, right, 1.0, 2, sw)
{}

void BarrelOfOil::doSomething()
{
    if(isDead())
        return;
    if(isVisible() == true && getWorld()->getPlayer()->isWithinRadius(this))
    {
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->getPlayer()->increaseScore(1000);
        getWorld()->gotBarrel();
        setDead();
    }
}

void BarrelOfOil::getAnnoyed(){return;}


////////////////////////////////////////////////////////////////////////////////
// Gold Nugget Implementation
////////////////////////////////////////////////////////////////////////////////
GoldNugget::GoldNugget(int x, int y, bool isVisible, StudentWorld* sw)
:Actor(IID_GOLD, x,y, right, 1.0, 2, sw)
{
    setVisible(isVisible);
}


////////////////////////////////////////////////////////////////////////////////
// Sonar Kit Implementation
////////////////////////////////////////////////////////////////////////////////
SonarKit::SonarKit(int x, int y, StudentWorld* sw)
:Actor(IID_SONAR, x,y, right, 1.0, 2, sw)
{
    
}


////////////////////////////////////////////////////////////////////////////////
// Water Pool Implementation
////////////////////////////////////////////////////////////////////////////////
WaterPool::WaterPool(int x, int y, StudentWorld* sw)
:Actor(IID_WATER_POOL, x,y, right, 1.0, 2, sw)
{
    
}


////////////////////////////////////////////////////////////////////////////////
// Regular Protestor Implementation
////////////////////////////////////////////////////////////////////////////////
RegularProtestor::RegularProtestor(int x, int y, StudentWorld* sw)
:Actor(IID_PROTESTER, x,y, left, 1.0, 0, sw)
{}


////////////////////////////////////////////////////////////////////////////////
// Hardcore Protestor Implementation
////////////////////////////////////////////////////////////////////////////////
HardcoreProtestor::HardcoreProtestor(int x, int y, StudentWorld* sw)
:RegularProtestor(x,y, sw)
{}













