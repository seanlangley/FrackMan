#include "StudentWorld.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir), m_ticks(0)
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}










////////////////////////////////////////////////////////////////////////////////
// Game Accessing Functions
////////////////////////////////////////////////////////////////////////////////
bool StudentWorld::isThereDirt(int x, int y) const 
{
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < 4; k++)
            if(m_dirt[x+i][y+k] != nullptr)
                return true;
    return false;
}

bool StudentWorld::isThereDirtExact(int x, int y) const
{
    if(m_dirt[x][y] != nullptr)
        return true;
    return false;
}


bool StudentWorld::canActorMoveTo(Actor* a, int x, int y) const
{
    if(isThereDirt(x,y))
        return false;
    return true;
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
                if(x == x1 + i && y == y1 + k)
                    return true;
        
    }
    return false;
}


bool StudentWorld::isThereFrackMan(int x, int y)
{
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < 4; k++)
            if(x == getPlayer()->getX() + i && y == getPlayer()->getY())
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

bool StudentWorld::facingTowardFrackMan(Actor *a) const
{
    ActivatingObject::Direction dir = a->getDirection();
    int actorX = a->getX();
    int actorY = a->getY();
    int playerX = m_player->getX();
    int playerY = m_player->getY();
    
    switch(dir)
    {
        case ActivatingObject::up:
            while(m_dirt[actorX][actorY] == nullptr)
            {
                if(actorX == playerX && actorY == playerY)
                    return true;
                else
                    actorY--;
            }
            break;
            
        case ActivatingObject::down:
            while(m_dirt[actorX][actorY] == nullptr)
            {
                if(actorX == playerX && actorY == playerY)
                    return true;
                else
                    actorY++;
            }
            break;
            
        case ActivatingObject::left:
            while(m_dirt[actorX][actorY] == nullptr)
            {
                if(actorX == playerX && actorY == playerY)
                    return true;
                else
                    actorX--;
            }
            break;
            
        case ActivatingObject::right:
            while(m_dirt[actorX][actorY] == nullptr)
            {
                if(actorX == playerX && actorY == playerY)
                    return true;
                else
                    actorX++;
            }
            break;
            
        case ActivatingObject::none: break;
    }
    return false;
}



void StudentWorld::addActor(Actor *a)
{
    m_actors.push_back(a);
}
////////////////////////////////////////////////////////////////////////////////
// End of Game Accessing Functions
////////////////////////////////////////////////////////////////////////////////














////////////////////////////////////////////////////////////////////////////////
// Game Managing Functions
////////////////////////////////////////////////////////////////////////////////
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
            if(5 - getLevel()/2 > 2)
                return 5 - getLevel()/2;
            else return 2;
            break;
        case IID_PROTESTER:
            if(2 + getLevel()*1.5 < 15)
                return 2 + getLevel()*1.5;
            else return 15;
        default:
            return 0; break;
            
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




Actor* StudentWorld::getActor(int x, int y)
{
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        int x1 = (**it).getX();
        int y1 = (**it).getY();
        
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < 4; k++)
                if(x == x1 + i && y == y1 + k)
                    return *it;
        
    }
    
    return nullptr;
}


void StudentWorld::revealObjects()
{
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if(isNearFrackMan(*it, 12))
            (**it).setVisible(true);
    }
}

////////////////////////////////////////////////////////////////////////////////
// End of Game Managing Functions
////////////////////////////////////////////////////////////////////////////////


















////////////////////////////////////////////////////////////////////////////////
// Actor Managing Functions
////////////////////////////////////////////////////////////////////////////////
void StudentWorld::addGold()
{
    for(int k = 0; k < numObjects(IID_GOLD); k++)
    {
        int x = rand() % 56; int y = rand() % 56;
        
        while(isNearActor(x,y, 5))
        {
            x = rand() % 56, y = rand() % 56;
        }
        
        if(isThereDirt(x,y) || isThereDirt(x+4, y+4))
        {
            
            m_actors.push_back(new GoldNugget(this, x,y, false));
        }
        else --k;
    }
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
        
        while(isNearActor(x,y, 8))
        {
            x = rand() % 56, y = rand() % 56;
            while(y < 20)
                y = rand() % 56;
            while(x < 4 || (x > 22 && x < 33))
                x = rand() % 56;
        }
        
        if(isThereDirt(x,y) || isThereDirt(x+4, y+4))
        {
            m_actors.push_back(new Boulder(this, x,y));
            clearDirt(x,y);
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
        
        while(isNearActor(x,y, 8))
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

void StudentWorld::addNewActors()
{
    int count = 0;
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        if((*it)->getID() == IID_SONAR)
            count++;
    int prob = getLevel()*25 + 300;
    int randInt = rand() % prob + 1;
    if(randInt == prob)
    {
        int probInt2 = rand() % 5 + 1;
        if(probInt2 < 5)
        {
            int x = rand() % 60; int y = rand() % 60;
            
            
            while(isThereDirt(x,y) || isThereBoulder(x,y))
            {
                x = rand() % 64; y = rand() % 60;
            }
            m_actors.push_back(new WaterPool(this, x,y));
        }
        
        else if(probInt2 == 5)
            if(count == 0)
                m_actors.push_back(new SonarKit(this, 20,60));
        
    }
    
    
}

void StudentWorld::addNewProtestors()
{
    int count = 0;
    int prob = 0;
    if(getLevel() * 10 + 30 > 90)
        prob = getLevel() * 10 + 30;
    
    else prob = 90;
    bool regProt = false;
    int probInt = rand() % prob + 1;
    
    if(probInt == prob)
        regProt = false;
    else regProt = true;
    
    
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
        if((*it)->getID() == IID_PROTESTER)
            count++;
    if(count == 0)
    {
        if(regProt)
            m_actors.push_back(new RegularProtester(this, 60,60));
        else
            m_actors.push_back(new HardcoreProtester(this, 60,60));
    }
    
    if(count >= numObjects(IID_PROTESTER))
        return;
    
    int maxTicks = 0;
    if(200 - getLevel() > 15)
        maxTicks = 200 - getLevel();
    else maxTicks = 15;
    
    if(m_ticks < maxTicks)
    {
        m_ticks++;
        return;
    }
    
    else
    {
        if(regProt)
            m_actors.push_back(new RegularProtester(this, 60,60));
        else
            m_actors.push_back(new HardcoreProtester(this, 60,60));
        m_ticks = 0;
    }
    
}
////////////////////////////////////////////////////////////////////////////////
// End of Actor Managing Functions
////////////////////////////////////////////////////////////////////////////////
















////////////////////////////////////////////////////////////////////////////////
// Radius Checking Functions
////////////////////////////////////////////////////////////////////////////////
bool StudentWorld::isNearFrackMan(Actor *a, int radius)
{
    int x = a->getX();
    int y = a->getY();
    
    int temp = radius;
    ////check left
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() - radius + k == x && m_player->getY()-i == y)
                return true;
    ////check down
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() + i == x && m_player->getY() - radius + k == y)
                return true;
    ////check up
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() + i == x && m_player->getY() + radius + 4 - k == y)
                return true;
    ////check right
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() + radius + 4 - k == x && m_player->getY() + i == y)
                return true;
    ////up left diagonal
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() - radius +  k == x && m_player->getY() + radius + 4 - i == y)
                return true;
    ////up right diagonal
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() + radius + 4 - k == x && m_player->getY() + radius + 4 - i == y)
                return true;
    ////down right diagonal
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() + radius + 4 - k == x && m_player->getY() - radius + i == y)
                return true;
    ////down left diagonal
    for(int i = 0; i < 4; i++)
        for(int k = 0; k < temp; k++)
            if(m_player->getX() - radius + k == x && m_player->getY() - radius + i == y)
                return true;
    return false;
    
    
    
}

bool StudentWorld::isNearActor(int x, int y, int radius)
{
    
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        
        int temp = radius;
        ////check left
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius + k == x && (*it)->getY()-i == y)
                    return true;
        ////check down
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + i == x && (*it)->getY() - radius + k == y)
                    return true;
        ////check up
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + i == x && (*it)->getY() + radius + 4 - k == y)
                    return true;
        ////check right
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() + i == y)
                    return true;
        ////up left diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius +  k == x && (*it)->getY() + radius + 4 - i == y)
                    return true;
        ////up right diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() + radius + 4 - i == y)
                    return true;
        ////down right diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() - radius + i == y)
                    return true;
        ////down left diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius + k == x && (*it)->getY() - radius + i == y)
                    return true;
    }
    
    return false;
}

bool StudentWorld::isNearActor(Actor* a, int radius)
{
    int x = a->getX();
    int y = a->getY();
    
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        
        int temp = radius;
        ////check left
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius + k == x && (*it)->getY()-i == y)
                    return true;
        ////check down
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + i == x && (*it)->getY() - radius + k == y)
                    return true;
        ////check up
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + i == x && (*it)->getY() + radius + 4 - k == y)
                    return true;
        ////check right
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() + i == y)
                    return true;
        ////up left diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius +  k == x && (*it)->getY() + radius + 4 - i == y)
                    return true;
        ////up right diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() + radius + 4 - i == y)
                    return true;
        ////down right diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() - radius + i == y)
                    return true;
        ////down left diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius + k == x && (*it)->getY() - radius + i == y)
                    return true;
    }
    
    return false;
}

bool StudentWorld::isNearProtester(int x, int y, int radius)
{
    for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((**it).getID() != IID_PROTESTER || (**it).getID() != IID_HARD_CORE_PROTESTER)
            continue;
        
        int temp = radius;
        ////check left
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius + k == x && (*it)->getY()-i == y)
                    return true;
        ////check down
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + i == x && (*it)->getY() - radius + k == y)
                    return true;
        ////check up
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + i == x && (*it)->getY() + radius + 4 - k == y)
                    return true;
        ////check right
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() + i == y)
                    return true;
        ////up left diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius +  k == x && (*it)->getY() + radius + 4 - i == y)
                    return true;
        ////up right diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() + radius + 4 - i == y)
                    return true;
        ////down right diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() + radius + 4 - k == x && (*it)->getY() - radius + i == y)
                    return true;
        ////down left diagonal
        for(int i = 0; i < 4; i++)
            for(int k = 0; k < temp; k++)
                if((*it)->getX() - radius + k == x && (*it)->getY() - radius + i == y)
                    return true;
    }
    
    return false;
}

////////////////////////////////////////////////////////////////////////////////
// End of Radius Checking Functions
////////////////////////////////////////////////////////////////////////////////











////////////////////////////////////////////////////////////////////////////////
// String Managing Functions
////////////////////////////////////////////////////////////////////////////////
string StudentWorld::setScoreString(int score)
{
    stringstream ss;
    
    // the number is converted to string with the help of stringstream
    ss << score;
    string ret;
    ss >> ret;
    
    // Append zero chars
    int str_length = static_cast<int>(ret.length());
    for (int i = 0; i < 6 - str_length; i++)
        ret = "0" + ret;
    return ret;
}


void StudentWorld::setGameString(string &s)
{
    
    string j = setScoreString(getScore());
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(2);
    
    oss << "Scr: " << j << " Lvl: " << getLevel()+1 << "  Lives: " << getLives() << "  Hlth: " << 10 * getPlayer()->getHitPoints() <<
    "%  Water: " << getPlayer()->getWater() << "  Gld: " << getPlayer()->getGold() << "  Sonar: "
    << getPlayer()->getSonar() << "  Oil Left: " << m_numBarrels;
    s = oss.str();
}

////////////////////////////////////////////////////////////////////////////////
// End of String Managing Functions
////////////////////////////////////////////////////////////////////////////////













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
    m_actors.push_back(new HardcoreProtester(this, 60, 60));
    m_player = new FrackMan(this);
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    string s;
    setGameString(s);
    setGameStatText(s);
    
    addNewActors();
    addNewProtestors();
    
    
    if(m_player->isAlive())
    {
        m_player->move();
        for(vector<Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
            (*it)->move();
        
        
        clearDirt(m_player->getX(), m_player->getY());
        clearDead();
        
    }
    
    
    else
    {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if(m_numBarrels > 0)
        return GWSTATUS_CONTINUE_GAME;
    else if(m_numBarrels == 0)
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



////////////////////////////////////////////////////////////////////////////////
// End of Main Functions
////////////////////////////////////////////////////////////////////////////////










