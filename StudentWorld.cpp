#include "StudentWorld.h"
#include <string>
#include <vector>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

////////////////////////////////////////////////////////////////////////////////
// Auxiliary Functions
////////////////////////////////////////////////////////////////////////////////



StudentWorld::~StudentWorld()
{
    for(int x = 0; x < 64; x++)
        for(int y = 0; y < 60; y++)
        {
            if(m_dirt[x][y] != nullptr)
                delete m_dirt[x][y];
        }
    
    delete m_player;
    
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        delete *it;
    m_actors.clear();
    
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

//DeAllocate an actor's memory, set it's pointer to NULL
void StudentWorld::deleteActor(Actor* deleteMe)
{
    if(deleteMe != nullptr)
    {
        deleteMe->setVisible(false);
        delete  deleteMe;
        deleteMe = nullptr;
    }
}


int StudentWorld::numBoulders()
{
    if(getLevel()/2 + 2 < 6)
        return getLevel()/2 +2;
    else
        return 6;
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
    for(int k = 0; k < numBoulders(); k++)
    {
        int x = rand() % 56; int y = rand() % 56;
        while(y < 20)
            y = rand() % 56;
        while(x < 4 || (x > 22 && x < 33))
            x = rand() % 56;
        
        
        if(isThereDirt(x,y) && isThereDirt(x+4, y+4))
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

void StudentWorld::deleteDirt(Dirt* deleteMe)
{
    if(deleteMe == nullptr)
        return;
    int x = deleteMe->getX();
    int y = deleteMe->getY();
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

////////////////////////////////////////////////////////////////////////////////
// Main Functions
////////////////////////////////////////////////////////////////////////////////

int StudentWorld::init()

{
    addDirt();
    addBoulders();
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
            (*it)->doSomething();
        
        
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

        
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    
    
    
    
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
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end() && i < m_actors.size(); it++, i++)
        delete *it;
    m_actors.clear();
    
    
}











