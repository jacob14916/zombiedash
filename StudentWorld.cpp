#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_Level(assetPath)
{
    // is this necessary? i think not
    // m_Actors = list<Actor*>();
    m_justCompletedLevel = false;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}


int StudentWorld::init()
{
    
    // load level
    Level::LoadResult lr = m_Level.loadLevel(getLevelFilename());
    // deal with problems in case it's not there
    if (lr == Level::load_fail_bad_format) {
        return GWSTATUS_LEVEL_ERROR;
    }
    if (lr == Level::load_fail_file_not_found) {
        return GWSTATUS_PLAYER_WON;
    }

    // populate map level file
    for (int i = 0; i < LEVEL_WIDTH; i++) {
        for (int j = 0; j < LEVEL_HEIGHT; j++) {
            switch(m_Level.getContentsOf(i,j)) {
            case Level::player:
                m_Penelope = new Penelope(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j);
                m_Actors.push_back(m_Penelope);
                break;
            case Level::wall:
                // so initially I put Wall(this, i, j) but this made the walls cluster together in the corner
                // because the given numbers are too small
                // now the Wall constructor scales this automatically

                m_Actors.push_back(new Wall(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
                break;
            case Level::exit:
                m_Actors.push_back(new Exit(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
                break;
            case Level::vaccine_goodie:
                m_Actors.push_back(new VaccineGoodie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
                break;
            case Level::gas_can_goodie:
                m_Actors.push_back(new GasCanGoodie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
                break;
            case Level::dumb_zombie:
                m_Actors.push_back(new DumbZombie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
                break;
            case Level::smart_zombie:
                m_Actors.push_back(new SmartZombie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
                break;
            case Level::pit:
                m_Actors.push_back(new Pit(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
            case Level::landmine_goodie:
                m_Actors.push_back(new LandmineGoodie(this, SPRITE_WIDTH*i, SPRITE_HEIGHT*j));
            default:
                break;
            }
        }
    }


    return GWSTATUS_CONTINUE_GAME;
}

string StudentWorld::getLevelFilename() {
    ostringstream oss;
    oss.fill('0');
    oss << "level";
    oss << setw(2) << getLevel();
    oss << ".txt";
    return oss.str();
}

int StudentWorld::move()
{
    // This code lets you kill yourself since there's no other way to right now

    for (list<Actor*>::iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        (*ai)->doSomething();
    }

    // look at who's dead
    if (m_Penelope->isDead()) {
        // player died
        // why do i have to call this myself? would make more sense if GameWorld handled
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    // clean out other dead actors
    list<Actor*>::iterator ai = m_Actors.begin();
    while (ai != m_Actors.end()) {
        if ((*ai)->isDead()) {
            (*ai)->deathrattle();
            list<Actor*>::iterator todelete = ai;
            ++ai;
            delete *todelete;
            m_Actors.erase(todelete);
        } else {
            ++ai;
        }
    }

    // update stats text

    setGameStatText(getGameStatText());

    if (m_justCompletedLevel) {
        m_justCompletedLevel = false;
        return GWSTATUS_FINISHED_LEVEL;
    }

    return GWSTATUS_CONTINUE_GAME;
}


string StudentWorld::getGameStatText() {
    // warning: only call after init() has been called
    ostringstream oss;
    oss.fill('0');
    oss << "Score: ";
    oss << setw(6) << getScore();

    oss << " ";
    oss << "Level: ";
    oss << setw(1) << getLevel();

    oss << " ";
    oss << "Lives: ";
    oss << setw(1) << getLives();

    oss << " ";
    oss << "Vaccines: ";
    // causes problems when called after m_Penelope has been deleted
    oss << setw(1) << m_Penelope->getnumVaccines();

    oss << " ";
    oss << "Flames: ";
    // causes problems when called after m_Penelope has been deleted
    oss << setw(1) << m_Penelope->getAmmo();

    oss << " ";
    oss << "Mines: ";
    // causes problems when called after m_Penelope has been deleted
    oss << setw(1) << m_Penelope->getnumLandmines();

    oss << " ";
    oss << "Infected: ";
    oss << setw(1) << m_Penelope->getInfectionCount();
    return oss.str();
}


bool StudentWorld::spriteCanGoHere(Actor* a, int x, int y) const {
    for (list<Actor*>::const_iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if (*ai == a || !((*ai)->blocksMovement())) {
            // avoid the same object causing movement blocking
            continue;
        }
        if(boundingBoxesIntersect(x,y,(*ai)->getX(), (*ai)->getY())) {
            return false;
        }
    }
    return true;
}


bool StudentWorld::playerCanEscape() const {
    for (list<Actor*>::const_iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if ((*ai)->preventsEscape()) {
            return false;
        }
    }
    return true;
}

bool StudentWorld::playerOverlapsWithThis(int x, int y) const {
    return objectsOverlap(x, y, m_Penelope->getX(), m_Penelope->getY());
}

bool StudentWorld::shouldTriggerLandmine(int x, int y) const {
    for (list<Actor*>::const_iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if (!((*ai)->triggersLandmine())) {
            continue;
        }
        if(objectsOverlap(x,y,(*ai)->getX(), (*ai)->getY())) {
            return true;
        }
    }
    return false;
}

void StudentWorld::givePlayerVaccine() {
    m_Penelope->pickupVaccine();
}

void StudentWorld::givePlayerAmmo() {
    m_Penelope->gainAmmo();
}

void StudentWorld::givePlayerLandmine() {
    m_Penelope->pickupLandmine();
}

void StudentWorld::getNearestZombieTarget(int x, int y, 
        int& targetx, int& targety, int& targetd2) const {
    int minx = m_Penelope->getX();
    int miny = m_Penelope->getY();
    int mind2 = (x-minx)*(x-minx) + (y-miny)*(y-miny);
    for (list<Actor*>::const_iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if ((*ai)->isZombieTarget()) {
            int ax = (*ai)->getX();
            int ay = (*ai)->getY();
            int d2 = (x-ax)*(x-ax) + (y-ay)*(y-ay);
            if (d2 < mind2) {
                minx = ax;
                miny = ay;
                mind2 = mind2;
            }
        }
    }
    targetx = minx;
    targety = miny;
    targetd2 = mind2;
}

void StudentWorld::infectOverlapping(int x, int y) {
    for (list<Actor*>::iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if ((*ai)->isZombieTarget() && 
            objectsOverlap(x, y, (*ai)->getX(), (*ai)->getY())) {
            (*ai)->infect();
        }
    }
}

void StudentWorld::damageOverlapping(int x, int y) {
    for (list<Actor*>::iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if (objectsOverlap(x, y, (*ai)->getX(), (*ai)->getY())) {
            (*ai)->damage();
        }
    }
}

void StudentWorld::spawnVomit(int x, int y) {
    m_Actors.push_back(new Vomit(this, x, y));
}

void StudentWorld::trySpawnVaccineGoodie(int x, int y) {
    for (list<Actor*>::iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if (objectsOverlap(x, y, (*ai)->getX(), (*ai)->getY())) {
            return;
        }
    }
    m_Actors.push_back(new VaccineGoodie(this, x, y));
}

void StudentWorld::spawnLandmine(int x, int y) {
    m_Actors.push_back(new Landmine(this, x, y));
}

void StudentWorld::spawnPit(int x, int y) {
    m_Actors.push_back(new Pit(this, x, y));
}

bool StudentWorld::spawnFlame(int x, int y, int dir) {
    for (list<Actor*>::iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if ((*ai)->blocksFlame() && 
            objectsOverlap(x, y, (*ai)->getX(), (*ai)->getY())) {
            return false;
        }
    }
    m_Actors.push_back(new Flame(this, x, y, dir));
    return true;
}

void StudentWorld::cleanUp()
{
    
    while (!m_Actors.empty()) {
        delete *m_Actors.begin();
        m_Actors.erase(m_Actors.begin());
    }
    // should have been deleted already
    m_Penelope = nullptr;
}
