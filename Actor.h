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
        :GraphObject(imageID, startX, startY, dir, depth, size), 
            m_World(w),
            m_dead(false) {
                    // do nothing
                }
        virtual ~Actor() {}

        StudentWorld* getWorld() const {return m_World;}

        virtual bool blocksMovement() const {return false;}

        virtual bool preventsEscape() const {return false;}

        // every actor runs this every tick
        virtual void doSomething() = 0;

        void die() {m_dead = true;}

        bool isDead() const {return m_dead;}

        virtual bool isZombieTarget() const {return false;}

        virtual void deathrattle() {}

        virtual void infect() {}

    private:
        StudentWorld* m_World;

        bool m_dead;
};

class Walker : public Actor {
    public:
    // don't want to assume that this will only be created at beginning
        Walker(StudentWorld* w, int imageID, double x, double y, int dir):
            Actor(w, imageID, x, y, dir) {}
        
        virtual ~Walker() {};

        virtual bool blocksMovement() const {return true;}

        bool tryMove(int direction, int stepLength);
};

class Zombie : public Walker {
    public:
        Zombie(StudentWorld* w, double x, double y):
            Walker(w, IID_ZOMBIE, x, y, right), 
            m_paralyzed(false),
            m_movementPlanDistance(0) {}
        
        virtual ~Zombie() {};
        
        void doSomething();
        virtual void decideMovementDirection() = 0;
    private:
        bool m_paralyzed;
        int m_movementPlanDistance;

};

class Person : public Walker {
    public:
        Person(StudentWorld* w, int imageID, double x, double y, int dir):
            Walker(w, imageID, x, y, dir),
            m_infectionCount(0),
            m_infected(false) {}

        virtual ~Person() {}

        bool isZombieTarget() const {return true;}

        void infect() {m_infected = true;}

        bool isInfected() const {return m_infected;}

        int getInfectionCount() const {return m_infectionCount;}

        void doSomething();

        virtual void decideMovement() = 0;
    
    private:
            
        int m_infectionCount;

        bool m_infected;

};


class Penelope : public Person {
    public:
        Penelope(StudentWorld* w, double x, double y):
            Person(w, IID_PLAYER, x, y, right), 
            m_numVaccines(0) {}
        
        ~Penelope() {};

        // this is what differentiates Penelope (so far) from a Citizen
        virtual void decideMovement();

        int getnumVaccines() const {return m_numVaccines;}

        void pickupVaccine() {m_numVaccines++;}

    private:
        // penelope data
        // most of this will be factored out eventually

        const int PENELOPE_SPEED = 4;

        int m_numVaccines;
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
            Actor(w, IID_VACCINE_GOODIE, x, y, right, 1) {}
        ~VaccineGoodie() {};

        void doSomething();

    private:

};

class DumbZombie : public Zombie {
    public:
        DumbZombie(StudentWorld* w, double x, double y):
            Zombie(w, x, y)
            {};

        ~DumbZombie() {};
        
        void decideMovementDirection();
    private:
};

class SmartZombie : public Zombie {
    public:
        SmartZombie(StudentWorld* w, double x, double y):
            Zombie(w, x, y)
            {};

        ~SmartZombie() {};
        
        void decideMovementDirection();
    private:
};

class Vomit : public Actor {
    public:
        Vomit(StudentWorld* w, double x, double y):
            Actor(w, IID_VOMIT, x, y),
            numTicksAlive(0) {}
        
        ~Vomit () {}

        void doSomething();
    public:
        int numTicksAlive;

};



#endif // ACTOR_H_
