#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject {
    public:

        Actor(StudentWorld* w, int imageID, double startX, double startY, 
            int dir = 0, int depth = 0, double size = 1.0)
        :GraphObject(imageID, startX, startY, dir, depth, size), m_World(w) {
                    // do nothing
                }
        virtual ~Actor() {}

        StudentWorld* getWorld() const {return m_World;}

        // every actor runs this every tick
        virtual void doSomething() = 0;

    private:
        StudentWorld* m_World;
};

class Walker : public Actor {
    public:
    // don't want to assume that this will only be created at beginning
        Walker(StudentWorld* w, int imageID, double x, double y, int dir):
            Actor(w, imageID, x, y, dir) {}
        
        virtual ~Walker() {};

        void tryMove(double newX, double newY);
};


class Penelope : public Walker {
    public:
        Penelope(StudentWorld* w, int levelx, int levely):
            Walker(w, IID_PLAYER, SPRITE_WIDTH*levelx, SPRITE_HEIGHT*levely, right) {}
        
        ~Penelope() {};

        void doSomething ();

        bool isAlive() const {return m_alive;}

    private:
        // penelope data
        // most of this will be factored out eventually

        const int PENELOPE_SPEED = 4;

        bool m_alive = true;
        
};


class Wall : public Actor {
    public:
        Wall(StudentWorld* w, int levelx, int levely):
            Actor(w, IID_WALL, SPRITE_WIDTH*levelx, SPRITE_HEIGHT*levely, right) {}
        ~Wall() {
            // do nothing
        }
        // since this is a wall, do nothing
        void doSomething();
    private:
        // no data, this is a wall
};



#endif // ACTOR_H_
