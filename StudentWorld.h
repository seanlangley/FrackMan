#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isThereDirt(int x, int y);
    bool isThereActor(int x, int y);
    bool isInvalidRadius(int x, int y);
    bool isThereBoulder(int x, int y);
    void deleteActor(Actor* deleteMe);
    void deleteDirt();
    void addDirt();
    void addBoulders();
    void addOil();
    void clearDead();
    void gotBarrel(){m_numBarrels--;}
    int numObjects(int IID);
    FrackMan* getPlayer(){return m_player;}
    
private:

    vector<Actor*> m_actors;
    Dirt* m_dirt[64][64];
    FrackMan* m_player;
    int m_numBarrels;
    
    
};

#endif // STUDENTWORLD_H_




