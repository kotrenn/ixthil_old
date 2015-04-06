#ifndef STATE_SHIP_H
#define STATE_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class StateShip : public Ship
{
    public:
        StateShip(Level *l);

	double get_health();
	void damage(int pain, int c);
	void die();
	void update();
	void draw(Surface *screen);
	
	void create_bullet();
    private:
	int state;
	int code[6];
	int state_health;
	int final_health;
	double center[2];
	int dt;
	int ddt;
};

class PasswordActor : public Actor
{
    public:
        PasswordActor(Level *l);
	~PasswordActor();

	void update();
	void draw(Surface *screen);
};

#endif
