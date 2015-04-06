#ifndef TELEPORTER_SHIP_H
#define TELEPORTER_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class TeleporterShip : public Ship
{
    public:
        TeleporterShip(Level *l);

	double get_health();
	void update();

	void create_bullet();
    private:
	void select_target();
	void teleport();

	int moving;
	int tele;
	int shooting;
	int num_state;
	int state;
	int tele_timer;
	int shoot_timer;
	double tx;
	double ty;
	int tn;
};

#endif
