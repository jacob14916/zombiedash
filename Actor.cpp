#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

// class Actor


// class Walker

void Walker::tryMove(double newX, double newY) {
    if (getWorld()->spriteCanGoHere(this, newX, newY)) {
        moveTo(newX,newY);
    }
}

// class Penelope

void Penelope::doSomething() {
    StudentWorld* sw = getWorld();
    int keydown;

    double newX = getX();
    double newY = getY();

    if (sw->getKey(keydown)) {
        switch (keydown)
        {
            case KEY_PRESS_ENTER:
                m_alive = false;
                break;

            case KEY_PRESS_DOWN:
                setDirection(down);
                newY -= PENELOPE_SPEED;
                tryMove(newX, newY);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                newX -= PENELOPE_SPEED;
                tryMove(newX, newY);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                newX += PENELOPE_SPEED;
                tryMove(newX, newY);
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                newY += PENELOPE_SPEED;
                tryMove(newX, newY);
                break;
        
        
        
            default:
                break;
        }
    }
}

// class Wall

void Wall::doSomething() {
    
}