class Ship;

#ifndef SHIP_H
#define SHIP_H

#include "actor.h"

class Ship : public Actor
{
    public:
        Ship(Level *level, const char *filename);
	~Ship();

	virtual void update();
	virtual void move();
	virtual void draw(Surface *screen);

	virtual void damage(int pain, int c);
	virtual double get_health();
	virtual void create_bullet();
	virtual void die();
	virtual void ship_died(Ship *ship);
	
	int health;
	int team;
	bool hit;
	bool items;
	bool particles;
	int item_rate;
	bool invincible;
	int score;
	bool collisions;
	int color;
	bool color_hit;
	Ship *parent;
};

#endif
