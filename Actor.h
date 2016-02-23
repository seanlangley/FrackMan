// Possible interfaces for actors.  You may use all, some, or none
// of this, as you wish.

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;


////////////////////////////////////////////////////////////////////////////////
// Actor Interface
////////////////////////////////////////////////////////////////////////////////
class Actor : public GraphObject
{
public:
    Actor(StudentWorld* world, int startX, int startY, Direction startDir,
          bool visible, int imageID, double size, int depth);
    
    // Action to perform each tick.
    virtual void move() = 0;
    
    // Is this actor alive?
    bool isAlive() const {if(m_isAlive) return true; else return false;}
    
    // Mark this actor as dead.
    void setDead() {m_isAlive = false;}
    
    // Annoy this actor.
    virtual bool annoy(unsigned int amt){return 0;}
    
    // Get this actor's world
    StudentWorld* getWorld() const{return m_world;}
    
    // Can other actors pass through this actor?
    virtual bool canActorsPassThroughMe() const{return true;}
    
    // Can this actor dig through dirt?
    virtual bool canDigThroughDirt() const{return true;}
    
    // Can this actor pick items up?
    virtual bool canPickThingsUp() const{return true;}
    
    // Does this actor hunt the FrackMan?
    virtual bool huntsFrackMan() const{return true;}
    
    // Can this actor need to be picked up to finish the level?
    virtual bool needsToBePickedUpToFinishLevel() const{return false;}
    
    // Move this actor to x,y if possible, and return true; otherwise,
    // return false without moving.
    bool moveToIfPossible(int x, int y){return true;}
private:
    bool m_isAlive;
    StudentWorld* m_world;
};


////////////////////////////////////////////////////////////////////////////////
// Agent Interface
////////////////////////////////////////////////////////////////////////////////
class Agent : public Actor
{
public:
    Agent(StudentWorld* world, int startX, int startY, Direction startDir,
          int imageID, double size, int depth, unsigned int hitPoints)
    :Actor(world, startX, startY, startDir, true, imageID, size, depth), m_hitPoints(hitPoints){}
    
    // Pick up a gold nugget.
    virtual void addGold() = 0;
    
    // How many hit points does this actor have left?
    unsigned int getHitPoints() const{return 0;}
    
    virtual bool annoy(unsigned int amount){return true;}
    virtual bool canPickThingsUp() const{return true;}
private:
    int m_hitPoints;
};


////////////////////////////////////////////////////////////////////////////////
// FrackMan Interface
////////////////////////////////////////////////////////////////////////////////
class FrackMan : public Agent
{
public:
    FrackMan(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool annoy(unsigned int amount){return true;}
    virtual void addGold(){m_numNuggets++;}
    virtual bool canDigThroughDirt() const{return true;}
    
    // Pick up a sonar kit.
    void addSonar(){m_numSonar++;}
    
    // Pick up water.
    void addWater(){m_numWater++;}
    
    // Get amount of gold
    unsigned int getGold() const{return m_numNuggets;}
    
    // Get amount of sonar charges
    unsigned int getSonar() const{return m_numSonar;}
    
    // Get amount of water
    unsigned int getWater() const{return m_numWater;}
    void increaseScore(int score){m_score += score;}
    
    void go(Direction dir);
private:
    int m_score;
    int m_numNuggets;
    int m_numSonar;
    int m_numWater;
};


////////////////////////////////////////////////////////////////////////////////
// Protestor Interface
////////////////////////////////////////////////////////////////////////////////
class Protester : public Agent
{
public:
    Protester(StudentWorld* world, int startX, int startY, int imageID,
              unsigned int hitPoints, unsigned int score);
    virtual void move();
    virtual bool annoy(unsigned int amount);
    virtual void addGold();
    virtual bool huntsFrackMan() const;
    
    // Set state to having gien up protest
    void setMustLeaveOilField();
    
    // Set number of ticks until next move
    void setTicksToNextMove();
};


////////////////////////////////////////////////////////////////////////////////
// Regular Protestor Interface
////////////////////////////////////////////////////////////////////////////////
class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};


////////////////////////////////////////////////////////////////////////////////
// Hardcore Protestor Interface
////////////////////////////////////////////////////////////////////////////////
class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY, int imageID);
    virtual void move();
    virtual void addGold();
};


////////////////////////////////////////////////////////////////////////////////
// Dirt Interface
////////////////////////////////////////////////////////////////////////////////
class Dirt : public Actor
{
public:
    Dirt(StudentWorld* world, int startX, int startY);
    virtual void move(){return;}
};


////////////////////////////////////////////////////////////////////////////////
// Boulder Interface
////////////////////////////////////////////////////////////////////////////////
class Boulder : public Actor
{
public:
    Boulder(StudentWorld* world, int startX, int startY) ;
    virtual void move();
    virtual bool canActorsPassThroughMe() const{return false;}
private:
    int m_count;
    bool moved;
};


////////////////////////////////////////////////////////////////////////////////
// Squirt Interface
////////////////////////////////////////////////////////////////////////////////
class Squirt : public Actor
{
public:
    Squirt(StudentWorld* world, int startX, int startY, Direction startDir);
    virtual void move();
};


////////////////////////////////////////////////////////////////////////////////
// Activating Object Interface
////////////////////////////////////////////////////////////////////////////////
class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
                     int soundToPlay, bool activateOnPlayer,
                     bool activateOnProtester, bool initallyActive)
    :Actor(world, startX, startY, right, true, imageID, 1.0, 2), m_ticksToLive(30){}
    virtual void move(){return;}
    
    // Set number of ticks until this object dies
    void setTicksToLive(){m_ticksToLive = 30;}
private:
    int m_ticksToLive;
};


////////////////////////////////////////////////////////////////////////////////
// Oil barrel Interface
////////////////////////////////////////////////////////////////////////////////
class OilBarrel : public ActivatingObject
{
public:
    OilBarrel(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const;
};


////////////////////////////////////////////////////////////////////////////////
// Gold Nugget Interface
////////////////////////////////////////////////////////////////////////////////
class GoldNugget : public ActivatingObject
{
public:
    GoldNugget(StudentWorld* world, int startX, int startY, bool temporary);
    virtual void move();
    
};


////////////////////////////////////////////////////////////////////////////////
// Sonar Kit Interface
////////////////////////////////////////////////////////////////////////////////
class SonarKit : public ActivatingObject
{
public:
    SonarKit(StudentWorld* world, int startX, int startY);
    virtual void move();
    
};


////////////////////////////////////////////////////////////////////////////////
// Water Pool Interface
////////////////////////////////////////////////////////////////////////////////
class WaterPool : public ActivatingObject
{
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const{return false;}
};



#endif // ACTOR_H_





