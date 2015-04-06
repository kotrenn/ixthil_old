#include "orbitship.h"

OrbitShip::OrbitShip(Level *l)
    :Ship(l, "orb_yellow.png")
{
    r = 180;
    dims[0] = 0.5 * level->dims[2] - r;
    dims[1] = -3 * r;
    dims[2] = 2 * r;
    dims[3] = 2 * r;
    t = 0;
    dt = -2;
    set_pair(v, 0, 1);
    health = 320;
    score = 18000;
    level->boss = this;
    n = 16;
    double o_r = 1.4 * r;
    for (int i = 0; i < n; i++)
    {
	OrbitingShip *o = new OrbitingShip(level, this, o_r,
					   i * (360.0 / n), 2);
	orbits.push_back(o);
    }
}

double OrbitShip::get_health()
{
    return health / 320.0;
}

void OrbitShip::die()
{
    Ship::die();

    list<OrbitingShip *>::iterator o;
    for (o = orbits.begin(); o != orbits.end(); o++)
    {
	(*o)->invincible = false;
	(*o)->parent = NULL;
	(*o)->damage((*o)->health, -1);
    }
}

void OrbitShip::update()
{
    Ship::update();

    t = (t + dt) % 360;
}

void OrbitShip::move()
{
    Ship::move();

    if (dims[1] > -0.7 * r)
    {
	dims[1] = -0.7 * r;
	v[1] = 0;
    }
    if (dims[0] < 0)
    {
	dims[0] = 0;
	v[0] *= -1;
    }
    if (dims[0] + dims[2] > level->dims[2])
    {
	dims[0] = level->dims[2] - dims[2];
	v[0] *= -1;
    }
}

void OrbitShip::draw(Surface *screen)
{
    Uint32 green = mapRGB(0, 255, 0);
    double cx = dims[0] + r;
    double cy = dims[1] + r;
    double delta = 360.0 / n;
    for (int i = 0; i < n + 1; i++)
    {
	double t1 = (t + (i + 0) * delta) * M_PI / 180.0;
	double t2 = (t + (i + 1) * delta) * M_PI / 180.0;
	draw_line(screen, green,
		  (double[]){cx + r * cos(t1), cy + r * sin(t1)},
		  (double[]){cx + r * cos(t2), cy + r * sin(t2)});
    }
}



OrbitingShip::OrbitingShip(Level *l, OrbitShip *p, double c_r,
			   int c_t, int c_dt)
    :Ship(l, "orbiter.png")
{
    dims[0] = -1000;
    dims[1] = -1000;
    parent = p;
    r = c_r;
    t = c_t;
    dt = c_dt;
    invincible = true;
    items = false;
}

void OrbitingShip::update()
{
    Ship::update();

    t = (t + dt) % 360;

    if (parent != NULL)
    {
	int k = (int)(1.8 + 0.05 * parent->health);
	if (randint(0, parent->n * k) == 0)
	    create_bullet();
    }
}

void OrbitingShip::move()
{
    if (parent != NULL)
    {
	double sx = parent->dims[0] + 0.5 * parent->dims[2];
	double sy = parent->dims[1] + 0.5 * parent->dims[3];
	double theta = t * M_PI / 180.0;
	dims[0] = sx + r * cos(theta) - 0.5 * dims[2];
	dims[1] = sy + r * sin(theta) - 0.5 * dims[3];
    }
}

void OrbitingShip::create_bullet()
{
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, 5);
    set_pair(bullet->v, 2 * randint(-2, 2), 8);
}
