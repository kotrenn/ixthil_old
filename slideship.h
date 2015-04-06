#ifndef SLIDE_SHIP_H
#define SLIDE_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class SlideShip : public Ship
{
    public:
        SlideShip(Level *l);

	double get_health();
	void update();
	void move();

	void create_bullet();
    private:
	double s;
	int timer;
	bool in_level;
};

#endif
