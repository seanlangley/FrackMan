#include "Actor.h"
#include "StudentWorld.h"
#include <queue>

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
    setVisible(visible);
}


////////////////////////////////////////////////////////////////////////////////
// Agent Implementation
////////////////////////////////////////////////////////////////////////////////
Agent::Agent(StudentWorld* world, int startX, int startY, Direction startDir, int imageID, double size, int depth, unsigned int hitPoints)
:Actor(world, startX, startY, startDir, true, imageID, size, depth), m_hitPoints(hitPoints)
{
    
    
    for(int i = 0; i < 64; i++)
        for(int k = 0; k < 64; k++)
        {
            if(getWorld()->isThereDirtExact(i,k))
                thisMap[63-k][63-i] = 'X';
            else
                thisMap[63-k][63-i] = '.';
        }
    
    //    for(int i = 0; i < 64; i++)
    //    {
    //        for(int j = 0; j < 64; j++)
    //            cout << thisMap[i][j];
    //        cout << endl;
    //    }
}


void Agent::addGold()
{

    m_numNuggets++; getWorld()->increaseScore(10);

}

bool Agent::annoy(unsigned int amt)
{
    m_hitPoints -= amt;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// End of Agent Implementation
////////////////////////////////////////////////////////////////////////////////





////////////////////////////////////////////////////////////////////////////////
// Activating Object Implementation
////////////////////////////////////////////////////////////////////////////////

ActivatingObject::ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
                                   int soundToPlay, bool activateOnPlayer,
                                   bool activateOnProtester, bool initallyActive, Direction dir, bool visible)
:Actor(world, startX, startY, dir, visible, imageID, 1.0, 1)
{
    switch(imageID)
    {
        case IID_GOLD:
        case IID_BARREL: m_ticksToLive = -1; break;
        case IID_SONAR:
        case IID_WATER_POOL:
            if(300 - getWorld()->getLevel() < 100)
                m_ticksToLive =  300 - getWorld()->getLevel();
            else
                m_ticksToLive = 300;
            break;
            
        default: m_ticksToLive = -1; break;
            
    }
}

void ActivatingObject::move()
{

    
    if(getTicksToLive() > 0)
    {

        decreaseTicksToLive();
        return;
        
    }
    else
        setDead();
    
}

void ActivatingObject::setTicksToLive()
{
    if(300 - getWorld()->getLevel() < 100)
        m_ticksToLive = 300 - getWorld()->getLevel();
    else
        m_ticksToLive = 300;
}















////////////////////////////////////////////////////////////////////////////////
// FrackMan Implementation
////////////////////////////////////////////////////////////////////////////////

FrackMan::FrackMan(StudentWorld* world)
:Agent(world, 30, 60, right, IID_PLAYER, 1.0, 0, 10), m_score(0), m_numNuggets(0), m_numWater(100), m_numSonar(1), gotHit(false), m_ticksPassed(0)
{}

void FrackMan::addSonar()
{
    m_numSonar++; getWorld()->increaseScore(75);
}

void FrackMan::addGold()
{
    m_numNuggets++; getWorld()->increaseScore(10);
}

void FrackMan::addWater()
{
    m_numWater += 5; getWorld()->increaseScore(100);
}

void FrackMan::decreaseSonar()
{
    m_numSonar--;
    getWorld()->playSound(SOUND_SONAR);
}

int FrackMan::getSafeTicks()
{
    if(100 - getWorld()->getLevel() * 10 < 50)
        return 100 - getWorld()->getLevel() * 10;
    else return 50;
}

void FrackMan::checkEnemies()
{
    if(getWorld()->isNearProtester(getX(), getY(), 3))
    {
        gotHit = true;
        getHurt();
    }
}

void FrackMan::move()
{
    checkHealth();
    getGoodie();
    if(gotHit == false)
        checkEnemies();
    else if(gotHit == true)
    {
        m_ticksPassed++;
        if(m_ticksPassed >= getSafeTicks())
        {
            m_ticksPassed = 0;
            gotHit = false;
        }
    }
    
    int ch;
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
            case KEY_PRESS_ESCAPE: setDead(); break;
            case 'z':
            case 'Z': getWorld()->revealObjects(); decreaseSonar(); break;
                
        }
    }
    
   
    
    Actor* temp = getWorld()->getActor(getX(), getY());
    if(temp != nullptr && temp->getID() == IID_PROTESTER && getWorld()->isNearFrackMan(temp, 3))
    {
        getWorld()->playSound(SOUND_PLAYER_ANNOYED);
        annoy(3);
    }
    
    
}

void FrackMan::shootGun()
{
    if(m_numWater <= 0)
        return;
    
    
    int x = 0, y = 0;
    Direction dir;
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
    switch(getDirection())
    {
        case up: y = 4; dir = up; break;
        case down: y = -2; dir = down; break;
        case left: x = -2; dir = left; break;
        case right: x = 4; dir = right; break;
        case none: break;
    }
    Squirt* temp = new Squirt(getWorld(), getX() + x, getY() + y, dir);
    getWorld()->addActor(temp);
    
    if(getWorld()->isThereDirt(getX() + x, getY() + y)
       || getWorld()->isThereBoulder(getX() + x, getY() + y))
    {
        decreaseAmmo();
        temp->setDead();
        temp->setVisible(false);
        return;
    }

    
    
    decreaseAmmo();
    
}

void FrackMan::getGoodie()
{

        Actor* temp = getWorld()->getActor(getX(), getY());
    
        if(temp != nullptr && getWorld()->isNearFrackMan(temp, 5))
        {
            if(temp->canIBePickedUp() == false)
                return;
            int IID = temp->getID();
            if(IID == IID_WATER_SPURT)
                return;
            if(temp->getID() != IID_BARREL)
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
    int x = 0, y = 0;
    switch(dir)
    {
        case up:
            x = 0; y = 8; break;
        case down:
            x = 0; y = -5; break;
        case left:
            x = -5; y = 0; break;
        case right:
            x = 8; y = 0; break;
        case none: break;
            
        
    }
    GoldNugget* temp = new GoldNugget(getWorld(), getX()+x, getY()+y, true);
    getWorld()->addActor(temp);
    temp->setVisible(true);
    temp->setTicksToLive();
    temp->setDropped();
    temp->setDropped();
}


void FrackMan::go(Direction dir)
{
    
    switch(dir)
    {
        case left:
            if(getWorld()->isThereBoulder(getX()-1, getY()) || getWorld()->isThereBoulder(getX()-1, getY()+3))
                break;
            if(getDirection() != left)
            {
                setDirection(left);
                break;
            }
            if(getX() > 0)
                moveTo(getX()-1, getY());
            else
                moveTo(getX(), getY());
            break;
        case right:
            if(getWorld()->isThereBoulder(getX()+4, getY()) || getWorld()->isThereBoulder(getX()+4, getY()+3))
                break;
            if(getDirection() != right)
            {
                setDirection(right);
                break;
            }
            if(getX() < 60)
                moveTo(getX()+1, getY());
            else
                moveTo(getX(), getY());
            break;
        case down:
            if(getWorld()->isThereBoulder(getX(), getY()-1) || getWorld()->isThereBoulder(getX()+3, getY()-1))
                break;
            if(getDirection() != down)
            {
                setDirection(down);
                break;
            }
            if(getY() > 0)
                moveTo(getX(), getY()-1);
            else
                moveTo(getX(), getY());
            break;
        case up:
            if(getWorld()->isThereBoulder(getX(), getY()+4) || getWorld()->isThereBoulder(getX()+3, getY()+4))
                break;
            if(getDirection() != up)
            {
                setDirection(up);
                break;
            }
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
// FrackMan Implementation
////////////////////////////////////////////////////////////////////////////////















////////////////////////////////////////////////////////////////////////////////
// Squirt Implementation
////////////////////////////////////////////////////////////////////////////////
Squirt::Squirt(StudentWorld* world, int startX, int startY, Direction startDir)
:Actor(world, startX, startY, startDir, true, IID_WATER_SPURT, 1.0, 1), m_travelDistance(4), annoyedSomeone(false){}

void Squirt::move()
{

    if(getWorld()->isThereDirt(getX(), getY()) || getWorld()->isThereBoulder(getX(), getY()))
    {
        setDead();
        return;
    }
    Actor* temp = getWorld()->getActor(getX(), getY());
    if(temp->getID() == IID_PROTESTER)
    {
        if(annoyedSomeone == false)
        {
            getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
            temp->setAnnoyed();
            temp->annoy(2);
        }
        annoyedSomeone = true;
        
    }
    
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
// End of Squirt Implementation
////////////////////////////////////////////////////////////////////////////////


















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
            int px = getWorld()->getPlayer()->getX();
            int py = getWorld()->getPlayer()->getY();
            if(getX() == px && getY() == py+3)
               getWorld()->getPlayer()->setDead();
            
            
        }
        
        moved = true;
    }

    else
        setDead();
}

////////////////////////////////////////////////////////////////////////////////
// End of Boulder Implementation
////////////////////////////////////////////////////////////////////////////////















////////////////////////////////////////////////////////////////////////////////
// Oil Implementation
////////////////////////////////////////////////////////////////////////////////
OilBarrel::OilBarrel(StudentWorld* world, int startX, int startY)
:ActivatingObject(world, startX, startY, IID_BARREL, SOUND_FOUND_OIL, true, false, false, right, false){}
//{setVisible(true);}

void OilBarrel::move()
{
    if(! isAlive())
        return;
    if(isVisible() == false && getWorld()->isNearFrackMan(this, 5))
    {
       setVisible(true);
        return;
    }
    if(isVisible() == true && getWorld()->isNearFrackMan(this, 1))
    {
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->increaseScore(1000);
        getWorld()->gotBarrel();
        setDead();
    }
}

bool OilBarrel::needsToBePickedUpToFinishLevel()const {return true;}

////////////////////////////////////////////////////////////////////////////////
// Gold Nugget Implementation
////////////////////////////////////////////////////////////////////////////////
GoldNugget::GoldNugget(StudentWorld* world, int startX, int startY, bool visible)
:ActivatingObject(world, startX, startY, IID_GOLD, SOUND_GOT_GOODIE, true, false, false, left, false), dropped(false)
{

}

void GoldNugget::move()
{
    if(! isAlive())
        return;
    if(getTicksToLive() >= 0)
    {
        if(getTicksToLive() == 0)
        {
            setDead();
            return;
        }
        
        else decreaseTicksToLive();
    }
    if(isVisible() == false && getWorld()->isNearFrackMan(this, 4))
    {
        setVisible(true);
        return;
    }
    
    if(isVisible() == true && getWorld()->isNearFrackMan(this, 1))
    {
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(10);
        getWorld()->getPlayer()->addGold();
        setDead();
    }
    
    Actor* temp = getWorld()->getActor(getX(), getY());
    if(temp != nullptr && temp->getID() == IID_PROTESTER)
    {
        getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
        getWorld()->increaseScore(25);
        setDead();
    }
    
}


////////////////////////////////////////////////////////////////////////////////
// Water Pool Implementation
////////////////////////////////////////////////////////////////////////////////
WaterPool::WaterPool(StudentWorld* world, int startX, int startY)
:ActivatingObject(world, startX, startY, IID_WATER_POOL, SOUND_GOT_GOODIE, true, false, false, right, true){}


////////////////////////////////////////////////////////////////////////////////
// Sonar Kit Implementation
////////////////////////////////////////////////////////////////////////////////
SonarKit::SonarKit(StudentWorld* world, int startX, int startY)
:ActivatingObject(world, startX, startY, IID_SONAR, SOUND_GOT_GOODIE, true, false, false, left, true){}




























////////////////////////////////////////////////////////////////////////////////
// Protester Implementation
////////////////////////////////////////////////////////////////////////////////
Protester::Protester(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score)
:Agent(world, startX, startY, left, imageID, 1.0, 0, 5), m_numSquaresToMove(8), shouldILeave(false), m_ticksPassed(0), foundFrackMan(false),
     m_ticksSinceHorizontalMove(0), m_ticksWhileStunned(0), gotHit(false)
{
    setMap();
}

void Protester::addGold()
{
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    shouldILeave = true;
}


void Protester::setMap()
{
    for(int i = 0; i < 64; i++)
        for(int k = 0; k < 64; k++)
        {
            if(getWorld()->isThereDirtExact(i,k))
                thisMap[63-k][63-i] = 'X';
            else
                thisMap[63-k][63-i] = '.';
        }
    
    for(int i = 0; i < 64; i++)
        for(int k = 0; k < 64; k++)
        {
            if(getWorld()->isThereDirtExact(i,k))
                dirtMap[63-k][63-i] = true;
            else
                dirtMap[63-k][63-i] = false;
        }
}


void Protester::printMap()
{
    for(int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 64; j++)
            cout << thisMap[i][j];
        cout << endl;
    }
    
}

void Protester::checkBullets()
{
    int x = getX(), y = getY();
    Actor* temp = getWorld()->getActor(x,y);
    
    if(temp != nullptr && temp->getID() == IID_WATER_SPURT)
        getHurt();
    if(getWorld()->isNearFrackMan(this, 0))
        gotHit = true;
    
}

void Protester::checkFrackMan()
{
    if(getWorld()->isNearFrackMan(this, 0))
        gotHit = true;
}

int Protester::ticksToWait()
{
    if(3 - getWorld()->getLevel()/4 > 0)
        return 3 - getWorld()->getLevel()/4;
    else
        return 0;
}

int Protester::restingTicks()
{
    if(100 - getWorld()->getLevel() * 10 < 50)
        return 100 - getWorld()->getLevel() * 10;
    else
        return 50;
}

int Protester::findPath(int x, int y)
{
    
    queue<Coord> coordQueue;
    Coord a(x,y);

    thisMap[63-a.c()][a.r()] = '#';

    int count = 0;
    coordQueue.push(a);

    while(! coordQueue.empty())
    {
        Coord b(coordQueue.front());
        coordQueue.pop();
        
        if(count == 60)
            return count;
        
        if(thisMap[4][60] == '#')
        {
            setMap();
            return count;
        }
        else
        {
            if(b.r() > 64 || b.r() < 0 || b.c() > 64 || b.c() < 0)
                continue;
            
            if(thisMap[63-b.c()][b.r() + 1] != 'X' && thisMap[63-b.c()][b.r() + 1] != '#')      //North
            {
                Coord c(b.r(), b.c()+1);
                coordQueue.push(c);
                thisMap[63-c.c()][c.r()] = '#';

            }
            
            if(thisMap[63-b.c() + 1][b.r()] != 'X' && thisMap[63-b.c() + 1][b.r()] != '#')    //East
            {
                Coord c(b.r()+1, b.c());
                coordQueue.push(c);
                thisMap[63-c.c()][c.r()] = '#';
            }
            
            if(thisMap[63-b.c()][b.r() - 1] != 'X' && thisMap[63-b.c()][b.r() - 1] != '#')    //South
            {
                Coord c(b.r(), b.c()-1);
                coordQueue.push(c);
                thisMap[63-c.c()][c.r()] = '#';

            }
            
            if(thisMap[63-b.c() - 1][b.r()] != 'X' && thisMap[63-b.c() - 1][b.r()] != '#')   //West
            {
                Coord c(b.r()-1, b.c());
                coordQueue.push(c);
                thisMap[63-c.c()][c.r()] = '#';
            }
            
            
        }

        count++;
    }

    setMap();
    return count;

}

void Protester::findExit()
{
    
    int count1 = findPath(getX()-1, getY());
    int count2 = findPath(getX()+1, getY());
    int count3 = findPath(getX(), getY()+1);
    int count4 = findPath(getX(), getY()-1);

    
    ////Move in the direction that took the least amount of steps
    if(count1 > count2 && count1 > count3 && count1 > count4)
    {
        moveTo(getX()-1, getY()); setDirection(left); return;
    }
    if(count2 > count1 && count2 > count3 && count2 > count4)
    {
        moveTo(getX()+1, getY()); setDirection(right); return;
    }
    if(count3 > count1 && count3 > count2 && count3 > count4)
    {
        moveTo(getX(), getY()+1); setDirection(up); return;
    }
    if(count4 > count1 && count4 > count2 && count4 > count3)
    {
        moveTo(getX(), getY()-1); setDirection(down); return;
    }
    
    if(count1 == count2)
    {
        moveTo(getX(), getY()+1); setDirection(up); return;
    }
        
    
}

void Protester::findExit2()
{
    int x = getX(), y = getY();
    if(y < 60 && dirtMap[x][y-1] == false)
    {
        setDirection(up); moveTo(x, y+1); return;
    }
    
    if(x < 64 && dirtMap[x+1][y] == false)
    {
        setDirection(right); moveTo(x+1, y); return;
    }
    
    if(x > 0 && dirtMap[x-1][y] == false)
    {
        setDirection(left); moveTo(x-1, y); return;
    }
    
    if(y > 0 && dirtMap[x][y+1] == false)
    {
        setDirection(down); moveTo(x, y-1); return;
    }
}

void Protester::huntFrackMan()
{
    if(! getWorld()->facingTowardFrackMan(this))
    {  setDirection(right); moveTo(getX()+1, getY()); return;}
    
    int px = getWorld()->getPlayer()->getX();
    int py = getWorld()->getPlayer()->getY();
    
    int x = getX();
    int y = getY();
    
    if(px < x)
    {   moveTo(x-1, y); setDirection(left); return;}
    if(px > x)
    {   moveTo(x+1, y); setDirection(right); return;}
    if(py < y)
    {   moveTo(x, y-1); setDirection(down); return;}
    if(py > y)
    {   moveTo(x, y+1); setDirection(up); return;}

}



void Protester::changeDirectionsIfPossible()
{
    if(m_numSquaresToMove > 0)
        return;

    
    
    bool repeat = true;
    
    int currX = getX();
    int currY = getY();

    while(repeat == true)
    {
        repeat = false;
        
        
        int choose = rand() % 4 + 1;
        
        
        switch(choose)
        {
            case 1:
                if(! isValidMove(up))
                {
                    repeat = true;
                    break;
                }

                if(currY <= 60 && currY >= 0)
                    setDirection(up);
                break;
            case 2:
                if(! isValidMove(down))
                {
                    repeat = true;
                    break;
                }
                if(currY <= 60 && currY >= 0)
                    setDirection(down);
                break;
            case 3:
                if(! isValidMove(left))
                {
                    repeat = true;
                    break;
                }
                if(currX <= 64 && currX >= 0)
                    setDirection(left);
                break;
            case 4:
                if(! isValidMove(right))
                {
                    repeat = true;
                    break;
                }
                if(currX <= 64 && currX >= 0)
                    setDirection(right);
            default: break;
            
        }
    }
    
    int numSteps = rand() % 52 + 8;
    m_numSquaresToMove = numSteps;
    
}


bool Protester::isValidMove(GraphObject::Direction dir)
{
    switch(dir)
    {
        case left:
            if(getWorld()->isThereBoulder(getX()-1, getY()) || getWorld()->isThereBoulder(getX()-1, getY()+3))
                return false;
            if(getWorld()->isThereDirt(getX()-1, getY()) || getWorld()->isThereBoulder(getX()-1, getY()+3))
                return false;
            else
                return true;
        case right:
            if(getWorld()->isThereBoulder(getX()+3, getY()) || getWorld()->isThereBoulder(getX()+3, getY()+3))
                return false;
            if(getWorld()->isThereDirt(getX()+3, getY()) || getWorld()->isThereBoulder(getX()+3, getY()+3))
                return false;
            else
                return true;
        case up:
            if(getWorld()->isThereBoulder(getX(), getY()+4) || getWorld()->isThereBoulder(getX()+3, getY()+4))
                return false;
            if(getWorld()->isThereDirt(getX(), getY()+4) || getWorld()->isThereDirt(getX()+3, getY()+4))
                return false;
            else
                return true;
        case down:
            if(getWorld()->isThereBoulder(getX(), getY()-1) || getWorld()->isThereBoulder(getX()+3, getY()-1))
                return false;
            if(getWorld()->isThereDirt(getX(), getY()-1) || getWorld()->isThereDirt(getX()+3, getY()-1))
                return false;
            return true;
            
        case none: return true;
    }
}

void Protester::go(GraphObject::Direction dir)
{

    switch(dir)
    {
        case up:
            if(! isValidMove(up))
            {
                m_numSquaresToMove = 0;
                break;
            }
            else
            {
                moveTo(getX(), getY()+1);
                setDirection(up);
                break;
            }
            
        case right:
            if(! isValidMove(right))
            {
                m_numSquaresToMove = 0;
                break;
            }
            else
            {
                moveTo(getX()+1, getY());
                setDirection(right);
                break;
            }
        case left:
            if(! isValidMove(left))
            {
                m_numSquaresToMove = 0;
                break;
            }
            else
            {
                moveTo(getX()-1, getY());
                setDirection(left);
                break;
            }
        case down:
            if(! isValidMove(down))
            {
                m_numSquaresToMove = 0;
                break;
            }
            else
            {
                moveTo(getX(), getY()-1);
                setDirection(down);
                break;
            }
            
        case none: break;
        default: break;
            
    }
    m_numSquaresToMove--;
}


bool Protester::madeHorizontalMove(GraphObject::Direction startDir, GraphObject::Direction newDir)
{
    if(startDir == up && newDir == right)
        return true;
    if(startDir == up && newDir == left)
        return true;
    if(startDir == down && newDir == right)
        return true;
    if(startDir == down && newDir == left)
        return true;
    if(startDir == right && newDir == up)
        return true;
    if(startDir == right && newDir == down)
        return true;
    if(startDir == left && newDir == up)
        return true;
    if(startDir == left && newDir == down)
        return true;
    else
        return false;
}

bool Protester::amIAtAnIntersection(int currX, int currY)
{
    switch(getDirection())
    {
        case up:
            if(isValidMove(right))
                return true;
            if(isValidMove(left))
                return true;
            break;
        case down:
            if(isValidMove(right))
                return true;
            if(isValidMove(left))
                return true;
            break;
        case left:
            if(isValidMove(up))
                return true;
            if(isValidMove(down))
                return true;
            break;
        case right:
            if(isValidMove(up))
                return true;
            if(isValidMove(down))
                return true;
            break;
        case none:
            return false; break;
            
        default: break;
    }
    
    return false;
}



void Protester::makePerpindicularMove()
{

    switch(getDirection())
    {
        case up:
            if(isValidMove(left))
            {
                setDirection(left);
                go(left);
                break;
            }
            else if(isValidMove(right))
            {
                setDirection(left);
                go(right);
            }
        
                break;
        case down:
            if(isValidMove(right))
            {
                
                setDirection(right);
                go(right);
                break;
            }
            
            else if(isValidMove(left))
            {
                
                setDirection(left);
                go(left);
            }
            break;

        case left:
            if(isValidMove(up))
            {
                
                setDirection(up);
                go(up);
                break;
            }
            
            else if(isValidMove(down))
            {
                
                setDirection(down);
                go(down);
            }
            break;
        case right:
            if(isValidMove(down))
            {
                setDirection(down);
                go(down);
                break;
            }
            if(isValidMove(up))
            {
                setDirection(up);
                go(up);
            }
            break;
            
            
        case none: break;
        default: break;
    }
    int numSteps = rand() % 52 + 8;
    m_numSquaresToMove = numSteps;
}

void Protester::moveRandomly()
{

    Direction startDir = getDirection();
    if(m_ticksSinceHorizontalMove > 100 && amIAtAnIntersection(getX(), getY()))
    {
        makePerpindicularMove();
        m_ticksSinceHorizontalMove = 0;
        return;
    }
    
    
    
    if(madeHorizontalMove(startDir, getDirection()))
        m_ticksSinceHorizontalMove = 0;
    
    if(m_numSquaresToMove <= 0)
        changeDirectionsIfPossible();
    if(m_numSquaresToMove > 0)
    {
        switch(getDirection())
        {
            case up: go(up); break;
            case right: go(right); break;
            case left: go(left); break;
            case down: go(down); break;
            case none: break;
            default: break;
            
        }
        m_numSquaresToMove--;
    }
    
    
    m_ticksPassed = 0;
}







void Protester::move()
{
    checkBullets();
    
    if(! isAlive())
        return;
    
    if(getX() == 60 && getY() == 60 && shouldILeave == true)
    { setDead(); return; }
    
    if(getHitPoints() <= 0)
    {setDead();  getWorld()->playSound(SOUND_PROTESTER_GIVE_UP); getWorld()->increaseScore(100); return;}
    
    checkBullets();
    checkFrackMan();
//    if(getWorld()->facingTowardFrackMan(this))
//        foundFrackMan = true;

    if(gotHit == true && m_ticksWhileStunned < restingTicks())
    {
        m_ticksWhileStunned ++;
        return;
    }
    gotHit = false;
    m_ticksWhileStunned = 0;
    
    m_ticksSinceHorizontalMove++;
//    cout << m_ticksSinceHorizontalMove << endl;
    if(m_ticksPassed < ticksToWait())
    {
        m_ticksPassed++;
        return;
    }

    
    
    if(shouldILeave == false && foundFrackMan == false)
    {
        moveRandomly();
        return;
        
    }
    
    
    
    if(shouldILeave == false && foundFrackMan == true)
    {
        
        moveRandomly();
//        huntFrackMan();
        
    }
    
    
    
    
    if(shouldILeave == true)
    {
        if(getX() == 60 && getY() == 60)
        {
            setDead(); return;
        }
        moveRandomly();
        findExit2();
//        moveTo(getX()+1, getY());
//        setDirection(right);
        m_ticksPassed = 0;
    }
    
    
    
}

////////////////////////////////////////////////////////////////////////////////
// End of Protester Implementation
////////////////////////////////////////////////////////////////////////////////









////////////////////////////////////////////////////////////////////////////////
// Regular Protester Implementation
////////////////////////////////////////////////////////////////////////////////
RegularProtester::RegularProtester(StudentWorld* world, int startX, int startY)
:Protester(world, startX, startY, IID_PROTESTER, 5, 0){}




////////////////////////////////////////////////////////////////////////////////
// Hardcore Protester Implementation
////////////////////////////////////////////////////////////////////////////////
HardcoreProtester::HardcoreProtester(StudentWorld* world, int startX, int startY)
:Protester(world, startX, startY, IID_HARD_CORE_PROTESTER, 5, 0){}

















