#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject
{
public:
    Actor(int IID, int x, int y, Direction dir, float size, unsigned  int depth)
    :GraphObject(IID, x,y, dir, size, depth){}
    virtual ~Actor()
    {
        return;
    }
private:
    int m_x, m_y;
    
};
#endif // ACTOR_H_



#ifndef DIRT_H
#define DIRT_H

class Dirt : public Actor
{
public:
    Dirt(int x, int y);
    virtual ~Dirt();
    
private:
    
};

#endif // DIRT_H_



#ifndef FRACKMAN_H
#define FRACKMAN_H

class FrackMan : public Actor
{
public:
    FrackMan();
    
    virtual ~FrackMan();
private:
    
};
#endif // FRACKMAN_H_
