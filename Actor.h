// Possible interfaces for actors.  You may use all, some, or none
// of this, as you wish.

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <list>

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
    virtual void setAnnoyed(){return;}
    
    // Is this actor alive?
    bool isAlive() const {if(m_isAlive) return true; else return false;}
    
    // Mark this actor as dead.
    void setDead() {m_isAlive = false;}
    
    // Annoy this actor.
    virtual bool annoy(unsigned int amt) = 0;
    
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
    virtual bool canIBePickedUp(){return false;}
    virtual void addGold(){return;}
    
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
          int imageID, double size, int depth, unsigned int hitPoints);
    
    
    virtual void addGold();
    
    
    virtual void go(Direction dir) = 0;
    virtual bool annoy(unsigned int amt);
    virtual bool canPickThingsUp() const{return true;}
    virtual void setAnnoyed(){return;}
    virtual unsigned int getGold() const{return m_numNuggets;}
    
    void getHurt(){m_hitPoints--;}
    void checkHealth(){ if(m_hitPoints <= 0) setDead();}
    int getHitPoints() const{return m_hitPoints;}
    
private:
    int m_numNuggets;
    int m_hitPoints;
    char thisMap[66][66];
};


////////////////////////////////////////////////////////////////////////////////
// FrackMan Interface
////////////////////////////////////////////////////////////////////////////////
class FrackMan : public Agent
{
public:
    FrackMan(StudentWorld* world);
    virtual void move();
    virtual bool canDigThroughDirt() const{return true;}
    
    
    // Pick up a sonar kit.
    void addSonar();
    void decreaseSonar();
    void addGold();
    // Pick up water.
    void addWater();
    void checkEnemies();
    // Get amount of gold
    
    
    // Get amount of sonar charges
    unsigned int getSonar() const{return m_numSonar;}
    virtual unsigned int getGold() const{return m_numNuggets;}
    // Get amount of water
    unsigned int getWater() const{return m_numWater;}
    void dropGold(Direction dir);
    void getGoodie();
    void shootGun();
    void decreaseAmmo(){m_numWater--;}
    void go(Direction dir);
    int getSafeTicks();
private:
    int m_score;
    int m_numNuggets;
    int m_numSonar;
    int m_numWater;
    int m_ticksPassed;
    bool gotHit;
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
    virtual void addGold();
    virtual bool huntsFrackMan() const{return true;}
    
    // Set state to having gien up protest

    bool shouldProtestorLeave(){return shouldILeave;}
    bool isValidMove(Direction dir);
    bool madeHorizontalMove(Direction startDir, Direction newDir);
    bool amIAtAnIntersection(int currX, int currY);
    
    // Set number of ticks until next move
    
    int getSquaresToMove(){return m_numSquaresToMove;}
    int ticksToWait();
    int findPath(int x, int y);
    int restingTicks();

    
    void moveRandomly();
    void go(Direction dir);
    void getGold();
    
    
    
    
    void decreaseSquaresToMove(){m_numSquaresToMove--;}
    void checkBullets();
    void setTicksToNextMove(){return;}
    void setSquaresToMove(){m_numSquaresToMove = 8;}
    void setMustLeaveOilField(){shouldILeave = true;}
    void findExit();
    void printMap();
    void setMap();
    void findExit2();
    void huntFrackMan();
    void makePerpindicularMove();
    void setAnnoyed(){gotHit = true;}
    void checkFrackMan();
    
    void changeDirectionsIfPossible();
    
private:
    
    class Coord
    {
    public:
        Coord(int rr, int cc): m_r(rr), m_c(cc){}
        int r() const{return m_r;}
        int c() const{return m_c;}
        
    private:
        int m_r;
        int m_c;
    };
    
    
    bool shouldILeave;
    bool foundFrackMan;
    bool gotHit;
    bool dirtMap[64][64];
    
    int m_numSquaresToMove;
    int m_ticksPassed;
    int m_ticksSinceHorizontalMove;
    int m_ticksWhileStunned;
    
    char thisMap[64][64];
    
    std::list<Direction> wayOut;
    
};






////////////////////////////////////////////////////////////////////////////////
// Regular Protestor Interface
////////////////////////////////////////////////////////////////////////////////
class RegularProtester : public Protester
{
public:
    RegularProtester(StudentWorld* world, int startX, int startY);
    virtual void addGold(){return;}
};


////////////////////////////////////////////////////////////////////////////////
// Hardcore Protestor Interface
////////////////////////////////////////////////////////////////////////////////
class HardcoreProtester : public Protester
{
public:
    HardcoreProtester(StudentWorld* world, int startX, int startY);
    virtual void addGold(){return;}
};


////////////////////////////////////////////////////////////////////////////////
// Dirt Interface
////////////////////////////////////////////////////////////////////////////////
class Dirt : public Actor
{
public:
    Dirt(StudentWorld* world, int startX, int startY);
    virtual bool annoy(unsigned int amt){return false;}
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
    virtual bool annoy(unsigned int amt){return false;}
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
    virtual bool canIBePickedUp(){return false;}
    virtual bool annoy(unsigned int amt){return false;}
    bool isThereProtestor();
    int getTravelDistance(){return m_travelDistance;}
    void decreaseTravelDistance(){m_travelDistance--;}
private:
    int m_travelDistance;
    bool annoyedSomeone;
};


////////////////////////////////////////////////////////////////////////////////
// Activating Object Interface
////////////////////////////////////////////////////////////////////////////////
class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* world, int startX, int startY, int imageID,
                     int soundToPlay, bool activateOnPlayer,
                     bool activateOnProtester, bool initallyActive, Direction dir, bool visible);
    virtual void move();
    virtual bool annoy(unsigned int amt){return false;}
    virtual void setTicksToLive();
    int getTicksToLive(){return m_ticksToLive;}
    void decreaseTicksToLive(){m_ticksToLive--;}
    virtual bool canIBePickedUp(){return true;}
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
    GoldNugget(StudentWorld* world, int startX, int startY, bool visible);
    virtual void move();
    void setDropped(){dropped = true;}
    bool didDrop(){return dropped;}
    
private:
    bool dropped;
    
};


////////////////////////////////////////////////////////////////////////////////
// Sonar Kit Interface
////////////////////////////////////////////////////////////////////////////////
class SonarKit : public ActivatingObject
{
public:
    SonarKit(StudentWorld* world, int startX, int startY);
    virtual void move(){return;}
};


////////////////////////////////////////////////////////////////////////////////
// Water Pool Interface
////////////////////////////////////////////////////////////////////////////////
class WaterPool : public ActivatingObject
{
public:
    WaterPool(StudentWorld* world, int startX, int startY);
//    virtual void move();
    virtual bool needsToBePickedUpToFinishLevel() const{return false;}
};



#endif // ACTOR_H_






