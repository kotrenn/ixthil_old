#include "boss6.h"

PrismShip::PrismShip(Level *l)
	:Ship(l, "crystal_boss.png")
{
	dims[0] = 0.5 * (level->dims[2] - image->get_width());
	dims[1] = -500000;
	level->boss = this;
	health = 1;
	max_health = 1;
	healing = true;
	state = 0;
	max_state = 4;
	if (level->player->num_subspace >= 3)
		max_state = 5;
	if (level->player->num_subspace >= 6)
		max_state = 6;
	tx = 0;
	ty = 0;
	bomb_rad = -1;
	bomb_max = max(level->dims[2], level->dims[3]);
	num_diamond = 12;
	laser_timer = 360;
	switch_state();
}

double PrismShip::get_health()
{
	return (1.0 * health) / max_health;
}

void PrismShip::die()
{
	list<Ship *>::iterator s;
	for (s = summons.begin(); s != summons.end(); s++)
	{
		(*s)->parent = NULL;
		(*s)->invincible = false;
		(*s)->damage((*s)->health, -1);
	}
	summons.clear();

	if (state < max_state)
	{
		invincible = true;
		healing = true;
		bomb_rad = 0;
		state++;
		switch_state();
	}
	else
	{
		list<MirrorShip *>::iterator m;
		for (m = mirrors.begin(); m != mirrors.end(); m++)
		{
			(*m)->parent = NULL;
			(*m)->invincible = false;
			(*m)->damage((*m)->health, -1);
		}
		mirrors.clear();
		Ship::die();
	}
}

void PrismShip::switch_state()
{
	set_pair(v, 0, 0);

	if (state == 0)
	{
		max_health = 200;
		tx = 0.5 * level->dims[2];
		ty = 0.4 * level->dims[3];
	}
	else if (state == 1)
	{
		max_health = 200;
		tx = 0.5 * level->dims[2];
		ty = 0.1 * level->dims[3];
		double x1 = -100;
		double y1 = 0.5 * dims[3];
		double x2 = 100;
		double y2 = 0.5 * dims[3] + 200;
		Ship *b1 = new BulletMagnet(level, this, (double[]){x1, y1},
		                            (double[]){x1, y2});
		Ship *b2 = new BulletMagnet(level, this, (double[]){x2, y1},
		                            (double[]){x2, y2});
		summons.push_back(b1);
		summons.push_back(b2);
	}
	else if (state == 2)
	{
		max_health = 250;
		double x1 = 0;
		double y1 = 0.5 * dims[3] + 0.2 * level->dims[3];
		double x2 = -0.2 * level->dims[2];
		double y2 = 0.5 * dims[3] + 0.4 * level->dims[3];
		double x3 = 0.2 * level->dims[2];
		Ship *p1 = new PrismPlane(level, this, (double[]){x1, y1},
		                          (double[]){x2, y2});
		Ship *p2 = new PrismPlane(level, this, (double[]){x1, y1},
		                          (double[]){x3, y2});
		Ship *p3 = new PrismPlane(level, this, (double[]){x2, y2},
		                          (double[]){x3, y2});
		Ship *s1 = new CrystalSummoner(level, this, -200, 0, 4);
		Ship *s2 = new CrystalSummoner(level, this,  200, 0, 2);
		summons.push_back(p1);
		summons.push_back(p2);
		summons.push_back(p3);
		summons.push_back(s1);
		summons.push_back(s2);
	}
	else if (state == 3)
	{
		max_health = 300;
		MirrorShip *m1 = new MirrorShip(level, this);
		m1->dims[0] = -50;
		m1->dims[1] = 0.1 * level->dims[3];
		set_pair(m1->v, 3, 2);
		MirrorShip *m2 = new MirrorShip(level, this);
		m2->dims[0] = level->dims[2] + 50;
		m2->dims[1] = 0.1 * level->dims[3];
		set_pair(m2->v, -3, 2);
		mirrors.push_back(m1);
		mirrors.push_back(m2);
		double x1 = 0.2 * level->dims[2];
		double x2 = 0.5 * level->dims[2];
		double x3 = 0.8 * level->dims[2];
		double y1 = 0.25 * level->dims[3];
		double y2 = 0.35 * level->dims[3];
		double y3 = 0.45 * level->dims[3];
		double y4 = 0.55 * level->dims[3];
		Ship *f1 = new SlowField(level, this, (double[]){x2, y1},
		                         (double[]){x1, y2});
		Ship *f2 = new SlowField(level, this, (double[]){x1, y2},
		                         (double[]){x1, y3});
		Ship *f3 = new SlowField(level, this, (double[]){x1, y3},
		                         (double[]){x2, y4});
		Ship *f4 = new SlowField(level, this, (double[]){x2, y4},
		                         (double[]){x3, y3});
		Ship *f5 = new SlowField(level, this, (double[]){x3, y3},
		                         (double[]){x3, y2});
		Ship *f6 = new SlowField(level, this, (double[]){x3, y2},
		                         (double[]){x2, y1});
		summons.push_back(f1);
		summons.push_back(f2);
		summons.push_back(f3);
		summons.push_back(f4);
		summons.push_back(f5);
		summons.push_back(f6);
	}
	else if (state == 4)
	{
		max_health = 150;
		tx = 0.5 * level->dims[2];
		ty = 0.1 * level->dims[3];
		list<MirrorShip *>::iterator m;
		for (m = mirrors.begin(); m != mirrors.end();m ++)
			(*m)->shooting = true;
		summon_crystals();
	}
	else if (state == 5)
	{
		max_health = 200;
		tx = 0.5 * level->dims[2];
		ty = 0.1 * level->dims[3];
	}
	else if (state == 6)
	{
		max_health = 200;
		tx = 0.5 * level->dims[2];
		ty = 0.1 * level->dims[3];
		MirrorShip *m1 = new MirrorShip(level, this);
		m1->dims[0] = -50;
		m1->dims[1] = 0.1 * level->dims[3];
		set_pair(m1->v, 3, 4);
		MirrorShip *m2 = new MirrorShip(level, this);
		m2->dims[0] = level->dims[2] + 50;
		m2->dims[1] = 0.1 * level->dims[3];
		set_pair(m2->v, -3, 4);
		mirrors.push_back(m1);
		mirrors.push_back(m2);
		double x1 = 0;
		double y1 = level->dims[3];
		double x2 = level->dims[2];
		for (int i = 0; i < 2; i++)
		{
			BulletMagnet *b = new BulletMagnet(level, this,
			                                   (double[]){x1, y1},
			                                   (double[]){x2, y1});
			b->constant = true;
			summons.push_back(b);
		}
		list<MirrorShip *>::iterator m;
		for (m = mirrors.begin(); m != mirrors.end(); m++)
			(*m)->shooting = false;
	}
	if (level->player->num_subspace >= 3)
		max_health += 50;
	if (level->player->num_subspace >= 6)
		max_health += 50;
	invincible = true;
	health = 1;
}

void PrismShip::summon_crystals()
{
	double r1 = dims[2];
	double r2 = 0.5 * r1;
	for (int i = 0; i < num_diamond; i++)
	{
		Ship *d = new DiamondShip(level, this, r1, r2,
		                          i * (360 / num_diamond), 4);
		summons.push_back(d);
	}
}

void PrismShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + dims[3] - 0.5 * bh;
	PlayerShip *player = level->player;
	double px = player->dims[0] + 0.5 * player->dims[2];
	double py = player->dims[1] + 0.5 * player->dims[3];
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = px - sx;
	double dy = py - sy;
	double dist = sqrt(dx * dx + dy * dy);
	double aim[2] = {0, 0};
	if (fabs(dist) > 0.001)
	{
		aim[0] = 8 * dx / dist;
		aim[1] = 8 * dy / dist;
	}

	if (state == 0)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		set_pair(bullet->v, randint(-3, 3), randint(5, 8));
	}
	else if (state == 1)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		set_pair(bullet->v, randint(-3, 3), randint(5, 8));
	}
	else if (state == 2)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		set_pair(bullet->v, randint(-3, 3), randint(5, 8));
	}
	else if (state == 3)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		set_pair(bullet->v, randint(-8, 8), randint(5, 8));
	}
	else if (state == 4)
	{
		Bullet *bullet0 = new Bullet(level, bx, by, bw, bh, 1, color);
		cpy_pair(bullet0->v, aim);
		double cos_t = cos(20 * M_PI / 180.0);
		double sin_t = sin(20 * M_PI / 180.0);
		Bullet *bullet1 = new Bullet(level, bx, by, bw, bh, 1, color);
		bullet1->v[0] = aim[0] * cos_t - aim[1] * sin_t;
		bullet1->v[1] = aim[0] * sin_t + aim[1] * cos_t;
		Bullet *bullet2 = new Bullet(level, bx, by, bw, bh, 1, color);
		bullet2->v[0] = aim[0] * cos_t + aim[1] * sin_t;
		bullet2->v[1] = -1 * aim[0] * sin_t + aim[1] * cos_t;
	}
	else if (state == 5)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		cpy_pair(bullet->v, aim);
	}
	else if (state == 6)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		set_pair(bullet->v, randint(-3, 3), randint(5, 8));
	}
}

void PrismShip::summon_enemy()
{
	level->random_ships();
}

void PrismShip::move_to_target(double *dx_p, double *dy_p)
{
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	set_pair(v, 0, 0);
	if (fabs(dx) > 0.001) v[0] = 0.1 * dx;
	if (fabs(dy) > 0.001) v[1] = 0.1 * dy;
	if (dx_p != NULL) *dx_p = dx;
	if (dy_p != NULL) *dy_p = dy;
}

void PrismShip::update()
{
	Ship::update();

	PlayerShip *player = level->player;
	color = player->cur_color;

	if (bomb_rad > bomb_max)
		bomb_rad = -1;
	else if (bomb_rad >= 0)
	{
		double sx = dims[0] + 0.5 * dims[2];
		double sy = dims[1] + 0.5 * dims[3];
		int r = bomb_rad * bomb_rad;
		list<Bullet *>::iterator b;
		list<Bullet *> *bullets = level->get_bullets();
		for (b = bullets->begin(); b != bullets->end(); b++)
		{
			double bx = (*b)->dims[0] + 0.5 * (*b)->dims[2];
			double by = (*b)->dims[1] + 0.5 * (*b)->dims[3];
			double dx = sx - bx;
			double dy = sy - by;
			if (dx * dx + dy * dy <= r)
				level->remove_bullet(*b);
		}
		bomb_rad += 15;
	}

	if (invincible && healing)
	{
		if (health < max_health)
			health += 5;
		else
			invincible = false;
	}

	if (health > max_health)
		health = max_health;

	if (state == 0)
	{
		double dx, dy;
		move_to_target(&dx, &dy);
		if (fabs(dx) < 1 && fabs(dy) < 1)
		{
			int pos_x = randint(-3, 3);
			int pos_y = randint(-1, 1);
			tx = (0.5 + 0.1 * pos_x) * level->dims[2];
			ty = (0.3 + 0.1 * pos_y) * level->dims[3];
		}
		if (randint(1, 3) == 1)
			create_bullet();
	}
	else if (state == 1)
	{
		double dx, dy;
		move_to_target(&dx, &dy);
		if (fabs(dx) < 1 && fabs(dy) < 1)
		{
			int pos = randint(-3, 3);
			tx = (0.5 + 0.1 * pos) * level->dims[2];
			ty = (0.1 * level->dims[3]);
		}
		if (randint(1, 3) == 1)
			create_bullet();
	}
	else if (state == 2)
	{
		double dx, dy;
		move_to_target(&dx, &dy);
		if (fabs(dx) < 1 && fabs(dy) < 1)
		{
			int pos = randint(-3, 3);
			tx = (0.5 + 0.1 * pos) * level->dims[2];
			ty = 0.1 * level->dims[3];
		}
		if (randint(1, 8) == 1)
			create_bullet();
	}
	else if (state == 3)
	{
		double dx, dy;
		move_to_target(&dx, &dy);
		if (fabs(dx) < 1 && fabs(dy) < 1)
		{
			int pos = randint(-3, 3);
			tx = (0.5 + 0.1 * pos) * level->dims[2];
			ty = 0.1 * level->dims[3];
		}
		if (randint(1, 2) == 1)
			create_bullet();
	}
	else if (state == 4)
	{
		invincible = false;
		if (summons.size() > 0)
		{
			invincible = true;
			laser_timer = 240;
		}
		else if (randint(1, 4) == 1 && laser_timer >= 120)
			create_bullet();
		if (laser_timer >= 120)
		{
			double dx, dy;
			move_to_target(&dx, &dy);
			if (fabs(dx) < 1 && fabs(dy) < 1)
			{
				double t = randint(0, 3) * M_PI / 2.0;
				double cx = 0.5 * level->dims[2];
				double cy = 0.3 * level->dims[3];
				double r = 0.2 * level->dims[3];
				tx = cx + r * cos(t);
				ty = cy + r * sin(t);
			}
		}
		else
		{
			set_pair(v, 0, 0);
		}
		if (laser_timer > 30 && laser_timer < 120 &&
		    laser_timer % 5 == 0)
		{
			double sx = dims[0] + 0.5 * dims[2];
			double sy = dims[1] + dims[3];
			double theta = randint(1, 360) * M_PI / 180.0;
			double r = randint(30, 60);
			double px = sx + r * cos(theta);
			double py = sy + r * sin(theta);
			double vel[2];
			vel[0] = -0.2 * r * cos(theta);
			vel[1] = -0.2 * r * sin(theta);
			new TimedParticle(level, px, py, vel, 5);
		}

		if (laser_timer > -90)
			laser_timer--;
		else
		{
			healing = false;
			laser_timer = 240;
			summon_crystals();
		}
		if (laser_timer <= 0)
		{
			double lw = -2 * laser_timer;
			if (dims[2] < lw)
				lw = dims[2];
			double px = player->dims[0] + 0.5 * player->dims[2];
			double sx = dims[0] + 0.5 * dims[2];
			if (fabs(px - sx) <= lw)
				player->damage(1, -1);
		}
	}
	else if (state == 5)
	{
		double dx, dy;
		move_to_target(&dx, &dy);
		if (fabs(dx) < 1 && fabs(dy) < 1)
		{
			int pos = randint(-3, 3);
			tx = (0.5 + 0.1 * pos) * level->dims[2];
			ty = 0.1 * level->dims[3];
		}
		if (randint(1, 8) == 1)
			create_bullet();
		if (level->get_ships()->size() < 10)
			summon_enemy();
	}
	else if (state == 6)
	{
		double dx, dy;
		move_to_target(&dx, &dy);
		if (fabs(dx) < 1 && fabs(dy) < 1)
		{
			int pos = randint(-3, 3);
			tx = (0.5 + 0.1 * pos) * level->dims[2];
			ty = 0.1 * level->dims[3];
		}
		if (randint(1, 12) == 1)
			create_bullet();
	}
}

void PrismShip::draw(Surface *screen)
{
	Ship::draw(screen);

	Uint32 white = mapRGB(255, 255, 255);

	if (bomb_rad > 0)
	{
		double cx = dims[0] + 0.5 * dims[2];
		double cy = dims[1] + 0.5 * dims[3];
		draw_lightning_circle(screen, (double[]){cx, cy},
		                      white, bomb_rad);
	}

	if (state == 4)
	{
		double sx = dims[0] + 0.5 * dims[2];
		double sy = dims[1] + dims[3];
		double lw = -2 * laser_timer;
		if (dims[2] < lw)
			lw = dims[2];
		draw_laser(screen, white, (double[]){sx - 1, sy},
		           (double[]){sx - 1, sy + level->dims[3]}, lw);
	}
}



MirrorShip::MirrorShip(Level *l, PrismShip *p)
	:Ship(l, "crystal.png")
{
	parent = p;
	dims[0] = 0.5 * level->dims[2];
	dims[1] = 0.5 * level->dims[3];
	dims[2] = 1;
	dims[3] = 1;
	collisions = false;
	invincible = true;
	theta = 0;
	set_pair(n, 0, 0);
	double t = randint(0, 360) * M_PI / 180.0;
	set_pair(v, 3 * cos(t), 3 * sin(t));
	r = 30.0;
	health = 1;
	shooting = false;
	in_level = false;
}

void MirrorShip::update()
{
	if (parent == NULL)
		return;
    
	Ship::update();

	color = parent->color;

	double t = theta * M_PI / 180.0;
	set_pair(n, cos(t), sin(t));
	theta = (theta + 5) % 360;

	list<Bullet *> *bullets = level->get_bullets();
	list<Bullet *>::iterator b;
	double x[] = {dims[0], dims[1]};
	double u[] = {-n[1], n[0]};
	for (b = bullets->begin(); b != bullets->end(); b++)
	{
		double c[] = {(*b)->dims[0], (*b)->dims[1]};
		double R = 0.5 * ((*b)->dims[2] + (*b)->dims[3]);
		double vel[] = {x[0] - c[0], x[1] - c[1]};
		double c1 = vel[0] * u[0] + vel[1] * u[1];
		double c2 = vel[0] * vel[0] + vel[1] * vel[1] - R * R;
		double disc = c1 * c1 - c2;
		if (disc > 0)
		{
			double k = min(-1.0 * c1 + sqrt(disc),
			               -1.0 * c1 - sqrt(disc));
			if (fabs(k) <= r)
			{
				double dot = 2 * ((*b)->v[0] * n[0] + (*b)->v[1] * n[1]);
				(*b)->v[0] = (*b)->v[0] - dot * n[0];
				(*b)->v[1] = (*b)->v[1] - dot * n[1];
				(*b)->team = team;
				(*b)->homing = false;

			}
		}
	}

	if (theta % 120 == 0 && shooting)
		create_bullet();
}

void MirrorShip::move()
{
	Ship::move();

	if (!in_level)
		if (!in_level)
			if (dims[0] >= 0 || dims[1] >= 0 ||
			    dims[0] <= level->dims[2] ||
			    dims[1] <= level->dims[3])
				in_level = true;

	if (dims[0] < 0 && in_level)
	{
		dims[0] = 0;
		v[0] *= -1;
	}
	if (dims[0] > level->dims[2] && in_level)
	{
		dims[0] = level->dims[2];
		v[0] *= -1;
	}
	if (dims[1] < 0 && in_level)
	{
		dims[1] = 0;
		v[1] *= -1;
	}
	if (dims[1] > 0.6 * level->dims[3] && in_level)
	{
		dims[1] = 0.6 * level->dims[3];
		v[1] *= -1;
	}
}

void MirrorShip::draw(Surface *screen)
{
	Uint32 white = mapRGB(255, 255, 255);
	double v1[] = {-n[1], n[0]};
	double v2[] = {n[1], -n[0]};
	double p1[] = {dims[0] + r * v1[0], dims[1] + r * v1[1]};
	double p2[] = {dims[0] + r * v2[0], dims[1] + r * v2[1]};
	draw_line(screen, white, p1, p2);
}

void MirrorShip::create_bullet()
{
	if (parent == NULL)
		return;

	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new SpecialBullet(level, bx, by, bw, bh, 1,
	                                   color);
	set_pair(bullet->v, 6 * n[0], 6 * n[1]);
}



PrismPlane::PrismPlane(Level *l, PrismShip *p,
                       double *c_p1, double *c_p2)
	:Ship(l, "crystal.png")
{
	parent = p;
	cpy_pair(p1_off, c_p1);
	cpy_pair(p2_off, c_p2);
	set_pair(p1, p1_off[0] + parent->dims[0], p1_off[1] + parent->dims[1]);
	set_pair(p2, p2_off[0] + parent->dims[0], p2_off[1] + parent->dims[1]);
	dims[0] = 0.5 * (p1[0] + p2[0]);
	dims[1] = 0.5 * (p1[1] + p2[1]);
	dims[2] = 1;
	dims[3] = 1;
	collisions = false;
	invincible = true;
	set_pair(v, 0, 0);
	double dx = p1[0] - p2[0];
	double dy = p1[1] - p2[1];
	r = 0.5 * sqrt(dx * dx + dy * dy);
	health = 1;
}

void PrismPlane::update()
{
	Ship::update();

	if (parent == NULL)
		return;

	color = parent->color;

	dims[0] = 0.5 * (p1[0] + p2[0]);
	dims[1] = 0.5 * (p1[1] + p2[1]);
	double dx = p1[0] - p2[0];
	double dy = p1[1] - p2[1];
	r = 0.5 * sqrt(dx * dx + dy * dy);
	n[0] = p1[0] - dims[0];
	n[1] = p1[1] - dims[1];
	double temp = n[0];
	n[0] = -1 * n[1];
	n[1] = temp;
	double mag = sqrt(n[0] * n[0] + n[1] * n[1]);
	if (fabs(mag) > 0.001)
	{
		n[0] /= mag;
		n[1] /= mag;
	}

	list<Bullet *> *bullets = level->get_bullets();
	list<Bullet *>::iterator b;
	double p[] = {dims[0], dims[1]};
	double u[] = {-n[1], n[0]};
	for (b = bullets->begin(); b != bullets->end(); b++)
	{
		double dot = n[0] * (*b)->v[0] + n[1] * (*b)->v[1];
		mag = sqrt((*b)->v[0] * (*b)->v[0] + (*b)->v[1] * (*b)->v[1]);
		if (fabs(mag) > 0.001) dot = dot / mag;
		if (fabs(dot) > 0.15 && (*b)->refract_timer == 0)
		{
			double x[] = {(*b)->dims[0], (*b)->dims[1]};
			double proj = (x[0] - p[0]) * u[0] + (x[1] - p[1]) * u[1];
			double h[] = {p[0] + proj * u[0], p[1] + proj * u[1]};
			double vel[] = {x[0] - h[0], x[1] - h[1]};
			double mag_v = sqrt(vel[0] * vel[0] + vel[1] * vel[1]);
			double R = 0.5 * ((*b)->dims[2] + (*b)->dims[3]);
			if (fabs(proj) < r && mag_v <= R)
			{
				level->remove_bullet(*b);
				double bw = BULLET_W, bh = BULLET_H;
				double bx = (*b)->dims[0] - 0.5 * bw;
				double by = (*b)->dims[1] - 0.5 * bh;
				double cos_t = cos(30 * M_PI / 180.0);
				double sin_t = sin(30 * M_PI / 180.0);
				Bullet *bullet1 = new Bullet(level, bx, by, bw, bh,
				                             (*b)->team,
				                             ((*b)->color + 1) % 6);
				bullet1->v[0] = (*b)->v[0] * cos_t - (*b)->v[1] * sin_t;
				bullet1->v[1] = (*b)->v[0] * sin_t + (*b)->v[1] * cos_t;
				bullet1->refract_timer = 5;
				bullet1->colorific = (*b)->colorific;
				if (bullets->size() < 200)
				{
					Bullet *bullet2 = new Bullet(level, bx, by, bw, bh,
					                             (*b)->team,
					                             ((*b)->color + 1) % 6);
					bullet2->v[0] = (*b)->v[0] * cos_t + (*b)->v[1] * sin_t;
					bullet2->v[1] = -1.0 * (*b)->v[0] * sin_t +
						(*b)->v[1] * cos_t;
					bullet2->refract_timer = 5;
					bullet2->colorific = (*b)->colorific;
				}
			}
		}
	}
}

void PrismPlane::move()
{
	if (parent == NULL)
		return;

	for (int i = 0; i < 2; i++)
	{
		p1[i] = parent->dims[i] + 0.5 * parent->dims[i + 2];
		p1[i] += parent->v[i] + p1_off[i];
		p2[i] = parent->dims[i] + 0.5 * parent->dims[i + 2];
		p2[i] += parent->v[i] + p2_off[i];
	}
}

void PrismPlane::draw(Surface *screen)
{
	draw_line(screen, mapRGB(255, 255, 255), p1, p2);
}



SlowField::SlowField(Level *l, PrismShip *p,
                     double *c_p1, double *c_p2)
	:Ship(l, "crystal.png")
{
	parent = p;
	cpy_pair(p1_off, c_p1);
	cpy_pair(p2_off, c_p2);
	set_pair(p1, p1_off[0], p1_off[1]);
	set_pair(p2, p2_off[0], p2_off[1]);
	dims[0] = 0.5 * (p1[0] + p2[0]);
	dims[1] = 0.5 * (p1[1] + p2[1]);
	dims[2] = 1;
	dims[3] = 1;
	collisions = false;
	invincible = true;
	set_pair(v, 0, 0);
	double dx = p1[0] - p2[0];
	double dy = p1[1] - p2[1];
	r = 0.5 * sqrt(dx * dx + dy * dy);
	health = 1;
}

void SlowField::update()
{
	Ship::update();

	if (parent == NULL)
		return;

	color = parent->color;

	dims[0] = 0.5 * (p1[0] + p2[0]);
	dims[1] = 0.5 * (p1[1] + p2[1]);
	double dx = p1[0] - p2[0];
	double dy = p1[1] - p2[1];
	r = 0.5 * sqrt(dx * dx + dy * dy);
	n[0] = p1[0] - dims[0];
	n[1] = p1[1] - dims[1];
	double temp = n[0];
	n[0] = -1 * n[1];
	n[1] = temp;
	double mag = sqrt(n[0] * n[0] + n[1] * n[1]);
	if (fabs(mag) > 0.001)
	{
		n[0] /= mag;
		n[1] /= mag;
	}
	list<Bullet *> *bullets = level->get_bullets();
	list<Bullet *>::iterator b;
	double p[] = {dims[0], dims[1]};
	double u[] = {-n[1], n[0]};
	for (b = bullets->begin(); b != bullets->end(); b++)
	{
		double dot = n[0] * (*b)->v[0] + n[1] * (*b)->v[1];
		mag = sqrt((*b)->v[0] * (*b)->v[0] + (*b)->v[1] * (*b)->v[1]);
		if (fabs(mag) > 0.001) dot = dot / mag;
		if (fabs(dot) > 0.15 && (*b)->refract_timer == 0)
		{
			double x[] = {(*b)->dims[0], (*b)->dims[1]};
			double proj = (x[0] - p[0]) * u[0] + (x[1] - p[1]) * u[1];
			double h[] = {p[0] + proj * u[0], p[1] + proj * u[1]};
			double vel[] = {x[0] - h[0], x[1] - h[1]};
			double mag_v = sqrt(vel[0] * vel[0] + vel[1] * vel[1]);
			double R = 0.5 * ((*b)->dims[2] + (*b)->dims[3]);
			if (fabs(proj) < r && mag_v <= R)
			{
				double s = 4.0;
				if (dot > 0)
					s = 2.0;
				(*b)->v[0] = (*b)->v[0] * s / mag;
				(*b)->v[1] = (*b)->v[1] * s / mag;
				(*b)->homing = false;
			}
		}
	}
}

void SlowField::move()
{
	if (parent == NULL)
		return;

	for (int i = 0; i < 2; i++)
	{
		p1[i] = p1_off[i];
		p2[i] = p2_off[i];
	}
}

void SlowField::draw(Surface *screen)
{
	draw_line(screen, mapRGB(255, 255, 255), p1, p2);
}



BulletMagnet::BulletMagnet(Level *l, PrismShip *p,
                           double *c_p1, double *c_p2)
	:Ship(l, "crystal.png")
{
	parent = p;
	cpy_pair(p1_off, c_p1);
	cpy_pair(p2_off, c_p2);
	cpy_pair(p1, p1_off);
	cpy_pair(p2, p2_off);
	dims[0] = 0.5 * (p1[0] + p2[0]);
	dims[1] = 0.5 * (p1[1] + p2[1]);
	dims[2] = 1;
	dims[3] = 1;
	constant = false;
	collisions = false;
	invincible = true;
	set_pair(v, 0, 0);
	double dx = p1[0] - p2[0];
	double dy = p1[1] - p2[1];
	r = 0.5 * sqrt(dx * dx + dy * dy);
	health = 1;
}

void BulletMagnet::update()
{
	Ship::update();

	if (parent == NULL)
		return;

	color = parent->color;

	double dx = p1[0] - p2[0];
	double dy = p1[1] - p2[1];
	r = 0.5 * sqrt(dx * dx + dy * dy);
	n[0] = p1[0] - dims[0];
	n[1] = p1[1] - dims[1];
	double temp = n[0];
	n[0] = -1 * n[1];
	n[1] = temp;
	double mag = sqrt(n[0] * n[0] + n[1] * n[1]);
	if (fabs(mag) > 0.001)
	{
		n[0] /= mag;
		n[1] /= mag;
	}
	dims[0] = 0.5 * (p1[0] + p2[0]);
	dims[1] = 0.5 * (p1[1] + p2[1]);

	list<Bullet *> *bullets = level->get_bullets();
	list<Bullet *>::iterator b;
	double p[] = {dims[0], dims[1]};
	double u[] = {-n[1], n[0]};
	for (b = bullets->begin(); b != bullets->end(); b++)
	{
		double x[] = {(*b)->dims[0], (*b)->dims[1]};
		double proj = (x[0] - p[0]) * u[0] + (x[1] - p[1]) * u[1];
		double h[] = {p[0] + proj * u[0], p[1] + proj * u[1]};
		double vel[] = {x[0] - h[0], x[1] - h[1]};
		double mag_v = sqrt(vel[0] * vel[0] + vel[1] * vel[1]);
		if (fabs(proj) <= r && mag_v > 5)
		{
			double dx = n[0] / (0.05 * mag_v);
			double dy = n[1] / (0.05 * mag_v);
			double dot = (x[0] - p[0]) * n[0] +
				(x[1] - p[1]) * n[1];
			if (dot > 0)
			{
				dx *= -1;
				dy *= -1;
			}
			(*b)->v[0] += dx;
			(*b)->v[1] += dy;
			(*b)->homing = false;
		}
	}
}

void BulletMagnet::move()
{
	if (parent == NULL || constant)
		return;

	for (int i = 0; i < 2; i++)
	{
		p1[i] = parent->dims[i] + 0.5 * parent->dims[i + 2];
		p1[i] += parent->v[i] + p1_off[i];
		p2[i] = parent->dims[i] + 0.5 * parent->dims[i + 2];
		p2[i] += parent->v[i] + p2_off[i];
	}
}

void BulletMagnet::draw(Surface *screen)
{
	draw_line(screen, mapRGB(255, 255, 255), p1, p2);
}



CrystalSummoner::CrystalSummoner(Level *l, PrismShip *p,
                                 double x, double y, int c)
	:Ship(l, "crystal_ball.png")
{
	offset[0] = x - 0.5 * image->get_width();
	offset[1] = y - 0.5 * image->get_height();
	parent = p;
	dims[0] = offset[0] + parent->dims[0];
	dims[1] = offset[1] + parent->dims[1];
	set_pair(v, 0, 0);
	invincible = true;
	health = 1;
	color_off = c;
	color = (parent->color + color_off) % 6;
}

void CrystalSummoner::update()
{
	if (parent == NULL)
		return;

	Ship::update();

	color = (parent->color + color_off) % 6;

	if (randint(1, 25) == 1)
		create_bullet();
}

void CrystalSummoner::move()
{
	if (parent == NULL)
		return;

	double px = parent->dims[0] + 0.5 * parent->dims[2];
	double py = parent->dims[1] + 0.5 * parent->dims[3];
	dims[0] = offset[0] + px + parent->v[0];
	dims[1] = offset[1] + py + parent->v[1];
}

void CrystalSummoner::create_bullet()
{
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
		bullet->v[0] = 5 * dx / dist;
		bullet->v[1] = 5 * dy / dist;
	}
}



DiamondShip::DiamondShip(Level *l, PrismShip *p, double c_r1,
                         double c_r2, int c_t, int c_dt)
	:Ship(l, "diamond.png")
{
	dims[0] = -1000;
	dims[1] = -1000;
	parent = p;
	r1 = 0;
	r2 = 0;
	r1_max = c_r1;
	r2_max = c_r2;
	t = c_t;
	dt = c_dt;
	health = 150 / parent->num_diamond;
	item_rate = 3;
}

void DiamondShip::die()
{
	Ship::die();
    
	parent->summons.remove(this);
}

void DiamondShip::update()
{
	Ship::update();

	if (parent == NULL)
		return;

	double step = 0.1;
	if (r1 >= r1_max)
		r1 = r1_max;
	else
		r1 += step * r1_max;
	if (r2 >= r2_max)
		r2 = r2_max;
	else
		r2 += step * r2_max;

	t = (t + dt) % 360;
    
	color = parent->color;

	if (randint(1, 2 * parent->num_diamond) == 1)
		create_bullet();
}

void DiamondShip::move()
{
	if (parent == NULL)
		return;

	double sx = parent->dims[0] + 0.5 * parent->dims[2];
	double sy = parent->dims[1] + parent->dims[3];
	double theta = t * M_PI / 180.0;
	dims[0] = sx + r1 * cos(theta) - 0.5 * dims[2];
	dims[1] = sy + r2 * sin(theta) - 0.5 * dims[3];
}

void DiamondShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1];
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, parent->color);
	set_pair(bullet->v, 0, 8);
}
