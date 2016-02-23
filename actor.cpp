#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

////////////////////////////////////////////////////////////////////////////////
// Actor Implementation
////////////////////////////////////////////////////////////////////////////////

Actor::Actor(StudentWorld* world, int startX, int startY, Direction startDir,
             bool visible, int imageID, double size, int depth)
:GraphObject(imageID, startX,startY, startDir, size, depth), m_world(world), m_isAlive(true)
{
    if(world == nullptr)
        exit(1);
    setVisible(true);
}





////////////////////////////////////////////////////////////////////////////////
// FrackMan Implementation
////////////////////////////////////////////////////////////////////////////////

FrackMan::FrackMan(StudentWorld* world)
:Agent(world, 30, 60, right, IID_PLAYER, 1.0, 0, 100), m_score(0), m_numNuggets(0), m_numWater(5)
{}




void FrackMan::move()
{
    
    int ch;
    getGoodie();
    if(getWorld()->getKey(ch) == true)
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT: go(left); break;
            case KEY_PRESS_RIGHT: go(right); break;
            case KEY_PRESS_DOWN: go(down); break;
            case KEY_PRESS_UP: go(up); break;
            case KEY_PRESS_TAB: dropGold(getDirection()); break;
            case KEY_PRESS_SPACE: shootGun(); break;
               
                
        }
    }
    
    
}

void FrackMan::shootGun()
{
    if(m_numWater <= 0)
        return;
    int x = 0, y = 0;
    Direction dir;
    switch(getDirection())
    {
        case up: y = 3; dir = up; break;
        case down: y = -1; dir = down; break;
        case left: x = -1; dir = left; break;
        case right: x = 4; dir = right; break;
        case none: break;
    }
    Squirt* temp = new Squirt(getWorld(), getX() + x, getY() + y, dir);
    getWorld()->addActor(temp);
    
}

void FrackMan::getGoodie()
{
    if(getWorld()->isThereActor(getX(), getY()))
    {
        Actor* temp = getWorld()->getActor(getX(), getY());
        if(temp->canIBePickedUp() == false)
            return;
        int IID = temp->getID();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        
        switch(IID)
        {
            case IID_GOLD: addGold(); temp->setDead(); break;
            case IID_SONAR: addSonar(); temp->setDead(); break;
            case IID_WATER_POOL: addWater(); temp->setDead(); break;
            default: break;
                
        }
        
    }
        
}

void FrackMan::dropGold(GraphObject::Direction dir)
{

    if(getGold() <= 0)
        return;
    m_numNuggets--;
    switch(dir)
    {
        case up:
            getWorld()->addActor(new GoldNugget(getWorld(), getX(), getY()+3)); break;
        case down:
            getWorld()->addActor(new GoldNugget(getWorld(), getX(), getY()-3)); break;
        case left:
            getWorld()->addActor(new GoldNugget(getWorld(), getX()-3, getY())); break;
        case right:
            getWorld()->addActor(new GoldNugget(getWorld(), getX()+4, getY())); break;
        case none: break;
            
        
    }
}

void FrackMan::go(Direction dir)
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

////////////////////////////////////////////////////////////////////////////////
// Squirt Implementation
////////////////////////////////////////////////////////////////////////////////
Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir)
:Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1.0, 1), m_travelDistance(4){}

void Squirt::move()
{

    
    if(getTravelDistance() > 0)
    {
        switch(getDirection())
        {
            case up:
                moveTo(getX(), getY()+1);  break;
            case down:
                moveTo(getX(), getY()-1); break;
            case left:
                moveTo(getX()-1, getY()); break;
            case right:
                moveTo(getX()+1, getY()); break;
            case none: break;
            default: break;
        }
        decreaseTravelDistance();
    }
    else if(getTravelDistance() == 0)
        setDead();

}


////////////////////////////////////////////////////////////////////////////////
// Dirt Implementation
////////////////////////////////////////////////////////////////////////////////

Dirt::Dirt(StudentWorld* world, int startX, int startY)
:Actor(world, startX, startY, right, true, IID_DIRT, .25, 3)
{}


////////////////////////////////////////////////////////////////////////////////
// Boulder Implementation
////////////////////////////////////////////////////////////////////////////////


Boulder::Boulder(StudentWorld* world, int startX, int startY)
:Actor(world, startX, startY, down, true, IID_BOULDER, 1.0, 1), m_count(0){moved = false;}



void Boulder::move()
{
    

    int x = getX();
    int y = getY();
    if(getWorld()->isThereDirt(x, y-1) && ! moved)
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
            if(getWorld()->isThereFrackMan(getX(), getY()))
               getWorld()->getPlayer()->setDead();
            
            
        }
        
        moved = true;
    }

    else
        setDead();
}


////////////////////////////////////////////////////////////////////////////////
// Oil Implementation
////////////////////////////////////////////////////////////////////////////////
OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
:ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL, true, false, false)
{setVisible(true);}

void OilBarrel::move()
{
    if(! isAlive())
        return;
    if(isVisible() == false && getWorld()->isNearFrackMan(this, 10))
       setVisible(true);
    if(isVisible() == true && getWorld()->isNearFrackMan(this, 5))
    {
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->getPlayer()->increaseScore(1000);
        getWorld()->gotBarrel();
        setDead();
    }
}

bool OilBarrel::needsToBePickedUpToFinishLevel()const {return true;}

////////////////////////////////////////////////////////////////////////////////
// Gold Nugget Implementation
////////////////////////////////////////////////////////////////////////////////
GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY)
:ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, true, false, false), dropped(false)
{
}

void GoldNugget::move()
{
    if(! isAlive())
        return;
    if(isVisible() == true && getWorld()->isNearFrackMan(this, 5))
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->increaseScore(10);
        setDead();
    }
}


////////////////////////////////////////////////////////////////////////////////
// Sonar Kit Implementation
////////////////////////////////////////////////////////////////////////////////
SonarKit::SonarKit(StudentWorld* world, int startX, int startY)
:ActivatingObject(world, startX, startY, IID_SONAR, SOUND_GOT_GOODIE, true, false, false){}


////////////////////////////////////////////////////////////////////////////////
// Protester Implementation
////////////////////////////////////////////////////////////////////////////////
Protester::Protester(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score)
:Agent(world, startX, startY, left, imageID, 1.0, 0, 5), m_numSquaresToMoveInCurrentDirection(8){}




////////////////////////////////////////////////////////////////////////////////
// Regular Protester Implementation
////////////////////////////////////////////////////////////////////////////////
RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY)
:Protester(world, startX, startY, IID_PROTESTER, 5, 0){}

void RegularProtester::move()
{
    Direction startDir = getDirection();
    while(getSquaresToMove() > 0)
    {
        if(! getWorld()->isThereDirt(getX(), getY()) && startDir == left)
            moveTo(getX()-1, getY());
        decreaseSquaresToMove();
        return;

    }
    
    setSquaresToMove();
}


////////////////////////////////////////////////////////////////////////////////
// Hardcore Protester Implementation
////////////////////////////////////////////////////////////////////////////////
HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY)
:Protester(world, startX, startY, IID_HARD_CORE_PROTESTER, 5, 0){}

















