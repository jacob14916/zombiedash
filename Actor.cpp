#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// TODO
// VaccineGoodie

// class Actor


// class Walker

bool Walker::tryMove(int direction, int stepLength) {
    int newX = getX();
    int newY = getY();

    switch (direction)
    {
        case up:
            newY += stepLength;
            break;
        case down:
            newY -= stepLength;
            break;
        case left:
            newX -= stepLength;
            break;
        case right:
            newX += stepLength;
            break;
    }

    if (getWorld()->spriteCanGoHere(this, newX, newY)) {
        moveTo(newX,newY);
        return true;
    }
    return false;
}

// class Person

void Person::doSomething() {
    if (isDead()) {
        return;
    }
    if (isInfected()) {
        m_infectionCount++;
    }
    if (getInfectionCount() == 500) {
        die();
        return;
    }
    decideMovement();
}

// class Penelope

void Penelope::decideMovement() {
    StudentWorld* sw = getWorld();
    int keydown;

    if (sw->getKey(keydown)) {
        switch (keydown)
        {
            case KEY_PRESS_ENTER:
                die();
                break;

            case KEY_PRESS_UP:
                setDirection(up);
                tryMove(up, PENELOPE_SPEED);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                tryMove(down, PENELOPE_SPEED);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                tryMove(left, PENELOPE_SPEED);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                tryMove(right, PENELOPE_SPEED);
                break;
        
        
        
            default:
                break;
        }
    }
}

// class Wall

void Wall::doSomething() {
    
}

// class Exit

void Exit::doSomething() {
    if (getWorld()->playerCanEscape() && 
            getWorld()->playerOverlapsWithThis(getX(), getY())) {
        getWorld()->completedLevel();
    }
}

// class VaccineGoodie

void VaccineGoodie::doSomething() {
    if (isDead()) {
        return;
    }
    if (getWorld()->playerOverlapsWithThis(getX(), getY())) {
        getWorld()->givePlayerVaccine();
        die();
    }
}

// class Zombie

void Zombie::doSomething() {
    // Section 1 in the spec - check if alive, return immediately if not
    if (isDead()) {
        return;
    }
    // Section 2 - paralyze every other tick
    if (m_paralyzed) {
        m_paralyzed = false;
        return;
    }
    // Section 3 - vomit
    int vomitX = getX();
    int vomitY = getY();

    switch (getDirection())
    {
        case up:
            vomitY += SPRITE_HEIGHT;
            break;
        case down:
            vomitY -= SPRITE_HEIGHT;
            break;
        case left:
            vomitX -= SPRITE_WIDTH;
            break;
        case right:
        default: // shouldn't be needed
            vomitX += SPRITE_WIDTH;
            break;
    }

    int a, b, d2;
    getWorld()->getNearestZombieTarget(vomitX, vomitY, a, b, d2);

    // 1 in 3 chance of vomiting and then not doing anything else this turn
    if (d2 <= 100 && randInt(1,3) == 1) {
        getWorld()->spawnVomit(vomitX, vomitY);
        return;
    }

    if (m_movementPlanDistance == 0) {
        // generate a random integer between 3 and 10 inclusive
        m_movementPlanDistance = randInt(3,10);
        decideMovementDirection();
    }

    if (tryMove(getDirection(), 1)) {
        m_movementPlanDistance -= 1;
    } else {
        m_movementPlanDistance = 0;
    }

    m_paralyzed = true;
}

// class DumbZombie

void DumbZombie::decideMovementDirection() {
    // generate a random integer that is either 0, 90, 180, or 270
    setDirection(90*randInt(0,3));
}

void SmartZombie::decideMovementDirection() {
    int targetx;
    int targety;
    int targetd2;
    getWorld()->getNearestZombieTarget(getX(), getY(), targetx, targety, targetd2);
    if (targetd2 > 6400) {
        setDirection(90*randInt(0,3));
        return;
    }
    bool shouldGoDown = (targety < getY());
    bool shouldGoLeft = (targetx < getX());
    bool goHorizontal = randInt(0,1);
    if (targetx == getX()) { // same column
        if (shouldGoDown) {
            setDirection(down);
        } else {
            setDirection(up);
        }
    } else if (targety == getY()) { // same row
        if (shouldGoLeft) {
            setDirection(left);
        } else {
            setDirection(right);
        }
    } else { // not same row or column
        if (goHorizontal) {
            if (shouldGoLeft) {
                setDirection(left);
            } else {
                setDirection(right);
            }
        } else {
            if (shouldGoDown) {
                setDirection(down);
            } else {
                setDirection(up);
            }
        }
    }
}

// class Vomit

void Vomit::doSomething() {
    if (isDead()) {
        return;
    }
    if (numTicksAlive == 2) {
        die();
        return;
    }

    getWorld()->infectOverlapping(getX(), getY());

    numTicksAlive++;
}