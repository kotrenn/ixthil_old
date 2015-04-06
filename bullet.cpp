#include "bullet.h"
#include "level.h"

Bullet::Bullet(Level *level, double x, double y, double w, double h,
               int t, int c)
	:Actor(level, x, y, w, h)
{
	team = t;
	color = c;
	level->add_bullet(this);
	dead = false;
	colorific = false;
	homing = false;
	PlayerShip *player = level->player;
	if (player->team == team)
		player->bullets++;
	refract_timer = 0;
}

Bullet::~Bullet()
{
}

Uint32 Bullet::get_color(int color)
{
	if (color == 0) return mapRGB(255, 0, 0);
	if (color == 1) return mapRGB(255, 255, 0);
	if (color == 2) return mapRGB(0, 255, 0);
	if (color == 3) return mapRGB(0, 255, 255);
	if (color == 4) return mapRGB(0, 0, 255);
	if (color == 5) return mapRGB(255, 0, 255);
	return mapRGB(255, 255, 255);
}

void Bullet::update()
{
	Actor::update();

	if (refract_timer > 0)
		refract_timer--;
}

void Bullet::draw(Surface *screen)
{
	if (colorific)
		color = (color + 1) % 6;

	draw_rect(screen, get_color(color), dims, 2);
}

void Bullet::die(Ship *source)
{
	dead = true;
}

void Bullet::ship_died(Ship *ship)
{
}



LightningBullet::LightningBullet(Level *l, double x, double y,
                                 double w, double h, int c, double r)
 :Bullet(l, x, y, w, h, 0, c)
{
	radius = r;
}

void LightningBullet::die(Ship *source)
{
	Bullet::die(source);

	double bx = source->dims[0] + 0.5 * source->dims[2];
	double by = source->dims[1] + 0.5 * source->dims[3];
	list<Ship *> *ships = level->get_ships();
	list<Ship *>::iterator s;
	for (s = ships->begin(); s != ships->end(); s++)
	{
		Ship *ship = *s;
		if (ship->team != team)
		{
			double sx = ship->dims[0] + 0.5 * ship->dims[2];
			double sy = ship->dims[1] + 0.5 * ship->dims[3];
			double dx = sx - bx;
			double dy = sy - by;
			if (dx * dx + dy * dy < radius * radius)
			{
				new Lightning(level, source, ship, 10, color);
				ship->damage(1, color);
			}
		}
	}
}



Lightning::Lightning(Level *l, Ship *a, Ship *b, int t, int c)
	:Bullet(l, 0, 0, 1, 1, 1, c)
{
	s1 = a;
	s2 = b;
	timer = t;
	ship_dead = false;
	set_pair(v, 0, 0);
	set_points();
}

void Lightning::set_points()
{
	p1[0] = s1->dims[0] + 0.5 * s1->dims[2];
	p1[1] = s1->dims[1] + 0.5 * s1->dims[3];
	p2[0] = s2->dims[0] + 0.5 * s2->dims[2];
	p2[1] = s2->dims[1] + 0.5 * s2->dims[3];
}

void Lightning::update()
{
	timer--;
	if (timer <= 0 && !dead)
		level->remove_bullet(this);
}

void Lightning::draw(Surface *screen)
{
	if (!ship_dead)
		set_points();
	draw_lightning(screen, p1, p2, get_color(color));
}

void Lightning::ship_died(Ship *ship)
{
	if (ship == s1 || ship == s2)
	{
		timer = 1;
		ship_dead = true;
	}
}



HomingBullet::HomingBullet(Level *l, double x, double y, double w,
                           double h, int c)
	:Bullet(l, x, y, w, h, 0, c)
{
	target = NULL;
	max_v = 10;
	homing = true;
}

void HomingBullet::update()
{
	Bullet::update();

	if (homing)
	{
		if (randint(1, 60) == 1)
			v[0] += randint(-1, 1);
		if (randint(1, 60) == 1)
			v[1] += randint(-1, 1);

		if (target != NULL)
			if (target->health <= 0)
				target = NULL;
		if (target == NULL)
		{
			int i = -1;
			list<Ship *> *ships = level->get_ships();
			list<Ship *>::iterator s;
			Ship *ship;
			if (ships->size() > 1 && level->player->health > 0)
			{
				do
				{
					i = randint(0, ships->size() - 1);
					s = ships->begin();
					for (int j = 0; j < i; j++)
						s++;
					ship = *s;
				} while (ship == level->player);
				target = ship;
			}
			else
				target = NULL;
		}
	}
}

void HomingBullet::move()
{
	if (homing && target != NULL)
	{
		double dx = target->dims[0] + 0.5 * target->dims[2];
		dx -= dims[0] + 0.5 * dims[2];
		double dy = target->dims[1] + 0.5 * target->dims[3];
		dy -= dims[1] + 0.5 * dims[3];
		if (fabs(dx) > 0.1) v[0] += 2 * fabs(dx) / dx;
		if (fabs(dy) > 0.1) v[1] += 2 * fabs(dy) / dy;
		for (int i = 0; i < 2; i++)
		{
			if (v[i] >  max_v) v[i] =  max_v;
			if (v[i] < -max_v) v[i] = -max_v;
		}
	}
	Bullet::move();
}

void HomingBullet::ship_died(Ship *ship)
{
	if (ship == target)
		target = NULL;
}

AccelBullet::AccelBullet(Level *l, double x, double y, double w, double h,
                         double a, int t, int c)
	:Bullet(l, x, y, w, h, t, c)
{
	acc = a;
	max_v = 10;
}

void AccelBullet::move()
{
	Bullet::move();

	if (acc < 0 && v[1] > -1 * max_v)
		v[1] += acc;
	if (acc > 0 && v[1] < max_v)
		v[1] += acc;
}



SpecialBullet::SpecialBullet(Level *l, double x, double y, double w,
                             double h, int t, int c)
	:Bullet(l, x, y, w, h, t, c)
{
	target = level->player;
	max_v = 10;
}

void SpecialBullet::ship_died(Ship *ship)
{
	if (ship == target)
		target = NULL;
}

void SpecialBullet::update()
{
	Bullet::update();

	if (target != NULL)
		if (target->health <= 0)
			target = NULL;
}

void SpecialBullet::move()
{
	if (target != NULL)
	{
		double sx = dims[0] + 0.5 * dims[2];
		double sy = dims[1] + 0.5 * dims[3];
		double tx = target->dims[0] + 0.5 * target->dims[2];
		double ty = target->dims[1] + 0.5 * target->dims[3];
		double dx = tx - sx;
		double dy = ty - sy;
		double mag = dx * dx + dy * dy;
		if (mag > 0)
		{
			mag = sqrt(mag);
			double s = 0.2;
			dx *= s / mag;
			dy *= s / mag;
			v[0] += dx;
			v[1] += dy;
			mag = v[0] * v[0] + v[1] * v[1];
			s = 12.0;
			if (mag > s * s)
			{
				mag = sqrt(mag);
				v[0] *= s / mag;
				v[1] *= s / mag;
			}
		}
	}
	Bullet::move();
}



BombBullet::BombBullet(Level *l, double x, double y, double w, double h,
                       int t, int c)
	:Bullet(l, x, y, w, h, t, c)
{
	max_y = randint(0.6 * level->dims[3], 0.9 * level->dims[3]);
	bomb_rad = 0;
}

void BombBullet::update()
{
	Bullet::update();

	if (dims[1] > max_y)
	{
		set_pair(v, 0, 0);
		bomb_rad += 3;
		if (bomb_rad > 50)
		{
			level->remove_bullet(this);
			die(NULL);
		}
		double sx = dims[0] + 0.5 * dims[2];
		double sy = dims[1] + 0.5 * dims[3];
		list<Ship *>::iterator s;
		list<Ship *> *ships = level->get_ships();
		for (s = ships->begin(); s != ships->end(); s++)
		{
			if ((*s)->team != team)
			{
				double tx = (*s)->dims[0] + 0.5 * (*s)->dims[2];
				double ty = (*s)->dims[1] + 0.5 * (*s)->dims[3];
				double dx = sx - tx;
				double dy = sy - ty;
				if (dx * dx + dy * dy <= bomb_rad * bomb_rad)
					(*s)->damage(1, color);
			}
		}
	}
}

void BombBullet::draw(Surface *screen)
{
	if (bomb_rad == 0)
	{
		Bullet::draw(screen);
		return;
	}

	double cx = dims[0] + 0.5 * dims[2];
	double cy = dims[1] + 0.5 * dims[3];
	draw_lightning_circle(screen, (double[]){ cx, cy },
	                      get_color(color), bomb_rad);
}



DecayBullet::DecayBullet(Level *l, double x, double y, double w, double h,
                         int t, int c)
	:Bullet(l, x, y, w, h, t, c)
{
	decay_time = randint(30, 60);
}

void DecayBullet::update()
{
	Bullet::update();

	if (decay_time > 0)
	{
		decay_time--;
		return;
	}

	level->remove_bullet(this);
	die(NULL);

	int t = randint(10, 45);
	int theta[] = { -t, t };
	for (int i = 0; i < 2; i++)
	{
		Bullet *bullet = new DecayBullet(level, dims[0], dims[1],
		                                 dims[2], dims[3], team,
		                                 color);
		double c = cos(theta[i] * M_PI / 180.0);
		double s = sin(theta[i] * M_PI / 180.0);
		bullet->v[0] = c * v[0] - s * v[1];
		bullet->v[1] = s * v[0] + c * v[1];
	}
	if (randint(1, 4) == 1)
	{
		Bullet *bullet = new DecayBullet(level, dims[0], dims[1],
		                                 dims[2], dims[3], team,
		                                 color);
		cpy_pair(bullet->v, v);
	}
}



SwerveBullet::SwerveBullet(Level *l, double x, double y, double w, double h,
                           int t, int c)
	:Bullet(l, x, y, w, h, t, c)
{
	axis = dims[0];
	v[0] = 0;
	while (v[0] == 0)
		v[0] = 4 * randint(-4, 4);
}

void SwerveBullet::update()
{
	Bullet::update();

	if (dims[0] > axis)
		v[0]--;
	if (dims[0] < axis)
		v[0]++;
}
