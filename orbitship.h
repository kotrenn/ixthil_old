#ifndef ORBIT_SHIP_H
#define ORBIT_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class OrbitingShip;

class OrbitShip : public Ship
{
    public:
        OrbitShip(Level *l);

	double get_health();
	void die();
	void update();
	void move();
	void draw(Surface *screen);

	friend class OrbitingShip;
    private:
	double r;
	int t;
	int dt;
	int n;
	list<OrbitingShip *> orbits;
};

class OrbitingShip : public Ship
{
    public:
        OrbitingShip(Level *l, OrbitShip *p, double c_r,
		     int c_t, int c_dt);

	void update();
	void move();

	void create_bullet();

	friend class OrbitShip;
    private:
	OrbitShip *parent;
	double r;
	int t;
	int dt;
};

#endif
