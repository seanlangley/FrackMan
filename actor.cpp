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

//ActivatingObject::ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
//                                   int soundToPlay, bool activateOnPlayer,
//                                   bool activateOnProtestor, bool initiallyActive)
//:Actor(world, startX, startY, )



////////////////////////////////////////////////////////////////////////////////
// FrackMan Implementation
////////////////////////////////////////////////////////////////////////////////

FrackMan::FrackMan(StudentWorld* world, int startX, int startY)
:Agent(world, startX, startY, right, IID_PLAYER, 1.0, 0, 100), m_score(0), m_numNuggets(0)
{}




void FrackMan::move()
{
    int ch;
    if(getWorld()->getKey(ch) == true)
    {
        switch(ch)
        {
            case KEY_PRESS_LEFT: go(left); break;
            case KEY_PRESS_RIGHT: go(right); break;
            case KEY_PRESS_DOWN: go(down); break;
            case KEY_PRESS_UP: go(up); break;
                
        }
    }
}



void FrackMan::go(Direction dir)
{
    switch(dir)
    {
        case left:
//            if(getWorld()->isThereBoulder(getX()-1, getY()) || getWorld()->isThereBoulder(getX()-1, getY()+3))
//                break;
            setDirection(left);
            if(getX() > 0)
                moveTo(getX()-1, getY());
            else
                moveTo(getX(), getY());
            break;
        case right:
            //            if(getWorld()->isThereBoulder(getX()+4, getY()) || getWorld()->isThereBoulder(getX()+4, getY()+3))
            //                break;
            setDirection(right);
            if(getX() < 60)
                moveTo(getX()+1, getY());
            else
                moveTo(getX(), getY());
            break;
        case down:
            //            if(getWorld()->isThereBoulder(getX(), getY()-1) || getWorld()->isThereBoulder(getX()+3, getY()-1))
            //                break;
            setDirection(down);
            if(getY() > 0)
                moveTo(getX(), getY()-1);
            else
                moveTo(getX(), getY());
            break;
        case up:
            //            if(getWorld()->isThereBoulder(getX(), getY()+4) || getWorld()->isThereBoulder(getX()+3, getY()+4))
            //                break;
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
    if(getWorld()->isThereDirt(x, y-1) && !moved)
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
        moved = true;
    }
    else
        setDead();
}


////////////////////////////////////////////////////////////////////////////////
// Oil Implementation
////////////////////////////////////////////////////////////////////////////////
OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
:ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL, true, false, false){}

void OilBarrel::move()
{
    if(! isAlive())
        return;
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
GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool temporary)
:ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, true, false, false){}

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


















