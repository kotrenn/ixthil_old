#ifndef BALLOS_H
#define BALLOS_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class BallosEye;
class BallosFire;
class BallosLightning;

class Ballos : public Ship
{
    public:
        Ballos(Level *l);

	double get_health();
	void update();
	void move();

	friend class BallosEye;
    private:
	void create_fire();
	void create_lightning();

	int strength;
	double max_y;
	list<BallosEye *> eyes;
	bool moving;
};

class BallosEye : public Ship
{
    public:
        BallosEye(Level *l, Ballos *p, int i);

	void die();
	void update();
	void move();

	void create_bullet();
    private:
	int angle;
	Ballos *parent;
};

class BallosFire : public Bullet
{
    public:
        BallosFire(Level *l, double x, double y);

	void update();
	void draw(Surface *screen);
    private:
	Surface *images[4];
	int frame;
};

class BallosLightning : public Bullet
{
    public:
        BallosLightning(Level *l, double x, double y);

	void draw(Surface *screen);
};

#endif
