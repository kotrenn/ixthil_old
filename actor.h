class Actor;

#ifndef ACTOR_H
#define ACTOR_H

#include "resourcemanager.h"

//#include "level.h"
class Level;

class Actor
{
    public:
        Actor(Level *l, double x, double y, double w, double h);
	~Actor();

	virtual void update();
	virtual void move();
	virtual void draw(Surface *screen);

	virtual void die();

	double dims[4];
	double v[2];
	
	Level *level;
    protected:
	ResourceManager *rm;
	Surface *image;
};

class TimedParticle : public Actor
{
    public:
        TimedParticle(Level *l, double x, double y, double *vel, int t);

	void update();
    private:
	int timer;
};

class LaserEffect : public Actor
{
    public:
        LaserEffect(Level *l, double *a, double *b,
		    Uint32 c, int t);

	void update();
	void draw(Surface *screen);
    private:
	double p1[2];
	double p2[2];
	int timer;
	int timer_max;
	Uint32 color;
};

#endif
