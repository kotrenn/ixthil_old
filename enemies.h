#ifndef ENEMIES_H
#define ENEMIES_H

#include "actor.h"
#include "bullet.h"
#include "ship.h"
#include "level.h"

class SimpleShip : public Ship
{
public:
	SimpleShip(Level *);

	void die();
	void update();

	void create_bullet();
private:
	int max_y;
};

class StrongShip : public Ship
{
public:
	StrongShip(Level *);

	void die();
	void update();
	void move();

	void create_bullet();
private:
	int max_y;
};

class AttractorShip : public Ship
{
public:
	AttractorShip(Level *l);

	void die();
	void update();
	void move();

	void create_bullet();
private:
	int axis;
	int a;
	int max_v;
};

class SmartShip : public Ship
{
public:
	SmartShip(Level *l);

	void die();
	void update();
	void move();

	void create_bullet();
private:
	int max_y;
	double speed;
	double max_speed;
};

class BounceShip : public Ship
{
public:
	BounceShip(Level *l);

	void die();
	void update();

	void create_bullet();
private:
	bool in_level;
};

class GroupCreator : public Actor
{
public:
	GroupCreator(Level *l);

	void update();
};

class GroupShip : public Ship
{
public:
	GroupShip(Level *l, double *c_pos, double *c_v,
	          double c_r, int c_t, int c_dt);

	void die();
	void update();
	void move();

	void create_bullet();

	list<GroupShip *> *ship_list;
private:
	double pos[2];
	double r;
	int t;
	int dt;
	bool in_level;
	bool dead;
};

class CloudShip;

class CloudManager
{
public:
	static CloudManager *get_instance();
	~CloudManager();

	void get_a(Ship *ship, double *dst);
	void set_center(double *c);
	void get_center(double *dst);
	void add_ship(CloudShip *ship);
	void remove_ship(CloudShip *ship);
private:
	CloudManager();
	static CloudManager *instance;

	list<CloudShip *> ships;
	double center[2];
};

class CloudShip : public Ship
{
public:
	CloudShip(Level *l);

	void die();
	void update();
	void move();

	void create_bullet();
private:
	CloudManager *cm;
	bool in_level;
	double a[2];
};

class DizzyShip : public Ship
{
public:
	DizzyShip(Level *l);

	void die();
	void update();
	void create_bullet();
private:
	void select_target();

	double tx;
	double ty;
};

class CrystalShip : public Ship
{
public:
	CrystalShip(Level *l);

	void die();
	void update();
	void create_bullet();
private:
	void select_target();

	double tx;
	double ty;
};

class BottomShip : public Ship
{
public:
	BottomShip(Level *l);

	void die();
	void update();
	void move();

	void create_bullet();
private:
	int shoot_timer;
	int shoot_delay;
};

class SpinnerShip : public Ship
{
public:
	SpinnerShip(Level *l);

	void update();
	void draw(Surface *screen);

	void create_bullet();
private:
	Uint32 colors[8];
	int frame;
	bool in_level;
	int n_bullets;
	int theta;
	int theta_v;
	int shoot_timer;
	int shoot_max;
};

class SneakShip : public Ship
{
public:
	SneakShip(Level *l);

	void update();
	void draw(Surface *screen);

	void create_bullet();
private:
	Uint32 colors[8];
	int frame;
	int shoot_timer;
	int shoot_max;
	int shoot_v;
};

class HomerShip : public Ship
{
public:
	HomerShip(Level *l);
	
	void update();
	void draw(Surface *screen);

	void create_bullet();
private:
	Uint32 colors[8];
	int frame;
	bool in_level;
	int shoot_timer;
	int shoot_max;
};

class DodgeShip : public Ship
{
public:
	DodgeShip(Level *l);

	void update();
	void draw(Surface *screen);

	void create_bullet();
private:
	Uint32 colors[8];
	int frame;
	bool in_level;
	double speed;
};

class WaveShip : public Ship
{
public:
	WaveShip(Level *l);

	void update();
	void draw(Surface *screen);

	void create_bullet();
private:
	void select_target();
	
	Uint32 colors[8];
	int frame;
	int theta;
	double tx;
	double ty;
	int shoot_timer;
	int shoot_max;
};

class AssassinShip : public Ship
{
public:
	AssassinShip(Level *l);

	void update();
	void move();
	void draw(Surface *screen);

	void create_bullet();
private:
	Uint32 colors[8];
	int frame;
	int max_y;
	double speed;
	double max_speed;
	int shoot_timer;
	int shoot_max;
	int cone;
};

class BlackHole : public Ship
{
public:
	BlackHole(Level *l);

	void update();
	void draw(Surface *screen);
private:
	bool in_level;
	double theta;
};
	
#endif
