#ifndef SURROUND_SHIP_H
#define SURROUND_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class SurroundGun;
class SurroundShip;

class SurroundManager : public Ship
{
    public:
        SurroundManager(Level *l);

	double get_health();
	void die();
	void update();
	void move();

	void create_bullet();

	friend class SurroundGun;
	friend class SurroundShip;
    private:
	int num_ships;
	double r;
	list<SurroundShip *> ships;
	list<SurroundGun *> guns;
	int fire_timer;
	double max_y;
	int flip_timer;
};

class SurroundGun : public Ship
{
    public:
        SurroundGun(Level *l, SurroundManager *p, double x);

	void update();
	void move();

	void create_bullet();

	friend class SurroundManager;
    private:
	SurroundManager *parent;
	double dx;
	int fire_timer;
};

class SurroundShip : public Ship
{
    public:
        SurroundShip(Level *l, SurroundManager *p, int c_n);

	void update();
	void move();

	void create_bullet();

	friend class SurroundManager;
    private:
	SurroundManager *parent;
	int n;
	double r;
	double theta;
	int fire_timer;
};

#endif
