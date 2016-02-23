#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}
bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
    if(isThereDirt(x,y))
        return false;
    return true;
}

void StudentWorld::addDirt()
{
    for(int x = 0; x < 64; x++)
        for(int y = 0; y < 4; y++)
            m_dirt[x][y] = new Dirt(this, x,y);
    
    for(int x = 0; x < 64; x++)
        for(int y = 4; y < 60; y++)
            if(x < 30 || x > 33)
                m_dirt[x][y] = new Dirt(this, x,y);
}


bool StudentWorld::isThereDirt(int x, int y) const 
{
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < 4; k++)
            if(m_dirt[x+i][y+k] != nullptr)
                return true;
    return false;
}

void StudentWorld::clearDirt(int x, int y)
{
    if(isThereDirt(x,y))                                //If there's dirt at the player's location, delete the dirt
    {
        playSound(SOUND_DIG);
        for(int i = 0;  i < 4; i++)
            for(int k = 0; k < 4; k++)
            {
                if(m_dirt[x+i][y+k] != nullptr)
                {
                    m_dirt[x+i][y+k]->setVisible(false);     //FIND OUT HOW TO USE POLYMORPHISM deleteActor HERE
                    delete m_dirt[x+i][y+k];
                    m_dirt[x+i][y+k] = nullptr;
                }
            }
        
        
    }
}

int StudentWorld::numObjects(int IID)
{
    switch(IID)
    {
        case IID_BOULDER:
            if(getLevel()/2 + 2 < 6)
                return getLevel()/2 +2;
            else return 6;
            break;
        case IID_BARREL:
            if(getLevel()+2 < 20)
                return getLevel()+2;
            else return 20;
            break;
        case IID_GOLD:
            if(5 - getLevel()/2 < 2)
                return 5 - getLevel()/2;
            else return 2;
            break;
        default:
            return 0; break;
            
    }
}

////////Add Boulders to the field
void StudentWorld::addBoulders()
{
    for(int k = 0; k < numObjects(IID_BOULDER); k++)
    {
        int x = rand() % 56; int y = rand() % 56;
        while(y < 20)
            y = rand() % 56;
        while(x < 4 || (x > 22 && x < 33))
            x = rand() % 56;
        
        
        if(isThereDirt(x,y) || isThereDirt(x+4, y+4))
        {
            m_actors.push_back(new Boulder(this, x,y));
            for(int i = 0;  i < 4; i++)
                for(int k = 0; k < 4; k++)
                {
                    if(m_dirt[x+i][y+k] != nullptr)
                    {
                        m_dirt[x+i][y+k]->setVisible(false);     //FIND OUT HOW TO USE POLYMORPHISM deleteActor HERE
                        delete m_dirt[x+i][y+k];
                        m_dirt[x+i][y+k] = nullptr;
                    }
                }
        }
        else
            k--;
    }
}

void StudentWorld::clearDead()
{
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end();)
        if(*it != nullptr)
        {
            if((*it)->isAlive() == false)
            {
                
                {
                    delete *it;
                    *it = nullptr;
                    it = m_actors.erase(it);
                }
                
                
            }
            else it++;
        }
    
    
    
    
}

bool StudentWorld::isThereActor(int x, int y) 
{
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        int x1 = (**it).getX();
        int y1 = (**it).getY();
        
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < 4; k++)
                if(x == x1 + i && y == y1 + k)
                    return true;
        
    }
    return false;
    
}

bool StudentWorld::isNearFrackMan(Actor *a, int radius)
{
    for(int i = 0; i < radius; i++)
        for(int k = 0; k < radius; k++)
        {
            if(a->getX() + i == m_player->getX() + i && a->getY() + k == m_player->getY() + k)
                return true;
        }
    return false;
    
        
       
}
////////Checks the Euclidian Radius of
bool StudentWorld::isInvalidRadius(int x, int y) 
{
    for(int i = 0; i < 8; i++)
        for(int k = 0; k < 8; k++)
            if(isThereActor(x+i, y+k) || isThereActor(x+4-i, y+4-k))
                return true;
    return false;
}

void StudentWorld::addActor(Actor *a, int imageID)
{
    
    for(int k = 0; k < numObjects(imageID); k++)
    {
        int x = rand() % 56; int y = rand() % 56;
        while(isInvalidRadius(x,y))
        {
            x = rand() % 56; y = rand() % 56;
        }
    
        if(isThereDirt(x,y) || isThereDirt(x+4, y+4))
        {
            
            m_actors.push_back(a);
        }
    }
    m_actors.push_back(a);
}

void StudentWorld::addOil()
{
    for(int k = 0; k < numObjects(IID_BARREL); k++)
    {
        int x = rand() % 56; int y = rand() % 56;
        
        while(isInvalidRadius(x,y))
        {
            x = rand() % 56, y = rand() % 56;
        }
        
        if(isThereDirt(x,y) || isThereDirt(x+4, y+4))
        {
            
            m_actors.push_back(new OilBarrel(this, x,y));
        }
        else --k;
    }
}

void StudentWorld::addGold()
{
    for(int k = 0; k < numObjects(IID_GOLD); k++)
    {
        int x = rand() % 56; int y = rand() % 56;
        
        while(isInvalidRadius(x,y))
        {
            x = rand() % 56, y = rand() % 56;
        }
        
        if(isThereDirt(x,y) || isThereDirt(x+4, y+4))
        {
            
            m_actors.push_back(new GoldNugget(this, x,y, true));
        }
        else --k;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Main Functions
////////////////////////////////////////////////////////////////////////////////

int StudentWorld::init()

{
    m_numBarrels = numObjects(IID_BARREL);
    addDirt();
    addBoulders();
    addOil();
    addGold();
    m_player = new FrackMan(this, 30, 60);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText("");
    
    if(m_player->isAlive())
    {
        
        m_player->move();
        for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        {
            (*it)->move();
            
            ///////////FIGURE OUT HOW TO DELETE ACTORS HERE
            
        }
        
        clearDirt(m_player->getX(), m_player->getY());
        clearDead();
        
    }
    
    if(m_numBarrels > 0)
        return GWSTATUS_CONTINUE_GAME;
    if(m_numBarrels == 0)
        return GWSTATUS_FINISHED_LEVEL;
    
    
    return GWSTATUS_PLAYER_DIED;
}


void StudentWorld::cleanUp()
{
    for(int x = 0; x < 64; x++)
        for(int y = 0; y < 60; y++)
        {
            if(m_dirt[x][y] != nullptr)
                delete m_dirt[x][y];
        }
    
    delete m_player;
    int i = 0;
    for(vector<Actor*>::iterator it = m_actors.begin(); i < m_actors.size(); it++, i++)
        delete *it;
    m_actors.clear();
    
    
}














