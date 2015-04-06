class Item;

#ifndef ITEM_H
#define ITEM_H

#include "actor.h"

#define IT_BLANK -1
#define IT_BOMB -2
#define IT_HEALTH -3
#define IT_SHIELD -4
#define IT_HEALTH_PLUS -5
#define IT_SHIELD_PLUS -6
#define IT_ENERGY -7
#define IT_PORTAL -8

class Item : public Actor
{
    public:
        Item(Level *l, double x, double y, int type);
	int item_type;
	bool pulled;
	double speed;
};

class EnergyItem : public Item
{
    public:
        EnergyItem(Level *l, double x, double y);

	void update();
	void draw(Surface *screen);
    private:
	int frame;
};

class PortalItem : public Item
{
    public:
        PortalItem(Level *l, int c);

	void update();
	void draw(Surface *screen);

	int get_color() { return color; }
    private:
	int color;
	int r;
	int t0;
	int t1;
};

#endif
