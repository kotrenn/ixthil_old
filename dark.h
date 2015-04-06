#ifndef DARK_H
#define DARK_H

#include "actor.h"
#include "bullet.h"
#include "ship.h"
#include "level.h"

class DarkShip : public Ship
{
    public:
    DarkShip(Level *l, const char *filename);

	virtual void die();
    protected:
	int energy;
};

class DarkSimpleShip : public DarkShip
{
    public:
        DarkSimpleShip(Level *l);

	void update();
	void move();

	void create_bullet();
    private:
	int max_y;
};

class BackAndForthShip : public DarkShip
{
    public:
        BackAndForthShip(Level *l);

	void update();
	void move();

	void create_bullet();
    private:
	int shoot_timer;
	int shoot_delay;
};

class DarkAttractorShip : public DarkShip
{
    public:
        DarkAttractorShip(Level *l);

	void update();
	void move();

	void create_bullet();
    private:
	double axis;
	double a;
	double max_v;
};

class DarkSmartShip : public DarkShip
{
    public:
        DarkSmartShip(Level *l);

	void update();
	void move();

	void create_bullet();
    private:
	double max_y;
	double speed;
	double max_speed;
};

class SplitShip : public DarkShip
{
    public:
        SplitShip(Level *l);

	void die();
	void update();
	
	void create_bullet();
    private:
	bool in_level;
	int size;
};

class OctaGroupCreator : public Actor
{
    public:
        OctaGroupCreator(Level *l);

	void update();
};

class OctaGroupShip : public DarkShip
{
    public:
        OctaGroupShip(Level *l, double *c_pos, double *c_v,
		      double c_r, int c_t, int c_dt);

	void update();
	void move();

	void create_bullet();
    private:
	double pos[2];
	double r;
	int t;
	int dt;
	bool in_level;
};

class BugShip : public DarkShip
{
    public:
        BugShip(Level *l);

	void update();
	void move();

	void create_bullet();
    private:
	double s;
	int timer;
	bool in_level;
};

class TriangleShip : public DarkShip
{
    public:
        TriangleShip(Level *l);

	void die();
	void update();

	void create_bullet();
    private:
	void reset_v();
	void find_target();

	TriangleShip *parent;
	TriangleShip *child;
	int pos;
	double side;
	double max_x;
	double max_y;
	int cx;
	int cy;
	double tx;
	double ty;
	int v_timer;
};

class NetworkShip : public DarkShip
{
    public:
        NetworkShip(Level *l);

	void die();
	void update();
	void draw(Surface *screen);

	void create_bullet();
    private:
	void find_target();

	NetworkShip *parent;
	bool has_child;
	list<NetworkShip *> children;
	int pos;
	double tx;
	double ty;
	int n_b;
	int b_theta;
	int b_theta_v;
	int b_timer;
	int b_timer_max;
};

class VerticalShip : public DarkShip
{
    public:
        VerticalShip(Level *l);

	void update();
	void move();

	void create_bullet();
    private:
	double speed;
	double max_speed;
};

#endif
