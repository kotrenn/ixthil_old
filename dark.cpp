#include "dark.h"

DarkShip::DarkShip(Level *l, const char *filename)
	:Ship(l, filename)
{
	energy = 1;
}

void DarkShip::die()
{
	Ship::die();
	if (!level->subspace && energy > 0)
	{
		int a = 2, b = 5;
		if (level->player->subspace[2])
		{
			a = 3;
			b = 6;
		}
		int n = max(1, randint(a, b) / energy);
		if (level->player->subspace[2])
			n++;
		for (int i = 0; i < n; i++)
			new EnergyItem(level, dims[0], dims[1]);
	}
}



DarkSimpleShip::DarkSimpleShip(Level *l)
	:DarkShip(l, "darkenemy2.png")
{
	dims[0] = randint(0, level->dims[2] - image->get_width());
	dims[1] = -200;
	set_pair(v, 2 * randint(-2, 2), randint(2, 5));
	max_y = randint(50, 150);
	health = 2;
	color = 5;
	score = 150;
}

void DarkSimpleShip::update()
{
	DarkShip::update();

	if (randint(1, 70) == 1)
		create_bullet();
}

void DarkSimpleShip::move()
{
	DarkShip::move();

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
	if (dims[1] > max_y)
	{
		set_pair(v, 0, 0);
	}
}

void DarkSimpleShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet1 = new Bullet(level, bx - bw, by, bw, bh, 1, color);
	Bullet *bullet2 = new Bullet(level, bx + bw, by, bw, bh, 1, color);
	set_pair(bullet1->v, 0, 5);
	set_pair(bullet2->v, 0, 5);
}



BackAndForthShip::BackAndForthShip(Level *l)
	:DarkShip(l, "enemy4.png")
{
	double x = 0;
	while (x >= -0.5 * level->dims[2] && x <= 1.5 * level->dims[2])
		x = randint(-1 * level->dims[2], 2 * level->dims[2]);
	dims[0] = x;
	dims[1] = randint(50, 150);
	set_pair(v, 0, 0);
	while (fabs(v[0]) <= 1)
	{
		set_pair(v, randint(-6, 6), 0);
	}
	if (x < 0 && v[0] < 0)
		v[0] *= -1;
	if (x > 0 && v[0] > 0)
		v[0] *= -1;
	health = 6;
	shoot_timer = 0;
	shoot_delay = randint(15, 25);
	color = 1;
	score = 450;
}

void BackAndForthShip::update()
{
	DarkShip::update();

	if (shoot_timer == 0)
	{
		shoot_timer = shoot_delay;
		create_bullet();
	}
	else
		shoot_timer--;
}

void BackAndForthShip::move()
{
	DarkShip::move();

	if (dims[0] < 0 && v[0] < 0)
	{
		dims[0] = 0;
		v[0] *= -1;
	}
	if (dims[0] + dims[2] > level->dims[2] &&
	    v[0] > 0)
	{
		dims[0] = level->dims[2] - dims[2];
		v[0] *= -1;
	}
}

void BackAndForthShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet1 = new Bullet(level, bx - bw, by, bw, bh, 1, color);
	Bullet *bullet2 = new Bullet(level, bx + bw, by, bw, bh, 1, color);
	set_pair(bullet1->v, 0, 5);
	set_pair(bullet2->v, 0, 5);
}



DarkAttractorShip::DarkAttractorShip(Level *l)
	:DarkShip(l, "darkenemy1.png")
{
	dims[0] = randint(0, level->dims[2]);
	dims[1] = -5 - randint(100, 200);
	health = 10;
	axis = 50 + randint(-10, 10);
	v[0] = 0;
	while (v[0] == 0)
		v[0] = randint(-5, 5);
	v[1] = 0;
	a = randint(1, 2);
	max_v = 10;
	color = 3;
	score = 700;
}

void DarkAttractorShip::update()
{
	DarkShip::update();

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

	if (randint(1, 100) == 1)
		create_bullet();
}

void DarkAttractorShip::move()
{
	if (dims[1] < axis)
		v[1] += a;
	else if (dims[1] > axis)
		v[1] -= a;
	if (v[1] < -max_v)
		v[1] = -max_v;
	if (v[1] > max_v)
		v[1] = max_v;
	DarkShip::move();
}

void DarkAttractorShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	int s = randint(-1, 2);
	Bullet *bullet1 = new Bullet(level, bx, by, bw, bh, 1, color);
	Bullet *bullet2 = new Bullet(level, bx, by, bw, bh, 1, color);
	Bullet *bullet3 = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet1->v, -2 - s, 3 + s);
	set_pair(bullet2->v, 2 + s, 3 + s);
	set_pair(bullet3->v, 0, 5 + s);
}



DarkSmartShip::DarkSmartShip(Level *l)
	:DarkShip(l, "darkenemy3.png")
{
	dims[0] = randint(0, level->dims[2]);
	dims[1] = -1 * randint(150, 250);
	health = 15;
	max_y = randint(50, 150);
	set_pair(v, randint(-5, 5), randint(2, 6));
	speed = 0.5 * randint(2, 8);
	max_speed = randint(8, 16);
	color = 0;
	score = 1000;
}

void DarkSmartShip::update()
{
	DarkShip::update();

	if (dims[0] < 0)
	{
		dims[0] = 0;
		v[0] = 0;
	}
	if (dims[0] + dims[2] > level->dims[2])
	{
		dims[0] = level->dims[2] - dims[2];
		v[0] = 0;
	}
	if (v[0] < -max_speed)
		v[0] = -max_speed;
	if (v[0] > max_speed)
		v[0] = max_speed;
	if (dims[1] > max_y)
	{
		v[1] = 0;
		dims[1] = max_y;
	}

	if (randint(1, 50) == 1)
		create_bullet();
}

void DarkSmartShip::move()
{
	DarkShip::move();

	if (dims[0] < level->player->dims[0])
		v[0] += speed;
	if (dims[0] > level->player->dims[0])
		v[0] -= speed;
}

void DarkSmartShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	double s = randint(6, 10);
	Bullet *bullet1 = new Bullet(level, bx - bw, by, bw, bh, 1, color);
	Bullet *bullet2 = new Bullet(level, bx + bw, by, bw, bh, 1, color);
	set_pair(bullet1->v, 0, s);
	set_pair(bullet2->v, 0, s);
}



SplitShip::SplitShip(Level *l)
	:DarkShip(l, "bounce5.png")
{
	dims[0] = randint(0, level->dims[2] - 1);
	dims[1] = -1 * randint(200, 400);
	set_pair(v, 0, 0);
	while (v[0] == 0 || v[1] == 0)
	{
		set_pair(v, randint(-4, 4), randint(0, 4));
	}
	for (int i = 0; i < 2; i++)
		v[i] *= 2.5;
	in_level = false;
	size = 5;
	health = 12;
	color = 0;
	score = 125;
	energy = 0;
}

void SplitShip::die()
{
	if (size > 1)
	{
		for (int i = 0; i < 2; i++)
		{
			SplitShip *ss = new SplitShip(level);
			int s = size - 1;
			ss->size = s;
			ss->color = 5 - s;
			if (ss->size <= 2) ss->color++;
			ss->health = s * s;
			ss->health = max(1, ss->health / 2);
			char filename[80];
			snprintf(filename, 80, "bounce%d.png", s);
			ss->image = rm->get_image(filename);
			cpy_pair(ss->dims, dims);
			ss->score = ss->size * 25;
			if (ss->size <= 2)
				ss->items = false;
			if (ss->size == 1)
				ss->energy = 3;
		}
	}
	DarkShip::die();
}

void SplitShip::update()
{
	DarkShip::update();

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
    
	if (randint(1, 140) == 1)
		create_bullet();

	if (dims[1] >= 0)
		in_level = true;
}

void SplitShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, 10);
}



OctaGroupCreator::OctaGroupCreator(Level *l)
	:Actor(l, -50000, -50000, 1, 1)
{
	Surface *image = rm->get_image("octa_spin.png");
	double pos[] = {randint(0, level->dims[2] - 2 * image->get_width()),
	                -150};
	set_pair(v, 0, 0);
	while (v[0] == 0 || v[1] == 0)
	{
		set_pair(v, randint(-5, 5), randint(3, 5));
	}
	int dt = randint(3, 8);
	if (randint(0, 1) == 0)
		dt *= -1;
	list<OctaGroupShip *> *ship_list = new list<OctaGroupShip *>();
	for (int i = 0; i < 4; i++)
	{
		OctaGroupShip *g = new OctaGroupShip(level, pos, v, image->get_width(),
		                                     i * 90, dt);
		ship_list->push_back(g);
	}
	delete ship_list;
}

void OctaGroupCreator::update()
{
	level->remove_actor(this);
}



OctaGroupShip::OctaGroupShip(Level *l, double *c_pos, double *c_v,
                             double c_r, int c_t, int c_dt)
	:DarkShip(l, "octa_spin.png")
{
	r = c_r;
	cpy_pair(pos, c_pos);
	dims[0] = pos[0] + r * cos(t * M_PI / 180.0);
	dims[1] = pos[1] + r * sin(t * M_PI / 180.0);
	cpy_pair(v, c_v);
	t = c_t;
	dt = c_dt;
	health = 12;
	item_rate = 3;
	color = 5;
	energy = 2;
	score = 500;
	in_level = false;
}

void OctaGroupShip::update()
{
	DarkShip::update();

	t = (t + dt) % 360;

	if (randint(1, 150) == 1)
		create_bullet();
}

void OctaGroupShip::move()
{
	pos[0] += v[0];
	pos[1] += v[1];

	if (pos[1] >= 0)
		in_level = true;

	if (pos[0] < 0)
	{
		pos[0] = 0;
		v[0] *= -1;
	}
	if (pos[0] > level->dims[2])
	{
		pos[0] = level->dims[2];
		v[0] *= -1;
	}
	if (pos[1] < 0 && in_level)
	{
		pos[1] = 0;
		v[1] *= -1;
	}
	if (pos[1] > 0.5 * level->dims[3])
	{
		pos[1] = 0.5 * level->dims[3];
		v[1] *= -1;
	}

	dims[0] = pos[0] + r * cos(t * M_PI / 180.0);
	dims[1] = pos[1] + r * sin(t * M_PI / 180.0);
}

void OctaGroupShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, randint(5, 7));
}



BugShip::BugShip(Level *l)
	:DarkShip(l, "bug.png")
{
	dims[0] = randint(0, level->dims[2] - image->get_width());
	dims[1] = -200;
	s = 6.0;
	set_pair(v, 0, s);
	timer = 60;
	health = 15;
	in_level = false;
	color = 3;
	energy = 2;
	item_rate = 3;
	score = 200;
}

void BugShip::update()
{
	DarkShip::update();

	if (timer == 0)
	{
		timer = randint(5, 25);
		int n = randint(0, 7);
		if (n == 0) { set_pair(v, 0, -1 * s); }
		else if (n == 1) { set_pair(v, 0, s); }
		else if (n == 2) { set_pair(v, -1 * s, 0); }
		else if (n == 3) { set_pair(v, s, 0); }
		else if (n == 4) { set_pair(v, -1 * s, -1 * s); }
		else if (n == 5) { set_pair(v, -1 * s, s); }
		else if (n == 6) { set_pair(v, s, -1 * s); }
		else if (n == 7) { set_pair(v, s, s); }
	}
	else
		timer--;

	int k = 100;
	if (health < 50)
		k = 50;
	if (randint(0, k) == 0)
		create_bullet();
}

void BugShip::move()
{
	DarkShip::move();

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

void BugShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 2 * randint(-1, 1), randint(5, 10));
}



TriangleShip::TriangleShip(Level *l)
	:DarkShip(l, "snake.png")
{
	dims[0] = 0;
	dims[1] = 0;
	parent = NULL;
	child = NULL;
	color = randint(0, 5);
	health = 14;
	item_rate = 3;
	energy = 4;
	score = 300;
	pos = 7;
	side = 60.0;
	max_x = -1 + (level->dims[2] - 100) / side;
	max_y = (0.4 * level->dims[3]) / side;
	cx = randint(0, (int)(max_x));
	cy = randint(0, (int)(max_y));
	find_target();
	dims[0] = randint(-300, -100);
	if (randint(0, 1) == 0)
		dims[0] = level->dims[2] - dims[0];
	dims[1] = ty;
	v_timer = 30;
	reset_v();
}

void TriangleShip::reset_v()
{
	v[0] = tx - dims[0] - 0.5 * dims[2];
	v[1] = ty - dims[1] - 0.5 * dims[3];
	double mag = sqrt(v[0] * v[0] + v[1] * v[1]);
	double s = 4.0;
	set_pair(v, s * v[0] / mag, s * v[1] / mag);
}

void TriangleShip::find_target()
{
	int dx = 0;
	int dy = 0;
	bool done = parent != NULL;
	while (!done)
	{
		done = true;
		dx = dy = 0;
		int n = randint(0, 5);
		if (cy % 2 == 0)
		{
			if (n == 0 || n == 1 || n == 5) dx = -1;
			if (n == 3) dx = 1;
			if (n == 1 || n == 2) dy = -1;
			if (n == 4 || n == 5) dy = 1;
		}
		else
		{
			if (n == 0) dx = -1;
			if (n == 2 || n == 3 || n == 4) dx = 1;
			if (n == 1 || n == 2) dy = -1;
			if (n == 4 || n == 5) dy = 1;
		}
		if (cx == 0 && dx < 0) done = false;
		if (cy == 0 && dy < 0) done = false;
		if (cx >= max_x && dx > 0) done = false;
		if (cy >= max_y && dy > 0) done = false;
	}
	if (parent == NULL)
	{
		cx += dx;
		cy += dy;
		double bx = 50, by = 50;
		tx = bx + cx * side;
		ty = by + cy * side * 0.5 * sqrt(3.0);
		if (cy % 2 == 1)
			tx += 0.5 * side;
	}
	else
	{
		cx = parent->cx;
		cy = parent->cy;
		tx = parent->tx;
		ty = parent->ty;
	}
	reset_v();
}

void TriangleShip::die()
{
	if (parent != NULL)
	{
		parent->child = NULL;
		parent->pos = 0;
	}
	if (child != NULL)
		child->parent = NULL;
	DarkShip::die();
}

void TriangleShip::update()
{
	DarkShip::update();

	if (randint(1, 100) == 1 && parent == NULL)
		create_bullet();

	if (child == NULL && pos > 0)
	{
		child = new TriangleShip(level);
		child->parent = this;
		child->pos = pos - 1;
		child->tx = tx;
		child->ty = ty;
		child->color = color;
		cpy_pair(child->dims, dims);
		child->dims[0] -= dims[2] * v[0] / fabs(v[0]);
		child->reset_v();
	}

	double dx = dims[0] + 0.5 * dims[2] - tx;
	double dy = dims[1] + 0.5 * dims[3] - ty;
	if (dx * dx + dy * dy < 2 * 2)
		find_target();

	if (v_timer > 0)
		v_timer--;
	else
	{
		v_timer = 30;
		reset_v();
	}
}

void TriangleShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet0 = new Bullet(level, bx, by, bw, bh, 1, color);
	Bullet *bullet1 = new Bullet(level, bx, by, bw, bh, 1, color);
	Bullet *bullet2 = new Bullet(level, bx, by, bw, bh, 1, color);
	double s = 8.0;
	set_pair(bullet0->v, 0.5 * v[0], 0.5 * v[1] + s);
	set_pair(bullet1->v, 0.5 * v[0] - 4, 0.5 * v[1] + s - 4);
	set_pair(bullet2->v, 0.5 * v[0] + 4, 0.5 * v[1] + s - 4);
}



NetworkShip::NetworkShip(Level *l)
	:DarkShip(l, "yellow_circle.png")
{
	dims[0] = randint(0, level->dims[2] - image->get_width());
	dims[1] = -200;
	parent = NULL;
	has_child = false;
	n_b = randint(3, 6);
	b_theta = randint(0, 360);
	b_theta_v = randint(1, 6);
	if (randint(1, 2) == 1)
		b_theta_v *= -1;
	b_timer = 0;
	b_timer_max = randint(30, 50);
	health = 12;
	color = 1;
	pos = 2;
	item_rate = 3;
	energy = 2;
	score = 500;

	find_target();
}

void NetworkShip::find_target()
{
	double mag = 0;
	while (mag < 30 * 30 && (parent == NULL || mag > 120 * 120))
	{
		tx = randint(0.2 * level->dims[2], 0.8 * level->dims[2]);
		ty = randint(0.1 * level->dims[3], 0.6 * level->dims[3]);
		double dx = tx - dims[0];
		double dy = ty - dims[1];
		mag = dx * dx + dy * dy;
	}
	v[0] = tx - dims[0] - 0.5 * dims[2];
	v[1] = ty - dims[1] - 0.5 * dims[3];
	mag = sqrt(v[0] * v[0] + v[1] * v[1]);
	double s = 2.0;
	if (parent != NULL)
		s = 2.0;
	set_pair(v, s * v[0] / mag, s * v[1] / mag);
}

void NetworkShip::die()
{
	list<NetworkShip *>::iterator i;
	for (i = children.begin(); i != children.end(); i++)
		(*i)->parent = NULL;
	if (parent != NULL)
		parent->children.remove(this);
	DarkShip::die();
}

void NetworkShip::update()
{
	DarkShip::update();

	b_theta = (b_theta + b_theta_v) % 360;
	if (b_timer == 0)
	{
		create_bullet();
		b_timer = b_timer_max;
	}
	else
		b_timer--;
    
	/*
	  if (randint(1, 120) == 1)
	  create_bullet();
	*/

	if (parent != NULL)
	{
		for (int i = 0; i < 6; i++)
		{
			double t1 = randint(0, 100) / 100.0;
			double t2 = 1 - t1;
			double sx = dims[0] + 0.5 * dims[2];
			double sy = dims[1] + 0.5 * dims[3];
			double px = parent->dims[0] + 0.5 * parent->dims[2];
			double py = parent->dims[1] + 0.5 * parent->dims[3];
			double cx = t1 * sx + t2 * px;
			double cy = t1 * sy + t2 * py;
			/*
			  new ColorParticle(level, Bullet::get_color(color),
			  cx, cy, (double[]){0, 0}, 4, 0.2);
			*/
		}
	}
    
	if (!has_child)
	{
		double dx = dims[0] + 0.5 * dims[2] - tx;
		double dy = dims[1] + 0.5 * dims[3] - ty;
		double mag = dx * dx + dy * dy;
		if (mag < 5 * 5)
		{
			set_pair(v, 0, 0);
			if (pos > 0)
			{
				int n = randint(0, 2);
				for (int i = 0; i < n; i++)
				{
					NetworkShip *child = new NetworkShip(level);
					cpy_pair(child->dims, dims);
					child->parent = this;
					child->pos = pos - 1;
					child->n_b /= 2;
					child->find_target();
					children.push_back(child);
					has_child = true;
				}
			}
		}
	}
}

void NetworkShip::draw(Surface *screen)
{
	DarkShip::draw(screen);

	if (parent != NULL && false)
	{
		if (parent->health > 0)
		{
			double *pd = parent->dims;
			draw_line(screen, mapRGB(255, 255, 0),
			          (double[]){dims[0] + 0.5 * dims[2],
					          dims[1] + 0.5 * dims[3]},
			          (double[]){pd[0] + 0.5 * pd[2],
					          pd[1] + 0.5 * pd[3]});
		}
	}
}

void NetworkShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	for (int i = 0; i < n_b; i++)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		double s = 6;
		double t = ((b_theta + (360 / n_b) * i) % 360) * M_PI / 180.0;
		bullet->v[0] = s * cos(t);
		bullet->v[1] = s * sin(t);
	}
	/*
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
	  if (fabs(dist) > 0.0001)
	  {
	  bullet->v[0] = 6 * dx / dist;
	  bullet->v[1] = 6 * dy / dist;
	  }
	*/
}



VerticalShip::VerticalShip(Level *l)
	:DarkShip(l, "vertical.png")
{
	dims[0] = randint(20, 0.2 * level->dims[2]);
	dims[1] = level->dims[3] + 200;
	if (randint(0, 1) == 0)
		dims[0] = level->dims[2] - dims[0] - dims[2];
	health = 10;
	set_pair(v, 0, randint(2, 6));
	speed = 0.0625 * randint(2, 8);
	max_speed = randint(4, 12);
	color = 5;
	score = 1800;
}

void VerticalShip::update()
{
	DarkShip::update();

	if (dims[1] < 0 && v[1] < 0)
	{
		dims[1] = 0;
		v[1] = 0;
	}
	if (dims[1] + dims[3] > level->dims[3] &&
	    v[1] > 0)
	{
		dims[1] = level->dims[3] - dims[3];
		v[1] = 0;
	}
	if (v[1] < -max_speed)
		v[1] = -max_speed;
	if (v[1] > max_speed)
		v[1] = max_speed;

	if (randint(1, 90) == 1)
		create_bullet();
}

void VerticalShip::move()
{
	DarkShip::move();

	if (dims[1] < level->player->dims[1])
		v[1] += speed;
	if (dims[1] > level->player->dims[1])
		v[1] -= speed;
}

void VerticalShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, randint(3, 6), 0);
	if (level->player->dims[0] < dims[0])
		bullet->v[0] *= -1;
}
