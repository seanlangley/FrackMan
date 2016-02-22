#include "StudentWorld.h"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

////////////////////////////////////////////////////////////////////////////////
// Auxiliary Functions
////////////////////////////////////////////////////////////////////////////////


StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), m_numBarrels(numObjects(IID_BARREL))
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

//Checks if there's dirt within Frackman's coordinate
bool StudentWorld::isThereDirt(int x, int y)
{
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < 4; k++)
            if(m_dirt[x+i][y+k] != nullptr)
                return true;
    return false;
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

bool StudentWorld::isThereBoulder(int x, int y)
{
    
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->getID() != IID_BOULDER)
            continue;
        int x1 = (**it).getX();
        int y1 = (**it).getY();
        
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < 4; k++)
                if(x == x1 + i  && y == y1 + k)
                    return true;
    }
    return false;
        
}

//DeAllocate an actor's memory, set it's pointer to NULL
void StudentWorld::deleteActor(Actor* deleteMe)
{
    m_actors.erase(remove(m_actors.begin(), m_actors.end(), deleteMe), m_actors.end());
    
    
    if(deleteMe != nullptr)
    {
        deleteMe->setVisible(false);
        delete  deleteMe;
        deleteMe = nullptr;
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
////////Checks the Euclidian Radius of
bool StudentWorld::isInvalidRadius(int x, int y)
{
    for(int i = 0; i < 8; i++)
        for(int k = 0; k < 8; k++)
            if(isThereActor(x+i, y+k) || isThereActor(x+4-i, y+4-k))
                return true;
    return false;
}

////////Add Dirt to the field
void StudentWorld::addDirt()
{
    for(int x = 0; x < 64; x++)
        for(int y = 0; y < 4; y++)
            m_dirt[x][y] = new Dirt(x,y, this);
    
    for(int x = 0; x < 64; x++)
        for(int y = 4; y < 60; y++)
            if(x < 30 || x > 33)
                m_dirt[x][y] = new Dirt(x,y, this);
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
            m_actors.push_back(new Boulder(x,y, this));
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
            
            m_actors.push_back(new BarrelOfOil(x,y, this));
        }
        else --k;
    }
}

void StudentWorld::deleteDirt()
{
    int x = m_player->getX(), y = m_player->getY();
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

void StudentWorld::clearDead()
{
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        if(*it != nullptr)
            if((*it)->isDead() == true)
            {
            
                {
                    delete *it;
                    *it = nullptr;
                    it = m_actors.erase(it);
                }
            
            
            }
        
    
        
    
}

////////////////////////////////////////////////////////////////////////////////
// Main Functions
////////////////////////////////////////////////////////////////////////////////

int StudentWorld::init()

{
    addDirt();
    addBoulders();
    addOil();
    m_player = new FrackMan(this);
    return GWSTATUS_CONTINUE_GAME;
}





int StudentWorld::move()
{
    
    setGameStatText("");
    
    if(m_player->getHealth() > 0)
    {
        
        m_player->doSomething();
        for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        {
            (*it)->doSomething();
            
              ///////////FIGURE OUT HOW TO DELETE ACTORS HERE

        }
        
        clearDead();
        deleteDirt();
        return GWSTATUS_CONTINUE_GAME;
    }
    
    
    if(m_numBarrels == 0)
        return GWSTATUS_FINISHED_LEVEL;
    
    decLives();
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











