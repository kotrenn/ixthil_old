#include "spinship.h"

SpinShip::SpinShip(Level *l)
    :Ship(l, "orb_purple.png")
{
    r = 50;
    dims[0] = 0.5 * level->dims[2] - r;
    dims[1] = -200;
    dims[2] = 2 * r;
    dims[3] = 2 * r;
    set_pair(v, 0, 8);
    theta = 0;
    dt = -2;
    health = 300;
    level->boss = this;
    score = 30000;
    num_sides = 8;
    laser = NULL;
}

double SpinShip::get_health()
{
    return health / 300.0;
}

void SpinShip::die()
{
    Ship::die();

    if (laser == NULL)
	return;

    laser->parent = NULL;
    laser->invincible = false;
    laser->damage(laser->health, -1);
}

void SpinShip::update()
{
    Ship::update();

    theta = (theta + dt) % 360;

    if (theta % 24 == 0)
	create_bullet();

    if (health < 200 && laser == NULL)
	laser = new LaserSpinShip(level, this);

    if (randint(0, 100) == 0)
	v[0] *= -1;
}

void SpinShip::move()
{
    Ship::move();

    if (dims[1] > 0.5 * r)
    {
	dims[1] = 0.5 * r;
	v[0] = 8;
	v[1] = 0;
    }
    if (dims[0] < 0)
    {
	dims[0] = 0;
	v[0] *= -1;
	dt *= -1;
    }
    if (dims[0] + dims[2] > level->dims[2])
    {
	dims[0] = level->dims[2] - dims[2];
	v[0] *= -1;
	dt *= -1;
    }
}

void SpinShip::draw(Surface *screen)
{
    Uint32 blue = mapRGB(0, 0, 255);
    double cx = dims[0] + r;
    double cy = dims[1] + r;
    double dt = 360.0 / num_sides;
    for (int i = 0; i <= num_sides; i++)
    {
	double t1 = (theta + (i + 0) * dt) * M_PI / 180.0;
	double t2 = (theta + (i + 1) * dt) * M_PI / 180.0;
	draw_line(screen, blue,
		  (double[]){cx + r * cos(t1), cy + r * sin(t1)},
		  (double[]){cx + r * cos(t2), cy + r * sin(t2)});
    }
}

void SpinShip::create_bullet()
{
    double bw = BULLET_W, bh = BULLET_H;
    double cx = dims[0] + r;
    double cy = dims[1] + r;
    double dt = 360.0 / num_sides;
    for (int i = 0; i < num_sides; i++)
    {
	double t = (theta + i * dt) * M_PI / 180.0;
	double bx = cx + r * cos(t);
	double by = cy + r * sin(t);
	int c = 3;
	if (i % 2 == 0)
	    c = 4;
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, c);
	set_pair(bullet->v, 5 * cos(t), 5 * sin(t));
    }
}



LaserSpinShip::LaserSpinShip(Level *l, SpinShip *p)
    :Ship(l, "teleporter.png")
{
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = level->dims[3] + 100;
    set_pair(v, 0, -5);
    parent = p;
    health = 5;
    invincible = true;
    laser_count = 0;
    entered = false;
    theta = 0;
    items = false;
    dt = (int[]){-1, 1}[randint(0, 1)];
}

void LaserSpinShip::update()
{
    Ship::update();

    if (parent == NULL)
	return;

    theta = (theta + 360 + dt) % 360;

    if (!entered)
    {
	if (dims[1] < level->dims[3] - 175)
	{
	    dims[1] = level->dims[3] - 175;
	    set_pair(v, 0, 0);
	    entered = true;
	}
    }
    else if (laser_count < 40)
	laser_count += 0.5;

    if (entered && laser_count >= 40)
    {
	PlayerShip *player = level->player;
	double px = player->dims[0] + 0.5 * player->dims[2];
	double py = player->dims[1] + 0.5 * player->dims[3];
	double lx = dims[0] + 0.5 * dims[2];
	double ly = dims[1] + 0.5 * dims[3];
	for (int i = 0; i < 2; i++)
	{
	    double t = (theta + i * 90.0) * M_PI / 180.0;
	    double hx = px - lx;
	    double hy = py - ly;
	    double H = hx * cos(t) + hy * sin(t);
	    double tx = hx - H * cos(t);
	    double ty = hy - H * sin(t);
	    double mag = tx * tx + ty * ty;
	    if (mag + 15 <= laser_count * laser_count)
		player->damage(1, -1);
	}
    }
}

void LaserSpinShip::draw(Surface *screen)
{
    Ship::draw(screen);

    Uint32 purple = mapRGB(127, 0, 127);
    double lx = dims[0] + 0.5 * dims[2];
    double ly = dims[1] + 0.5 * dims[3];
    for (int i = 0; i < 4; i++)
    {
	double t = (theta + i * 90.0) * M_PI / 180.0;
	double ux = cos(t);
	double uy = sin(t);
	double lw = laser_count;
	draw_laser(screen, purple, (double[]){lx, ly},
		   (double[]){lx + 1000 * ux, ly + 1000 * uy},
		   (int)(lw));
    }
}
