#include "slideship.h"

SlideShip::SlideShip(Level *l)
    :Ship(l, "orb_yellow.png")
{
    dims[0] = 0.5 * level->dims[2] - 0.5 * image->get_width();
    dims[1] = -200;
    s = 8;
    set_pair(v, 0, s);
    timer = 30;
    health = 80;
    score = 12000;
    level->boss = this;
    in_level = false;
}

double SlideShip::get_health()
{
    return health / 80.0;
}

void SlideShip::update()
{
    Ship::update();

    if (timer == 0)
    {
	timer = randint(5, 25);
	int n = randint(0, 3);
	if      (n == 0) { set_pair(v, 0, -1 * s); }
	else if (n == 1) { set_pair(v, 0, s); }
	else if (n == 2) { set_pair(v, -1 * s, 0); }
	else if (n == 3) { set_pair(v, s, 0); }
    }
    else
	timer--;

    int k = 20;
    if (health < 40)
	k = 10;
    if (randint(0, k) == 0)
	create_bullet();
}

void SlideShip::move()
{
    Ship::move();

    if (dims[1] > 0)
	in_level = true;

    if (dims[0] < 0)
    {
	dims[0] = 0;
	v[0] *= -1;
    }
    if (dims[1] < 0 && in_level)
    {
	dims[1] = 0;
	v[1] *= -1;
    }
    if (dims[0] + dims[2] > level->dims[2])
    {
	dims[0] = level->dims[2] - dims[2];
	v[0] *= -1;
    }
    if (dims[1] > level->dims[3] - 200)
    {
	dims[1] = level->dims[3] - 200;
	v[1] *= -1;
    }
}

void SlideShip::create_bullet()
{
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    for (int i = 0; i < 5; i++)
    {
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, 5);
	set_pair(bullet->v, 2 * (i - 2), 6 - 2 * fabs(i - 2));
    }
}
