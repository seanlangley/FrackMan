#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir){}
    ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isThereDirt(int x, int y);
    void deleteActor(Actor* deleteMe);

    
private:

    list<Actor*> m_actors;
    Dirt* m_dirt[64][64];
    FrackMan* m_player;
    
    
};

#endif // STUDENTWORLD_H_




