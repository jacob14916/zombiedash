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

        virtual bool blocksMovement() const {return false;}

        virtual bool preventsEscape() const {return false;}

        // every actor runs this every tick
        virtual void doSomething() = 0;

        virtual bool isDead() const {return false;}

        virtual void deathrattle() {}

    private:
        StudentWorld* m_World;
};

class Walker : public Actor {
    public:
    // don't want to assume that this will only be created at beginning
        Walker(StudentWorld* w, int imageID, double x, double y, int dir):
            Actor(w, imageID, x, y, dir) {}
        
        virtual ~Walker() {};

        virtual bool blocksMovement() const {return true;}

        void tryMove(double newX, double newY);
};


class Penelope : public Walker {
    public:
        Penelope(StudentWorld* w, double x, double y):
            Walker(w, IID_PLAYER, x, y, right), m_numVaccines(0) {}
        
        ~Penelope() {};

        void doSomething ();

        bool isDead() const {return m_dead;}

        int getnumVaccines() const {return m_numVaccines;}

        void pickupVaccine() {m_numVaccines++;}

    private:
        // penelope data
        // most of this will be factored out eventually

        const int PENELOPE_SPEED = 4;

        int m_numVaccines;

        bool m_dead = false;
        
};


class Wall : public Actor {
    public:
        Wall(StudentWorld* w, double x, double y):
            Actor(w, IID_WALL, x, y, right) {}
        ~Wall() {
            // do nothing
        }
        // since this is a wall, do nothing
        void doSomething();
        virtual bool blocksMovement() const {return true;}
    private:
        // no data, this is a wall
};


class Exit : public Actor {
    public:
        Exit(StudentWorld*w, double x, double y):
            Actor(w, IID_EXIT, x, y, right, 1) {}
        ~Exit() {};
        void doSomething();
    private:

};

class VaccineGoodie : public Actor {
    public:
        VaccineGoodie(StudentWorld* w, double x, double y):
            Actor(w, IID_VACCINE_GOODIE, x, y, right, 1), m_dead(false) {}
        ~VaccineGoodie() {};

        void doSomething();

        virtual bool isDead() const {return m_dead;}

    private:
        bool m_dead;

};



#endif // ACTOR_H_
