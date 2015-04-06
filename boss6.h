#ifndef BOSS_6_H
#define BOSS_6_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class MirrorShip;
class PrismPlane;
class SlowField;
class BulletMagnet;
class CrystalSummoner;
class DiamondShip;

class PrismShip : public Ship
{
    public:
        PrismShip(Level *l);

	double get_health();
	void die();
	void update();
	void draw(Surface *screen);
	
	void create_bullet();

	friend class MirrorShip;
	friend class PrismPlane;
	friend class SlowField;
	friend class BulletMagnet;
	friend class CrystalSummoner;
	friend class DiamondShip;
    private:
	void switch_state();
	void move_to_target(double *dx_p, double *dy_p);
	void summon_crystals();
	void summon_enemy();
	
	int max_health;
	bool healing;
	int state;
	int max_state;
	double tx;
	double ty;
	list<Ship *> summons;
	list<MirrorShip *> mirrors;
	int bomb_rad;
	int bomb_max;
	int num_diamond;
	int laser_timer;
};

class MirrorShip : public Ship
{
    public:
        MirrorShip(Level *l, PrismShip *p);
	
	void update();
	void move();
	void draw(Surface *screen);

	void create_bullet();

	friend class PrismShip;
    private:
	PrismShip *parent;
	int theta;
	double n[2];
	double r;
	bool shooting;
	bool in_level;
};

class PrismPlane : public Ship
{
    public:
        PrismPlane(Level *l, PrismShip *p, double *c_p1, double *c_p2);

	void update();
	void move();
	void draw(Surface *screen);

	friend class PrismShip;
    private:
	double p1[2];
	double p2[2];
	double p1_off[2];
	double p2_off[2];
	double n[2];
	double r;
};

class SlowField : public Ship
{
    public:
        SlowField(Level *l, PrismShip *p, double *c_p1, double *c_p2);

	void update();
	void move();
	void draw(Surface *screen);

	friend class PrismShip;
    private:
	double p1[2];
	double p2[2];
	double p1_off[2];
	double p2_off[2];
	double n[2];
	double r;
};

class BulletMagnet : public Ship
{
    public:
        BulletMagnet(Level *l, PrismShip *p, double *c_p1, double *c_p2);

	void update();
	void move();
	void draw(Surface *screen);

	friend class PrismShip;
    private:
	bool constant;
	double p1[2];
	double p2[2];
	double p1_off[2];
	double p2_off[2];
	double n[2];
	double r;
};

class CrystalSummoner : public Ship
{
    public:
        CrystalSummoner(Level *l, PrismShip *p, double x, double y, int c);

	void update();
	void move();

	void create_bullet();

	friend class PrismShip;
    private:
	double offset[2];
	int color_off;
};

class DiamondShip : public Ship
{
    public:
        DiamondShip(Level *l, PrismShip *p, double c_r1,
		    double r2, int c_t, int c_dt);

	void die();
	void update();
	void move();

	void create_bullet();
	
	friend class PrismShip;
    private:
	PrismShip *parent;
	double r1;
	double r2;
	double r1_max;
	double r2_max;
	int t;
	int dt;
};

#endif
