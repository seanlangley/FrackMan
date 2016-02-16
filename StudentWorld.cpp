#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


////////////////////////////////////////////////////////////////////////////////
// Student World Implementation
////////////////////////////////////////////////////////////////////////////////



StudentWorld::~StudentWorld()
{
    for(int x = 0; x < 60; x++)
        for(int y = 0; y < 60; y++)
        {
            if(m_dirt[x][y] != nullptr)
                delete m_dirt[x][y];
        }
//        delete [] m_dirt;

    delete m_player;
    
}

bool StudentWorld::isThereDirt(int x, int y)
{
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < 4; k++)
            if(m_dirt[x+i][y+k] != nullptr)
                return true;                //Checks if there's dirt within Frackman's coordinate
    
    return false;
}

void StudentWorld::deleteActor(Actor* deleteMe)
{
    if(deleteMe != nullptr)
    {
        deleteMe->setVisible(false);
        delete  deleteMe;               //DeAllocate an actor's memory, set it's pointer to NULL
        deleteMe = nullptr;
    }
}




////////////////////////////////////////////////////////////////////////////////
// Main Functions
////////////////////////////////////////////////////////////////////////////////

int StudentWorld::init()

{
    for(int x = 0; x < 64; x++)
        for(int y = 0; y < 4; y++)
            m_dirt[x][y] = new Dirt(x,y, this);
    
    for(int x = 0; x < 64; x++)
        for(int y = 4; y < 60; y++)
            if(x < 30 || x > 33)
                m_dirt[x][y] = new Dirt(x,y, this);
    
    
    m_player = new FrackMan(this);
    
    return GWSTATUS_CONTINUE_GAME;
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
}




int StudentWorld::move()
{

    
    setGameStatText("Hello World!");
    
    if(m_player->getHealth() > 0)
    {
        m_player->doSomething();
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









