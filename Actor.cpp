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
    doPersonAction();
}

// class Penelope

void Penelope::doPersonAction() {
    StudentWorld* sw = getWorld();
    int keydown;

    if (sw->getKey(keydown)) {
        switch (keydown)
        {
            case KEY_PRESS_ENTER:
                // use vaccine
                if (m_numVaccines > 0) {
                    uninfect();
                    m_numVaccines--;
                }
                break;
            case KEY_PRESS_SPACE:
                // fire flamethrower
                tryFireFlamethrower();
                break;
            case KEY_PRESS_TAB:
                if (m_numLandmines > 0) {
                    getWorld()->spawnLandmine(getX(), getY());
                    m_numLandmines--;
                }
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

void Penelope::tryFireFlamethrower() {
    // could also be done recursively
    if (m_ammo == 0) {
        return;
    }

    m_ammo--;

    int flamex = getX();
    int flamey = getY();

    Direction dir = getDirection();

    for (int i = 0; i < 3; i++) {
        switch (dir)
        {
            case up: flamey += SPRITE_HEIGHT;       break;
            case down: flamey -= SPRITE_HEIGHT;     break;
            case left: flamex -= SPRITE_WIDTH;      break;
            case right: flamex += SPRITE_HEIGHT;    break;
        
            default:
                break;
        }
        if (!getWorld()->spawnFlame(flamex, flamey, dir)) {
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

// class Pit

void Pit::doSomething() {
    getWorld()->damageOverlapping(getX(), getY());
}

// class Goodie

void Goodie::doSomething() {
    if (isDead()) {
        return;
    }
    if (getWorld()->playerOverlapsWithThis(getX(), getY())) {
        getWorld()->increaseScore(50);
        getPickedUp();
        die();
    }
}

// class VaccineGoodie

void VaccineGoodie::getPickedUp() {
    getWorld()->givePlayerVaccine();
}

// class GasCanGoodie

void GasCanGoodie::getPickedUp() {
    getWorld()->givePlayerAmmo();
}

void LandmineGoodie::getPickedUp() {
    getWorld()->givePlayerLandmine();
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

void Zombie::deathrattle() {
    getWorld()->increaseScore(getBounty());
    getWorld()->spawnVaccineGoodie(getX(), getY());
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

// class Projectile

void Projectile::doSomething() {
    if (isDead()) {
        return;
    }
    if (m_numTicksAlive == 2) {
        die();
        return;
    }
    doProjectileAction();
    m_numTicksAlive++;
}

// class Vomit

void Vomit::doProjectileAction() {
    getWorld()->infectOverlapping(getX(), getY());
}

// class Flame

void Flame::doProjectileAction() {
    getWorld()->damageOverlapping(getX(), getY());
}

// class Landmine

void Landmine::doSomething () {
    if (isDead()) return;
    if (m_numSafetyTicks > 0) {
        m_numSafetyTicks--;
        return;
    }
    if (getWorld()->shouldTriggerLandmine(getX(), getY())) {
        cout << "Managed to run shouldTriggerLandmine" << endl;
        explode();
    }
}

void Landmine::damage() {
    if (isDead()) {
        // prevent infinite loop where landmine is damaged by its own flame
        return;
    }
    explode();
}

void Landmine::explode() {
    die();
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            getWorld()->spawnFlame(getX() + SPRITE_WIDTH*i,
                getY() + SPRITE_HEIGHT*j, right); 
        }
    }
}

void Landmine::deathrattle() {
    getWorld()->spawnPit(getX(), getY());
}