#ifndef SPIN_SHIP_H
#define SPIN_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class LaserSpinShip;

class SpinShip : public Ship
{
    public:
        SpinShip(Level *l);

	double get_health();
	void die();
	void update();
	void move();
	void draw(Surface *screen);

	void create_bullet();
    private:
	double r;
	int theta;
	int dt;
	int num_sides;
	LaserSpinShip *laser;
};

class LaserSpinShip : public Ship
{
    public:
        LaserSpinShip(Level *l, SpinShip *p);

	void update();
	void draw(Surface *screen);

	friend class SpinShip;
    private:
	SpinShip *parent;
	double laser_count;
	bool entered;
	int theta;
	int dt;
};

#endif
