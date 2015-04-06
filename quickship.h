#ifndef QUICKSHIP_H
#define QUICKSHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class QuickShip : public Ship
{
    public:
        QuickShip(Level *l);

	double get_health();
	void update();

	void create_bullet();
    private:
	void select_target();
	double tx;
	double ty;
	int move_timer;
};

#endif
