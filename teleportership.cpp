#include "teleportership.h"

TeleporterShip::TeleporterShip(Level *l)
    :Ship(l, "eye.png")
{
    dims[0] = 0.5 * (dims[2] - image->get_width());
    dims[1] = -200;
    set_pair(v, 0, 0);
    health = 80;
    moving = 0;
    tele = 1;
    shooting = 2;
    num_state = 3;
    state = moving;
    tele_timer = 0;
    shoot_timer = 120;
    tx = 0;
    ty = 0;
    tn = -1;
    level->boss = this;
    select_target();
    score = 15000;
}

double TeleporterShip::get_health()
{
    return health / 80.0;
}

void TeleporterShip::select_target()
{
    if (tn == -1)
    {
	tx = 0.5 * (level->dims[2] - dims[2]);
	ty = 100;
    }
    else if (tn == 0)
    {
	tx = 0.2 * level->dims[2];
	ty = 0.2 * level->dims[2];
    }
    else if (tn == 1)
    {
	tx = 0.2 * level->dims[2];
	ty = 0.6 * level->dims[3];
    }
    else if (tn == 2)
    {
	tx = 0.8 * level->dims[2];
	ty = 0.6 * level->dims[3];
    }
    else if (tn == 3)
    {
	tx = 0.8 * level->dims[2];
	ty = 0.2 * level->dims[3];
    }
    if (randint(1, 6) == 1 && tn >= 0)
    {
	while (state == moving)
	{
	    state = randint(0, num_state - 1);
	    set_pair(v, 0, 0);
	}
    }
    tn = (tn + 1) % 4;
}

void TeleporterShip::teleport()
{
    if (tele_timer > 0)
	tele_timer--;
    else
    {
	dims[0] = randint(0.2 * level->dims[2],
			  0.8 * level->dims[2]);
	dims[1] = randint(0.2 * level->dims[3],
			  0.6 * level->dims[3]);
	tele_timer = 90;
	create_bullet();
	if (randint(1, 6) == 1)
	{
	    while (state == tele)
	    {
		state = randint(0, num_state - 1);
		set_pair(v, 0, 0);
	    }
	}
    }
}

void TeleporterShip::update()
{
    Ship::update();

    if (state == moving)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (fabs(dx) > 0.001) v[0] = 0.1 * dx;
	if (fabs(dy) > 0.001) v[1] = 0.1 * dy;
	double dist = dx * dx + dy * dy;
	if (randint(1, 20) == 1)
	    create_bullet();
	if (dist <= 10 * 10)
	    select_target();
    }
    else if (state == tele)
    {
	if (tele_timer == 0)
	{
	    teleport();
	    tele_timer = 30;
	}
	else
	{
	    tele_timer--;
	    if (randint(1, 30) == 1)
		create_bullet();
	}
    }
    else if (state == shooting)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (fabs(dx) > 0.001) v[0] += 1 * fabs(dx) / dx;
	if (fabs(dy) > 0.001) v[1] += 1 * fabs(dy) / dy;
	double max_v = 8;
	for (int i = 0; i < 2; i++)
	{
	    if (v[i] >  max_v) v[i] =  max_v;
	    if (v[i] < -max_v) v[i] = -max_v;
	    v[i] += randint(-2, 2);
	}
	double dist = dx * dx + dy * dy;
	if (dist <= 30 * 30)
	{
	    tx = randint(0.2 * level->dims[2],
			 0.8 * level->dims[2]);
	    ty = randint(0.2 * level->dims[3],
			 0.6 * level->dims[3]);
	}
	if (shoot_timer < 60 && shoot_timer % 5 == 0)
	    create_bullet();
	if (shoot_timer == 0)
	{
	    shoot_timer = 120;
	    if (randint(1, 2) == 1)
	    {
		while (state == shooting)
		{
		    state = randint(0, num_state - 1);
		    set_pair(v, 0, 0);
		}
	    }
	}
	else
	    shoot_timer--;
    }
}

void TeleporterShip::create_bullet()
{
    color = -1;
    if (state == moving)
	color = 1;
    else if (state == tele)
	color = 5;
    else if (state == shooting)
	color = 3;
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
    PlayerShip *player = level->player;
    double px = player->dims[0] + 0.5 * player->dims[2];
    double py = player->dims[1] + 0.5 * player->dims[3];
    double sx = dims[0] + 0.5 * dims[2];
    double sy = dims[1] + 0.5 * dims[3];
    double dx = px - sx;
    double dy = py - sy;
    double dist = sqrt(dx * dx + dy * dy);
    set_pair(bullet->v, 0, 0);
    if (fabs(dist) > 0.001)
    {
	bullet->v[0] = 8 * dx / dist;
	bullet->v[1] = 8 * dy / dist;
    }
}
