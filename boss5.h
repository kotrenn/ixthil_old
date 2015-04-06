#ifndef BOSS_5_H
#define BOSS_5_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class OrbShip;
class RedOrbShip;
class YellowOrbShip;
class GreenOrbShip;
class TealOrbShip;
class BlueOrbShip;
class PurpleOrbShip;

class MysteryShip : public Ship
{
    public:
        MysteryShip(Level *l);

	double get_health();
	void update();
	void move();
	void draw(Surface *screen);

	OrbShip *get_cur_orb();
	void create_bullet();
	void create_ring();
	void create_extra_bullet();

	friend class OrbShip;
	friend class RedOrbShip;
	friend class YellowOrbShip;
	friend class GreenOrbShip;
	friend class TealOrbShip;
	friend class BlueOrbShip;
	friend class PurpleOrbShip;
    private:
	double max_y;
	list<OrbShip *> orbs;
	int orb_theta;
	int orb_theta_v;
	int state;
	int colors[6];
	int cur_color;
	int fire_timer;
	double thetas[5][2];
	int cur_theta;
};

class OrbShip : public Ship
{
    public:
        OrbShip(Level *l, MysteryShip *p, int i, const char *filename);

	int get_i() { return my_i; }
	void select_target();
	bool has_returned();
	virtual void request_return();

	virtual void damage(int pain, int c);
	virtual void die();
	virtual void update();
	virtual void create_bullet();

	friend class MysteryShip;
    protected:
	MysteryShip *parent;
	int my_i;
	bool returning;
	double tx;
	double ty;
};

class RedOrbShip : public OrbShip
{
    public:
        RedOrbShip(Level *l, MysteryShip *p, int i);

	void request_return();
	void update();

	void create_bullet();
};

class YellowOrbShip : public OrbShip
{
    public:
        YellowOrbShip(Level *l, MysteryShip *p, int i);

	void request_return();
	void update();
    private:
	int shooting;
	int tele;
	int num_states;
	int state;
	int shoot_timer;
	int tele_timer;
};

class GreenOrbShip : public OrbShip
{
    public:
        GreenOrbShip(Level *l, MysteryShip *p, int i);

	void request_return();
	void update();
	void move();

	void create_bullet();
    private:
	double v2[2];
};

class GreenLightning : public Bullet
{
    public:
        GreenLightning(Level *l, double x, double y);

	void draw(Surface *screen);
};

class TealOrbShip : public OrbShip
{
    public:
        TealOrbShip(Level *l, MysteryShip *p, int i);

	void request_return();
	void update();
	void move();
	void draw(Surface *screen);

	void create_bullet();
    private:
	int target_delay;
	double v2[2];
	double lx;
	double ly;
	double ux;
	double uy;
	double lw;
};

class BlueOrbShip : public OrbShip
{
    public:
        BlueOrbShip(Level *l, MysteryShip *p, int i);

	void die();
	void update();
	void move();
	void draw(Surface *screen);
	
	void create_bullet();

	friend class MysteryShip;
	friend class PurpleOrbShip;
    private:
	PurpleOrbShip *friendly;
	int bt;
};

class PurpleOrbShip : public OrbShip
{
    public:
        PurpleOrbShip(Level *l, MysteryShip *p, int i);

	void die();
	void update();
	void move();
	void draw(Surface *screen);

	void create_bullet();

	friend class MysteryShip;
	friend class BlueOrbShip;
    private:
	BlueOrbShip *friendly;
	int bt;
};

#endif
