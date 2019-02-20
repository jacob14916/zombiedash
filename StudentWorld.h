#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <list>
#include <vector>

using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Penelope;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

    // checks for bounding box violation with all actors, except a
    // pass a = this when appropriate, or nullptr if not
    bool spriteCanGoHere(Actor* a, double x, double y);
private:

    string getGameStatText();

    string getLevelFilename();

    list<Actor*> m_Actors;
    Level m_Level;

    Penelope* m_Penelope;
};

#endif // STUDENTWORLD_H_
