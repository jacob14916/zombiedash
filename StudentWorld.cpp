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
}

StudentWorld::~StudentWorld() {
    cleanUp();
}


int StudentWorld::init()
{
    
    // load level
    Level::LoadResult lr = m_Level.loadLevel(getLevelFilename());
    // deal with problems in case it's not there
    if (lr == Level::load_fail_bad_format || lr == Level::load_fail_file_not_found) {
        return GWSTATUS_LEVEL_ERROR;
    }
    // populate map level file
    for (int i = 0; i < LEVEL_WIDTH; i++) {
        for (int j = 0; j < LEVEL_HEIGHT; j++) {
            switch(m_Level.getContentsOf(i,j)) {
            case Level::player:
                m_Penelope = new Penelope(this, i, j);
                m_Actors.push_back(m_Penelope);
                break;
            case Level::wall:
                // so initially I put Wall(this, i, j) but this made the walls cluster together in the corner
                // because the given numbers are too small
                // now the Wall constructor scales this automatically

                m_Actors.push_back(new Wall(this, i, j));
                break;
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
    oss << setw(2) << 1;
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

    if (!m_Penelope->isAlive()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }


    // update stats text

    setGameStatText(getGameStatText());

    return GWSTATUS_CONTINUE_GAME;
}


string StudentWorld::getGameStatText() {
    // warning: only call after init() has been called
    ostringstream oss;
    oss.fill('0');
    oss << "Score: ";
    oss << setw(6) << getScore();
    oss << "  ";
    oss << "Level: ";
    oss << setw(1) << getLevel();
    oss << "  ";
    oss << "Lives: ";
    oss << setw(1) << getLives();
    return oss.str();
}


bool StudentWorld::spriteCanGoHere(Actor* a, double x, double y) {
    for (list<Actor*>::iterator ai = m_Actors.begin(); ai != m_Actors.end(); ai++) {
        if (*ai == a) {
            // avoid the same object causing movement blocking
            continue;
        }

        double xdist = (*ai)->getX() - x;
        double ydist = (*ai)->getY() - y;

        if (xdist < 0) xdist = -xdist;
        if (ydist < 0) ydist = -ydist;

        if (xdist < SPRITE_WIDTH && ydist < SPRITE_HEIGHT) {
            return false;
        }
    }
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
