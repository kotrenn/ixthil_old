#ifndef COLOR_SHIP_H
#define COLOR_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class ColorBody;
class ColorArmor;
class ColorGun;

class ColorShip : public Ship
{
    public:
        ColorShip(Level *l);

	double get_health();
	void damage(int pain, int c);
	void die();
	void update();
	void move();
	void draw(Surface *screen);

	friend class ColorArmor;
	friend class ColorGun;
	friend class ColorBody;
    private:
	void select_target();
	int colors[4];
	list<Ship *> parts;
	ColorGun *left_gun;
	ColorGun *right_gun;
	double max_y;
	double tx;
	int laser_timer;
};

class ColorBody : public Ship
{
    public:
        ColorBody(Level *l, ColorShip *p, double px,
		  double py, int side);

	void move();
    private:
	double pos[2];
};

class ColorArmor : public Ship
{
    public:
        ColorArmor(Level *l, ColorShip *p, double px,
		  double py, int side);

	void move();
    private:
	double pos[2];
};

class ColorGun : public Ship
{
    public:
        ColorGun(Level *, ColorShip *p, double px,
	         double py, int c_id);

	void update();
	void move();

	void create_bullet();

	friend class ColorShip;
    private:
	double pos[2];
	int color_id;
	int fire_timer;
};

class ColorActor : public Actor
{
    public:
        ColorActor(Level *l);
	~ColorActor();

	void update();
	void draw(Surface *screen);
};

#endif
