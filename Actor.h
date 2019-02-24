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

        virtual bool blocksFlame() const {return false;}

        virtual bool triggersLandmine() const {return false;}

        virtual bool preventsEscape() const {return false;}

        // every actor runs this every tick
        virtual void doSomething() = 0;

        void die() {m_dead = true;}

        bool isDead() const {return m_dead;}

        virtual bool isZombieTarget() const {return false;}

        virtual void deathrattle() {}

        virtual void infect() {}

        virtual void damage() {}

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

        bool triggersLandmine() const {return true;}

        virtual void damage() {die();}

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

        virtual int getBounty() const = 0;

        void deathrattle();
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

        void uninfect() {m_infected = false; m_infectionCount = 0;}

        bool isInfected() const {return m_infected;}

        int getInfectionCount() const {return m_infectionCount;}

        void doSomething();

        virtual void doPersonAction() = 0;
    
    private:

        int m_infectionCount;

        bool m_infected;

};


class Penelope : public Person {
    public:
        Penelope(StudentWorld* w, double x, double y):
            Person(w, IID_PLAYER, x, y, right), 
            m_numVaccines(0),
            m_ammo(0),
            m_numLandmines(0) {}
        
        ~Penelope() {};

        // this is what differentiates Penelope (so far) from a Citizen
        virtual void doPersonAction();

        int getnumVaccines() const {return m_numVaccines;}

        int getnumLandmines() const {return m_numLandmines;}

        int getAmmo() const {return m_ammo;}

        void gainAmmo() {m_ammo += 5;}

        void pickupVaccine() {m_numVaccines++;}

        void pickupLandmine() {m_numLandmines++;}

    private:
        // penelope data
        // most of this will be factored out eventually

        // since this is complicated, put in a separate method
        void tryFireFlamethrower();

        const int PENELOPE_SPEED = 4;

        int m_numVaccines;

        int m_ammo;

        int m_numLandmines;
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
        virtual bool blocksFlame() const {return true;}
    private:
        // no data, this is a wall
};

class Pit : public Actor {
    public:
        Pit(StudentWorld* w, double x, double y):
            Actor(w, IID_PIT, x, y, right, 1) {}

        ~Pit() {}

        void doSomething();
};

class Exit : public Actor {
    public:
        Exit(StudentWorld*w, double x, double y):
            Actor(w, IID_EXIT, x, y, right, 1) {}
        ~Exit() {};
        void doSomething();
        virtual bool blocksFlame() const {return true;}
    private:

};

class Goodie : public Actor {
    public:
        Goodie(StudentWorld* w, int imageID, double x, double y):
            Actor(w, imageID, x, y, right, 1) {}

        virtual ~Goodie() {};

        void doSomething();

        virtual void getPickedUp() = 0;

        void damage() {die();}

    private:
};

class VaccineGoodie : public Goodie {
    public:
        VaccineGoodie(StudentWorld* w, double x, double y):
            Goodie(w, IID_VACCINE_GOODIE, x, y) {}
        ~VaccineGoodie() {};

        void getPickedUp();

    private:

};

class GasCanGoodie : public Goodie {
    public:
        GasCanGoodie(StudentWorld* w, double x, double y):
            Goodie(w, IID_GAS_CAN_GOODIE, x, y) {}
        ~GasCanGoodie() {};

        void getPickedUp();
};

class LandmineGoodie : public Goodie {
    public:
        LandmineGoodie(StudentWorld* w, double x, double y):
            Goodie(w, IID_LANDMINE_GOODIE, x, y) {}
        ~LandmineGoodie() {};

        void getPickedUp();
};

class DumbZombie : public Zombie {
    public:
        DumbZombie(StudentWorld* w, double x, double y):
            Zombie(w, x, y)
            {};

        ~DumbZombie() {};
        
        void decideMovementDirection();
        int getBounty() const {return 1000;}
    private:
};

class SmartZombie : public Zombie {
    public:
        SmartZombie(StudentWorld* w, double x, double y):
            Zombie(w, x, y)
            {};

        ~SmartZombie() {};
        
        void decideMovementDirection();
        int getBounty() const {return 2000;}
    private:
};

// generalizes Vomit and Flame

class Projectile: public Actor {
    public:
        Projectile(StudentWorld*w, int imageID, double x, double y, Direction dir)
            :Actor(w, imageID, x, y, dir),
            m_numTicksAlive(0)
            {}

        virtual ~Projectile() {}

        void doSomething();

        virtual void doProjectileAction() = 0;
    private:
        int m_numTicksAlive;
};

class Vomit : public Projectile {
    public:
        Vomit(StudentWorld* w, double x, double y):
            Projectile(w, IID_VOMIT, x, y, right) {}
        
        ~Vomit () {}

        void doProjectileAction();
};

class Flame : public Projectile {
    public:
        Flame(StudentWorld* w, double x, double y, Direction dir)
            :Projectile(w, IID_FLAME, x, y, dir) {}

        ~Flame() {}

        void doProjectileAction();
};

class Landmine : public Actor {
    public:
        Landmine(StudentWorld* w, double x, double y)
            :Actor(w, IID_LANDMINE, x, y, right, 1),
            m_numSafetyTicks(30) {}

        ~Landmine() {}

        void doSomething();

        void damage();

        void deathrattle();

    private:
        void explode();
        int m_numSafetyTicks;
};



#endif // ACTOR_H_
