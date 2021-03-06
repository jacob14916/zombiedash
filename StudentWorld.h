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

    // checks for bounding box violation with all actors, except a;
    // pass a = this when appropriate, or nullptr if not
    bool spriteCanGoHere(Actor* a, int x, int y) const;

    // see if there are any citizens left alive
    bool playerCanEscape() const;

    // see if Penelope overlaps with the object at (x,y)
    bool playerOverlapsWithThis(int x, int y) const;

    // used by Exit to save citizens
    void saveOverlapping(int x, int y);

    // see if landmine at x,y should be triggered
    bool shouldTriggerLandmine(int x, int y) const;

    // give Penelope a vaccine (called by VaccineGoodie objects)
    void givePlayerVaccine();

    // used by GasCanGoodie to give Penelope 5 ammo
    void givePlayerAmmo();

    // used by LandmineGoodie to give Penelope 2 landmines
    void givePlayerLandmine();

    // used by SmartZombie to find targets
    void getNearestZombieTarget(int x, int y, 
        int& targetx, int& targety, int& targetd2) const;

    // used by Citizen to find the player
    void getPlayerLocationAndDistance(int x, int y, 
        int& targetx, int& targety, int& targetd2) const;

    // used by Citizen to find zombies to run away from,
    // returns false if no zombies 
    bool getNearestScaryActor(int x, int y, 
        int& targetx, int& targety, int& targetd2) const;

    // used by Vomit to infect overlapping people
    void infectOverlapping(int x, int y);

    // used by Zombie to spawn vomit
    void spawnVomit(int x, int y);
    
    // used by DumbZombie to spawn vaccine goodie when it dies
    void trySpawnVaccineGoodie(int x, int y);

    // used by Penelope to spawn landmines
    void spawnLandmine(int x, int y);

    void spawnPit(int x, int y);

    
    // used by Citizen to spawn zombies when they die of infection
    void spawnSmartZombie(int x, int y);
    void spawnDumbZombie(int x, int y);

    // used by Penelope and Landmine to spawn flame
    bool spawnFlame(int x, int y, int dir);

    // used by Flame to burn overlapping burnables
    void damageOverlapping(int x, int y);

    static bool boundingBoxesIntersect(double x1, double y1, double x2, double y2) {
        double xdist = x1 - x2;
        double ydist = y1 - y2;

        if (xdist < 0) xdist = -xdist;
        if (ydist < 0) ydist = -ydist;

        return (xdist < SPRITE_WIDTH && ydist < SPRITE_HEIGHT);
    };

    static bool objectsOverlap(double x1, double y1, double x2, double y2) {
        double xdist = x1 - x2;
        double ydist = y1 - y2;

        return (xdist*xdist + ydist*ydist) <= 100;
    };

    void completedLevel () {m_justCompletedLevel = true;}

private:

    string getGameStatText();

    string getLevelFilename();

    list<Actor*> m_Actors;
    Level m_Level;

    Penelope* m_Penelope;

    bool m_justCompletedLevel;
};

#endif // STUDENTWORLD_H_
