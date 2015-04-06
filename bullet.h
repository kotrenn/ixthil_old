class Bullet;

#ifndef BULLET_HEADER
#define BULLET_HEADER

#define BULLET_W 6
#define BULLET_H 6

#include "actor.h"
#include "ship.h"

class Bullet : public Actor
{
    public:
        Bullet(Level *level, double x, double y, double w, double h,
	       int t, int c);
	~Bullet();

	static Uint32 get_color(int color);

	virtual void update();
	virtual void draw(Surface *screen);
	virtual void die(Ship *source);

	virtual void ship_died(Ship *ship);

	int team;
	int color;
	bool dead;
	bool colorific;
	bool homing;
	int refract_timer;
};

class LightningBullet : public Bullet
{
    public:
        LightningBullet(Level *l, double x, double y, double w, double h,
			int c, double r);

	void die(Ship *source);
    private:
	double radius;
};

class Lightning : public Bullet
{
    public:
        Lightning(Level *l, Ship *a, Ship *b, int t, int c);

	void update();
	void draw(Surface *screen);

	void ship_died(Ship *ship);
    private:
	void set_points();
	
	Ship *s1;
	Ship *s2;
	double p1[2];
	double p2[2];
	int timer;
	bool ship_dead;
};

class HomingBullet : public Bullet
{
    public:
        HomingBullet(Level *l, double x, double y, double w,
		     double h, int c);

	void update();
	void move();

	void ship_died(Ship *ship);
    private:
	Ship *target;
	double max_v;
};

class AccelBullet : public Bullet
{
    public:
        AccelBullet(Level *l, double x, double y, double w, double h,
		    double a, int t, int c);

	void move();
    private:
	double acc;
	double max_v;
};

class SpecialBullet : public Bullet
{
    public:
        SpecialBullet(Level *l, double x, double y, double w, double h,
		      int t, int c);

	void ship_died(Ship *ship);
	void update();
	void move();
    private:
	Ship *target;
	double max_v;
};

class BombBullet : public Bullet
{
    public:
        BombBullet(Level *l, double x, double y, double w, double h,
	           int t, int c);

	void update();
	void draw(Surface *screen);
    private:
	int max_y;
	int bomb_rad;
};

class DecayBullet : public Bullet
{
    public:
        DecayBullet(Level *l, double x, double y, double w, double h,
	    	    int t, int c);

	void update();
    private:
	int decay_time;
};

class SwerveBullet : public Bullet
{
    public:
        SwerveBullet(Level *l, double x, double y, double w, double h,
		     int t, int c);

	void update();
    private:
	int axis;
};

#endif
