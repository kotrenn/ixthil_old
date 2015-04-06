#ifndef BIG_SHIP_H
#define BIG_SHIP_H

#include "actor.h"
#include "bullet.h"
#include "level.h"
#include "ship.h"

class BigShipConnector;
class BigShipLaser;

class BigShip : public Ship
{
    public:
        BigShip(Level *l);

	double get_health();
	void die();
	void update();
	void move();
	void draw(Surface *screen);

	void ship_died(Ship *ship);
	void create_bullet();
	void main_gun();

	friend class BigShipConnector;
	friend class BigShipLaser;
    private:
	void reposition();
	
	BigShipConnector *left_conn;
	BigShipLaser *left_laser;
	BigShipConnector *right_conn;
	BigShipLaser *right_laser;
	double tx;
	double ty;
	int target_delay;
	int gun_theta;
	double gx1;
	double gx2;
	double gy;
	double gv;
	int ship_timer;
	list<Ship *> ships;
};

class BigShipConnector : public Ship
{
    public:
        BigShipConnector(Level *l, BigShip *p);

	void die();
	void damage(int pain, int c);
	void move();

	friend class BigShip;
	friend class BigShipLaser;
    private:
	BigShip *parent;
	BigShipLaser *laser;
	double offset[2];
};

class BigShipLaser : public Ship
{
    public:
        BigShipLaser(Level *l, BigShip *p);

	void die();
	void update();
	void move();
	void draw(Surface *screen);

	friend class BigShip;
	friend class BigShipConnector;
    private:
	BigShip *parent;
	BigShipConnector *conn;
	double offset[2];
	double lx;
	double ly;
	double ux;
	double uy;
	double lw;
};

#endif
