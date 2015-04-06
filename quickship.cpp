#include "quickship.h"

QuickShip::QuickShip(Level *l)
	:Ship(l, "teleporter.png")
{
	dims[0] = 0.5 * (level->dims[2] - image->get_rect()[2]);
	dims[1] = -200;
	set_pair(v, 0, 0);
	health = 40;
	tx = 0;
	ty = 0;
	move_timer = 0;
	level->boss = this;
	score = 10000;
	select_target();
}

double QuickShip::get_health()
{
	return health / 40.0;
}

void QuickShip::select_target()
{
	tx = randint(0.2 * level->dims[2], 0.8 * level->dims[2]);
	ty = randint(0.2 * level->dims[3], 0.6 * level->dims[3]);
}

void QuickShip::update()
{
	Ship::update();

	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (dx * dx + dy * dy < 5 * 5)
	{
		set_pair(v, 0, 0);
		if (move_timer <= 0)
		{
			move_timer = randint(30, 90);
			select_target();
		}
		else
			move_timer--;
	}
	else
	{
		if (fabs(dx) > 0.001) v[0] = 0.1 * dx;
		if (fabs(dy) > 0.001) v[1] = 0.1 * dy;
	}

	if (randint(1, 25) == 1)
		create_bullet();
}

void QuickShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	for (int i = 0; i < 3; i++)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, 0);
		set_pair(bullet->v, 2 * (i - 1), 5);
		if (i == 1) bullet->v[1] = 8;
	}
}
