#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;
////////////////////////////////////////////////////////////////////////////////
// Actor Definition
////////////////////////////////////////////////////////////////////////////////
class Actor : public GraphObject
{
public:
    Actor(int IID, int x, int y, Direction dir, bool isVisible, float size, unsigned  int depth, StudentWorld* sw);
    virtual ~Actor();
    virtual void doSomething() = 0;
    virtual void getAnnoyed() = 0;
    virtual StudentWorld* getWorld(){return m_world;}
private:
    StudentWorld* m_world;
   
    
};
#endif // ACTOR_H_

////////////////////////////////////////////////////////////////////////////////
// Dirt Definition
////////////////////////////////////////////////////////////////////////////////

#ifndef DIRT_H
#define DIRT_H

class Dirt : public Actor
{
public:
    Dirt(int x, int y, StudentWorld* sw);
    virtual ~Dirt();
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
    
};

#endif // DIRT_H_

////////////////////////////////////////////////////////////////////////////////
// FrackMan Definition
////////////////////////////////////////////////////////////////////////////////

#ifndef FRACKMAN_H
#define FRACKMAN_H

class FrackMan : public Actor
{
public:
    FrackMan(StudentWorld* sw);
    
    virtual ~FrackMan();
    virtual void doSomething();
    virtual void getAnnoyed();
    int getHealth(){return m_health;}
private:
    int m_health;
    

    
};
#endif // FRACKMAN_H_

////////////////////////////////////////////////////////////////////////////////
// Boulders Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef BOULDER_H
#define BOULDER_H

class Boulder : public Actor
{
public:
    Boulder(int x, int y, StudentWorld* sw);
    virtual ~Boulder(){return;}
    virtual void doSomething();
    virtual void getAnnoyed(){return;}
    bool isAlive(){return m_isAlive;}
private:
    bool m_isAlive;

};

#endif // BOULDER_H_
////////////////////////////////////////////////////////////////////////////////
// Squirt Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef SQUIRT_H
#define SQUIRT_H

class Squirt : public Actor
{
public:
    Squirt(int x, int y, Direction dir, StudentWorld* sw);
    virtual ~Squirt(){return;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
};

#endif // SQUIRT_H_

////////////////////////////////////////////////////////////////////////////////
// Barrel of Oil Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef BARREL_OF_OIL_H
#define BARREL_OF_OIL_H

class BarrelOfOil : public Actor
{
public:
    BarrelOfOil(int x, int y, StudentWorld* sw);
    virtual ~BarrelOfOil(){return;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
};

#endif // BARREL_OF_OIL_H
////////////////////////////////////////////////////////////////////////////////
// Gold Nugget Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef GOLD_NUGGET_H
#define GOLD_NUGGET_H

class GoldNugget : public Actor
{
public:
    GoldNugget(int x, int y, bool isVisible, StudentWorld* sw);
    virtual ~GoldNugget(){return;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
private:
    //Timestate Variable goes here
};

#endif // GOLD_NUGGET_H
////////////////////////////////////////////////////////////////////////////////
// Sonar Kit Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef SONAR_KIT_H
#define SONAR_KIT_H

class SonarKit : public Actor
{
public:
    SonarKit(int x, int y, StudentWorld* sw);
    virtual ~SonarKit(){return;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
};

#endif // SONAR_KIT_H
////////////////////////////////////////////////////////////////////////////////
// Water Pool Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef WATER_POOL_H
#define WATER_POOL_H

class WaterPool : public Actor
{
public:
    WaterPool(int x, int y, StudentWorld* sw);
    virtual ~WaterPool(){return;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
};

#endif // WATER_POOL_H
////////////////////////////////////////////////////////////////////////////////
// Regular Protestor Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef REGULAR_PROTESTOR_H
#define REGULAR_PROTESTOR_H

class RegularProtestor : public Actor
{
public:
    RegularProtestor(int x, int y, StudentWorld* sw);
    virtual ~RegularProtestor(){return;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
};

#endif // REGULAR_PROTESTOR_H
////////////////////////////////////////////////////////////////////////////////
// Hardcore Protestor Definition
////////////////////////////////////////////////////////////////////////////////
#ifndef HARDCORE_PROTESTOR_H
#define HARDCORE_PROTESTOR_H

class HardcoreProtestor : public RegularProtestor
{
public:
    HardcoreProtestor(int x, int y, StudentWorld* sw);
    virtual ~HardcoreProtestor(){return;}
    virtual void doSomething(){return;}
    virtual void getAnnoyed(){return;}
};

#endif // HARDCORE_PROTESTOR_H










