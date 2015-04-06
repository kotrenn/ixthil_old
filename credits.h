#ifndef CREDITS_H
#define CREDITS_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class WordShip;

class Credits : public Ship
{
    public:
        Credits(Level *l);

	void update();
};

class WordShip : public Ship
{
    public:
        WordShip(Level *l, double y, const char *msg);
	~WordShip();

	void update();
};

#endif
