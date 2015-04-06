#include "enemies.h"
#include "dark.h"

SimpleShip::SimpleShip(Level *l)
	:Ship(l, "enemy2.png")
{
	dims[0] = randint(0, level->dims[2] - image->get_rect()[2]);
	dims[1] = -200;
	set_pair(v, 2 * randint(-2, 2), randint(2, 5));
	max_y = randint(50, 150);
	health = 1;
	color = 2;
	score = 100;
}

void SimpleShip::die()
{
	Ship::die();
	if (color_hit)
		new DarkSimpleShip(level);
}

void SimpleShip::update()
{
	Ship::update();

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
		v[0] = v[1] = 0;

	if (randint(1, 100) == 1)
		create_bullet();
}

void SimpleShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, 5);
}



StrongShip::StrongShip(Level *l)
	:Ship(l, "enemy6.png")
{
	dims[0] = randint(0, level->dims[2] - image->get_width());
	dims[1] = -200;
	set_pair(v, 0, randint(3, 8));
	while (fabs(v[0]) < 2)
		v[0] = randint(-4, 4);
	max_y = randint(50, 150);
	health = 3;
	color = 4;
	score = 300;
}

void StrongShip::die()
{
	Ship::die();
	if (color_hit)
		new BackAndForthShip(level);
}

void StrongShip::update()
{
	Ship::update();

	if (randint(1, 70) == 1)
		create_bullet();
}

void StrongShip::move()
{
	Ship::move();
    
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
		v[1] = 0;
}

void StrongShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, 5);
}



AttractorShip::AttractorShip(Level *l)
	:Ship(l, "enemy1.png")
{
	dims[0] = randint(0, level->dims[2]);
	dims[1] = -50 - randint(100, 200);
	health = 5;
	axis = 50 + randint(-10, 10);
	v[0] = 0;
	while (v[0] == 0)
		v[0] = randint(-5, 5);
	v[1] = 0;
	a = randint(1, 2);
	max_v = 10;
	color = 0;
	score = 500;
}

void AttractorShip::die()
{
	Ship::die();
	if (color_hit)
		new DarkAttractorShip(level);
}

void AttractorShip::update()
{
	Ship::update();
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
	if (randint(1, 50) == 1)
		create_bullet();
}

void AttractorShip::move()
{
	if (dims[1] < axis)
		v[1] += a;
	else if (dims[1] > axis)
		v[1] -= a;
	if (v[1] < -max_v)
		v[1] = -max_v;
	if (v[1] > max_v)
		v[1] = max_v;
	Ship::move();
}

void AttractorShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, 5);
}



SmartShip::SmartShip(Level *l)
	:Ship(l, "enemy3.png")
{
	dims[0] = randint(0, level->dims[2]);
	dims[1] = -1 * randint(150, 250);
	health = 10;
	max_y = randint(20, 100);
	set_pair(v, randint(-5, 5), randint(2, 6));
	speed = 0.5 * randint(2, 8);
	max_speed = randint(10, 22);
	color = 1;
	score = 750;
}

void SmartShip::die()
{
	Ship::die();
	if (color_hit)
		new DarkSmartShip(level);
}

void SmartShip::update()
{
	Ship::update();

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

void SmartShip::move()
{
	Ship::move();

	if (dims[0] < level->player->dims[0])
		v[0] += speed;
	if (dims[0] > level->player->dims[0])
		v[0] -= speed;
}

void SmartShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, randint(4, 8));
}



BounceShip::BounceShip(Level *l)
	:Ship(l, "bouncer.png")
{
	dims[0] = randint(0, level->dims[2] - 1);
	dims[1] = -1 * randint(200, 400);
	health = 20;
	set_pair(v, 0, 0);
	while (v[0] == 0 || v[1] == 0)
	{
		set_pair(v, randint(-4, 4), randint(0, 4));
	}
	for (int i = 0; i < 2; i++)
		v[i] *= 2.5;
	in_level = false;
	color = 3;
	score = 1000;
}

void BounceShip::die()
{
	Ship::die();
	if (color_hit)
		new SplitShip(level);
}

void BounceShip::update()
{
	Ship::update();

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
	if (randint(1, 60) == 1)
		create_bullet();

	if (dims[1] >= 0)
		in_level = true;
}

void BounceShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, randint(8, 12));
}



GroupCreator::GroupCreator(Level *l)
	:Actor(l, -50000, -50000, 1, 1)
{
	Surface *image = rm->get_image("spinner.png");
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
	list<GroupShip *> *ship_list = new list<GroupShip *>();
	for (int i = 0; i < 3; i++)
	{
		GroupShip *g = new GroupShip(level, pos, v, image->get_width(),
		                             i * 120, dt);
		ship_list->push_back(g);
	}
	list<GroupShip *>::iterator g;
	for (g = ship_list->begin(); g != ship_list->end(); g++)
		(*g)->ship_list = ship_list;
}

void GroupCreator::update()
{
	level->remove_actor(this);
}



GroupShip::GroupShip(Level *l, double *c_pos, double *c_v,
                     double c_r, int c_t, int c_dt)
	:Ship(l, "spinner.png")
{
	r = c_r;
	cpy_pair(pos, c_pos);
	dims[0] = pos[0] + r * cos(t * M_PI / 180.0);
	dims[1] = pos[1] + r * sin(t * M_PI / 180.0);
	cpy_pair(v, c_v);
	t = c_t;
	dt = c_dt;
	health = 10;
	item_rate = 2;
	color = 2;
	score = 450;
	in_level = false;
	ship_list = NULL;
	dead = false;
}

void GroupShip::die()
{
	if (dead)
		return;
    
	Ship::die();

	if (ship_list->size() == 1)
	{
		delete ship_list;
		if (color_hit)
			new OctaGroupCreator(level);
	}
	else
	{
		int count = 0;
		list<GroupShip *>::iterator g;
		for (g = ship_list->begin(); g != ship_list->end(); g++)
			if (*g == this)
				count++;
		if (count > 0)
			ship_list->remove(this);
	}

	dead = true;
}

void GroupShip::update()
{
	Ship::update();

	t = (t + dt) % 360;

	if (randint(1, 150) == 1)
		create_bullet();
}

void GroupShip::move()
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

void GroupShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, randint(5, 7));
}



CloudManager *CloudManager::instance = NULL;

CloudManager *CloudManager::get_instance()
{
	if (instance == NULL)
		instance = new CloudManager();
	return instance;
}

CloudManager::CloudManager()
{
}

void CloudManager::get_a(Ship *ship, double *dst)
{
	double a[] = {0, 0};
	list<CloudShip *>::iterator s;
	for (s = ships.begin(); s != ships.end(); s++)
	{
		if (*s != ship)
		{
			double dx = (*s)->dims[0] - ship->dims[0];
			double dy = (*s)->dims[1] - ship->dims[1];
			double dist = dx * dx + dy * dy;
			if (dist < 50 * 50 && dist > 0)
			{
				dist = sqrt(dist);
				dx *= 0.3 / dist;
				dy *= 0.3 / dist;
				a[0] += dx;
				a[1] += dy;
			}
		}
	}
	double mag = sqrt(a[0] * a[0] + a[1] * a[1]);
	if (mag > 0)
	{
		set_pair(a, a[0] / mag, a[1] / mag);
	}
	cpy_pair(dst, a);
}

void CloudManager::set_center(double *c)
{
	cpy_pair(center, c);
}

void CloudManager::get_center(double *dst)
{
	cpy_pair(dst, center);
}

void CloudManager::add_ship(CloudShip *ship)
{
	ships.push_back(ship);
}

void CloudManager::remove_ship(CloudShip *ship)
{
	int count = 0;
	list<CloudShip *>::iterator i;
	for (i = ships.begin(); i != ships.end(); i++)
		if (*i == ship)
			count++;
	if (count > 0)
		ships.remove(ship);
}



CloudShip::CloudShip(Level *l)
	:Ship(l, "cloud.png")
{
	cm = CloudManager::get_instance();
	double theta = randint(1, 360) * M_PI / 180.0;
	double r = 0.5 * randint(1, 40);
	double center[2];
	cm->get_center(center);
	dims[0] = r * cos(theta) + center[0];
	dims[1] = r * sin(theta) + center[1];
	health = 10;
	team = 1;
	set_pair(v, 0, 0);
	while (v[0] == 0 && v[1] == 0)
	{
		set_pair(v, 0.2 * randint(-10, 10), 0.2 * randint(2, 10));
	}
	in_level = false;
	color = 5;
	score = 150;
	cm->add_ship(this);
}

void CloudShip::die()
{
	Ship::die();
	if (color_hit)
		new BugShip(level);
	cm->remove_ship(this);
}

void CloudShip::update()
{
	Ship::update();
	cm->get_a(this, a);

	double mag = v[0] * v[0] + v[1] * v[1];
	double max_v = 8.0;
	if (mag > max_v * max_v)
	{
		mag = sqrt(mag);
		v[0] = max_v * v[0] / mag;
		v[1] = max_v * v[1] / mag;
	}

	if (randint(1, 100) == 1)
		create_bullet();
}

void CloudShip::move()
{
	for (int i = 0; i < 2; i++)
		v[i] += a[i];

	if (dims[1] >= 0)
		in_level = true;
	else if (v[1] < 0)
		v[1] *= -1;
	Ship::move();

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
	if (dims[1] + dims[3] > level->dims[3] - 200)
	{
		dims[1] = level->dims[3] - dims[3] - 200;
		v[1] *= -1;
	}
}

void CloudShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 2 * randint(-1, 1), randint(5, 10));
}



DizzyShip::DizzyShip(Level *l)
	:Ship(l, "teleporter.png")
{
	dims[0] = randint(0, level->dims[2] - image->get_width());
	dims[1] = randint(-300, -200);
	set_pair(v, 0, 0);
	health = 25;
	tx = ty = 0;
	color = randint(0, 5);
	score = 2000;
	select_target();
}

void DizzyShip::select_target()
{
	tx = randint(0.2 * level->dims[2], 0.8 * level->dims[2]);
	ty = randint(0.2 * level->dims[3], 0.6 * level->dims[3]);
}

void DizzyShip::die()
{
	Ship::die();
	if (color_hit)
		(new TriangleShip(level))->color = color;
}

void DizzyShip::update()
{
	Ship::update();

	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (fabs(dx) > 0.01) v[0] += 1 * fabs(dx) / dx;
	if (fabs(dy) > 0.01) v[1] += 1 * fabs(dy) / dy;
	double max_v = 8.0;
	for (int i = 0; i < 2; i++)
	{
		if (v[i] >  max_v) v[i] =  max_v;
		if (v[i] < -max_v) v[i] = -max_v;
		v[i] += randint(-2, 2);
	}
	double dist = dx * dx + dy * dy;
	if (dist <= 30 * 30)
		select_target();
	if (randint(1, 100) == 1)
		create_bullet();
}

void DizzyShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	for (int i = 0; i < 3; i++)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		set_pair(bullet->v, 2 * (i - 1), 5);
		if (i == 1) bullet->v[1] = 8;
	}
}



CrystalShip::CrystalShip(Level *l)
	:Ship(l, "crystal.png")
{
	double x = 0, y = 0;
	while (x >= -1 * level->dims[2] && x < 2 * level->dims[2] &&
	       y >= -1 * level->dims[3] && y < 2 * level->dims[3])
	{
		x = randint(-3 * level->dims[2], 4 * level->dims[2]);
		y = randint(-3 * level->dims[3], 1 * level->dims[3]);
	}
	set_pair(dims, x, y);
	set_pair(v, 0, 0);
	health = 25;
	tx = 0;
	ty = 0;
	color = 0;
	score = 2500;
	select_target();
}

void CrystalShip::select_target()
{
	tx = randint(0.1 * level->dims[2], 0.9 * level->dims[2]);
	ty = randint(0.1 * level->dims[3], 0.7 * level->dims[3]);
}

void CrystalShip::die()
{
	Ship::die();
	if (color_hit)
		new NetworkShip(level);
}

void CrystalShip::update()
{
	Ship::update();

	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (fabs(dx) > 0.01) v[0] = 0.075 * dx;
	if (fabs(dy) > 0.01) v[1] = 0.075 * dy;
	double dist = dx * dx + dy * dy;
	if (dist <= 10 * 10)
		select_target();

	if (randint(1, 70) == 1)
		create_bullet();

	double max_v = 10.0;
	double mag = v[0] * v[0] + v[1] * v[1];
	if (mag > max_v * max_v)
	{
		mag = sqrt(mag);
		v[0] = max_v * v[0] / mag;
		v[1] = max_v * v[1] / mag;
	}
}

void CrystalShip::create_bullet()
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
	if (fabs(dist) > 0.0001)
	{
		bullet->v[0] = 6 * dx / dist;
		bullet->v[1] = 6 * dy / dist;
	}
}



BottomShip::BottomShip(Level *l)
	:Ship(l, "enemy7.png")
{
	double x = 0;
	while (x >= -0.5 * level->dims[2] && x <= 1.5 * level->dims[2])
		x = randint(-1 * level->dims[2], 2 * level->dims[2]);
	double y = randint(level->dims[3] - 150, level->dims[3] - 50);
	set_pair(dims, x, y);
	set_pair(v, 0, 0);
	while (fabs(v[0]) <= 1)
	{
		set_pair(v, randint(-6, 6), 0);
	}
	if (x < 0 && v[0] < 0)
		v[0] *= -1;
	if (x > 0 && v[0] > 0)
		v[0] *= -1;
	health = 20;
	shoot_timer = 0;
	shoot_delay = randint(30, 45);
	color = 1;
	score = 1800;
}

void BottomShip::die()
{
	Ship::die();
	if (color_hit)
		new VerticalShip(level);
}

void BottomShip::update()
{
	Ship::update();

	if (shoot_timer == 0)
	{
		shoot_timer = shoot_delay;
		create_bullet();
	}
	else
		shoot_timer--;
}

void BottomShip::move()
{
	Ship::move();

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

void BottomShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet->v, 0, -4);
}



SpinnerShip::SpinnerShip(Level *l)
	:Ship(l, "player.png")
{
	dims[0] = randint(0, level->dims[2] - 1);
	dims[1] = -1 * randint(100, 200);
	dims[2] = dims[3] = 15;
	health = 25;
	set_pair(v, randint(-5, 5), randint(2, 6));
	int s = 0.5 * randint(6, 9);
	double mag = sqrt(v[0] * v[0] + v[1] * v[1]);
	v[0] = s * v[0] / mag;
	v[1] = s * v[1] / mag;
	in_level = false;
	color = 2;
	score = 1000;
	n_bullets = randint(2, 6);
	theta = randint(1, 360);
	theta_v = randint(7, 10);
	if (randint(1, 2) == 1)
		theta_v *= -1;
	shoot_timer = 0;
	shoot_max = randint(120, 180);
	for (int i = 0; i < 8; i++)
		colors[i] = mapRGB(0, 32 * i - 1, 0);
	frame = randint(0, 7);
}

void SpinnerShip::update()
{
	Ship::update();

	theta = (theta + theta_v) % 360;
	if (shoot_timer == 0)
		shoot_timer = shoot_max;
	else
	{
		shoot_timer--;
		if (shoot_timer < 20 && shoot_timer % 3 == 0)
			create_bullet();
	}

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

	if (randint(1, 100) == 1 && in_level)
		v[randint(0, 1)] *= -1;

	if (dims[1] >= 0)
		in_level = true;
}

void SpinnerShip::draw(Surface *screen)
{
	frame = (frame + 1) % 8;
	for (int x = 1; x < 16; x++)
	{
		for (int y = 1; y < 16; y++)
		{
			int d = abs(x - 8) + abs(y - 8);
			Uint32 c = colors[(d + frame) % 8];
			if (hit)
				c = invertRGB(c);
			if (d < 8)
				draw_point(screen, c, dims[0] + x, dims[1] + y);
		}
	}
}

void SpinnerShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	for (int i = 0; i < n_bullets; i++)
	{
		double dt = (360.0 / n_bullets) * M_PI / 180.0;
		double t = (theta + i * dt);
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		bullet->v[0] = 5 * cos(t);
		bullet->v[1] = 5 * sin(t);
	}
}



SneakShip::SneakShip(Level *l)
	:Ship(l, "player.png")
{
	dims[0] = randint(0, level->dims[2] - 1);
	dims[1] = -1 * randint(100, 200);
	dims[2] = 15;
	dims[3] = 31;
	set_pair(v, 0, randint(4, 8));
	if (randint(1, 2) == 1)
	{
		dims[1] = level->dims[3] - dims[1];
		v[1] *= -1;
	}
	health = 25;
	color = 4;
	score = 1000;
	shoot_timer = 0;
	shoot_max = randint(20, 30);
	shoot_v = randint(2, 5);
	for (int i = 0; i < 8; i++)
		colors[i] = mapRGB(0, 0, 32 * i - 1);
	frame = randint(0, 7);
}

void SneakShip::update()
{
	Ship::update();

	if (v[1] > 0)
	{
		if (dims[1] > level->dims[3] + 100)
		{
			v[1] *= -1;
			dims[0] = randint(0, level->dims[2]);
		}
	}
	else
	{
		if (dims[1] < -100)
		{
			v[1] *= -1;
			dims[0] = randint(0, level->dims[2]);
		}
	}

	if (shoot_timer == 0)
	{
		shoot_timer = shoot_max;
		create_bullet();
	}
	else
		shoot_timer--;
}

void SneakShip::draw(Surface *screen)
{
	frame = (frame + 1) % 8;
	for (int x = 1; x < 16; x++)
	{
		for (int y = 1; y < 32; y++)
		{
			int d = abs(x - 8) + abs(y - 16) / 2;
			Uint32 c = colors[(d + frame) % 8];
			if (hit)
				c = invertRGB(c);
			if (d < 8)
				draw_point(screen, c, dims[0] + x, dims[1] + y);
		}
	}
}

void SneakShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet1 = new Bullet(level, bx, by, bw, bh, 1, color);
	Bullet *bullet2 = new Bullet(level, bx, by, bw, bh, 1, color);
	set_pair(bullet1->v, -shoot_v, 0);
	set_pair(bullet2->v, shoot_v, 0);
}




HomerShip::HomerShip(Level *l)
	:Ship(l, "player.png")
{
	dims[0] = randint(0, level->dims[2] - 1);
	dims[1] = -1 * randint(100, 200);
	dims[2] = 15;
	dims[3] = 15;
	double t = (90 + randint(-45, 45)) * M_PI / 180.0;
	double s = 0.5 * randint(6, 10);
	set_pair(v, s * cos(t), s * sin(t));
	health = 25;
	color = 1;
	score = 1000;
	in_level = false;
	shoot_timer = 0;
	shoot_max = randint(80, 120);
	for (int i = 0; i < 8; i++)
		colors[i] = mapRGB(32 * (7 - i) - 1, 32 * (7 - i) - 1, 0);
	frame = randint(0, 7);
}

void HomerShip::update()
{
	Ship::update();

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

	if (dims[1] > 0)
		in_level = true;

	if (shoot_timer == 0)
	{
		shoot_timer = shoot_max;
		create_bullet();
	}
	else
		shoot_timer--;
}

void HomerShip::draw(Surface *screen)
{
	frame = (frame + 1) % 8;
	for (int x = 1; x < 16; x++)
	{
		for (int y = 1; y < 16; y++)
		{
			int d = max(abs(x - 8), abs(y - 8));
			Uint32 c = colors[(d + frame) % 8];
			if (hit)
				c = invertRGB(c);
			draw_point(screen, c, dims[0] + x, dims[1] + y);
		}
	}
}

void HomerShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet = new SpecialBullet(level, bx, by,
	                                   bw, bh, 1, color);
	set_pair(bullet->v, 0, 0);
}



DodgeShip::DodgeShip(Level *l)
	:Ship(l, "player.png")
{
	dims[0] = randint(0, level->dims[2] - 1);
	dims[1] = -1 * randint(100, 200);
	dims[2] = 15;
	dims[3] = 15;
	set_pair(v, 2 * randint(-2, 2), randint(4, 8));
	health = 25;
	color = 3;
	score = 1000;
	in_level = false;
	for (int i = 0; i < 8; i++)
		colors[i] = mapRGB(0, 32 * i - 1, 32 * i - 1);
	frame = randint(0, 7);
	speed = 0.25 * randint(32, 44);
	speed = randint(12, 16);
}

void DodgeShip::update()
{
	Ship::update();

	if (dims[1] > 100)
		in_level = true;

	if (randint(1, 300) == 1)
		create_bullet();

	if (!in_level)
		return;

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

	double ax = 0, ay = 0;
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	list<Bullet *> *bullets = level->get_bullets();
	list<Bullet *>::iterator b;
	for (b = bullets->begin(); b != bullets->end(); b++)
	{
		Bullet *bullet = *b;
		if (bullet->team != team)
		{
			double bx = bullet->dims[0] + 0.5 * bullet->dims[2];
			double by = bullet->dims[1] + 0.5 * bullet->dims[3];
			double dx = sx - bx;
			double dy = sy - by;
			double mag = sqrt(dx * dx + dy * dy);
			if (mag < 250)
			{
				dx *= 1 / mag;
				dy *= 1 / mag;
				mag = (250 - mag) / 250.0;
				mag = mag * mag * mag;
				ax += M_PI * dx * mag;
				ay += M_PI * dy * mag;
			}
		}
	}
	v[0] += ax;
	v[1] += ay;
	double mag = sqrt(v[0] * v[0] + v[1] * v[1]);
	if (mag > speed)
	{
		v[0] = speed * v[0] / mag;
		v[1] = speed * v[1] / mag;
	}
}

void DodgeShip::draw(Surface *screen)
{
	frame = (frame + 1) % 8;
	for (int x = 1; x < 16; x++)
	{
		for (int y = 1; y < 16; y++)
		{
			int d = abs(x - 8) + abs(y - 8);
			Uint32 c = colors[(d + frame) % 8];
			if (hit)
				c = invertRGB(c);
			if (d < 8)
				draw_point(screen, c, dims[0] + x, dims[1] + y);
		}
	}
}

void DodgeShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	PlayerShip *player = level->player;
	double px = player->dims[0] + 0.5 * player->dims[2];
	double py = player->dims[1] + 0.5 * player->dims[3];
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = px - sx;
	double dy = py - sy;
	double dist = sqrt(dx * dx + dy * dy);
	for (int i = 0; i < 5; i++)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		set_pair(bullet->v, 0, 0);
		if (fabs(dist) > 0.0001)
		{
			int s = 2 * (i + 1);
			bullet->v[0] = s * dx / dist;
			bullet->v[1] = s * dy / dist;
		}
	}
}



WaveShip::WaveShip(Level *l)
	:Ship(l, "player.png")
{
	double x = 0, y = 0;
	while (x >= -1 * level->dims[2] && x < 2 * level->dims[2] &&
	       y >= -1 * level->dims[3] && y < 2 * level->dims[3])
	{
		x = randint(-3 * level->dims[2], 4 * level->dims[2]);
		y = randint(-3 * level->dims[3], 1 * level->dims[3]);
	}
	set_pair(dims, x, y);
	dims[2] = dims[3] = 15;
	double t = (90 + randint(-45, 45)) * M_PI / 180.0;
	double s = 0.5 * randint(6, 10);
	set_pair(v, s * cos(t), s * sin(t));
	health = 25;
	color = 5;
	score = 1000;
	for (int i = 0; i < 8; i++)
		colors[i] = mapRGB(32 * i - 1, 0, 32 * i - 1);
	frame = randint(0, 7);
	shoot_max = randint(120, 180);
	shoot_timer = randint(0, shoot_max);
	select_target();
}

void WaveShip::select_target()
{
	tx = randint(0.1 * level->dims[2], 0.9 * level->dims[2]);
	ty = randint(0.1 * level->dims[3], 0.5 * level->dims[3]);
}

void WaveShip::update()
{
	Ship::update();

	if (shoot_timer == 0)
		shoot_timer = shoot_max;
	else
	{
		shoot_timer--;
		if (shoot_timer < 20 && shoot_timer % 8 == 0)
			create_bullet();
	}

	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (fabs(dx) > 0.01) v[0] = 0.075 * dx;
	if (fabs(dy) > 0.01) v[1] = 0.075 * dy;
	double dist = dx * dx + dy * dy;
	if (dist <= 10 * 10)
		select_target();

	double max_v = 10.0;
	double mag = v[0] * v[0] + v[1] * v[1];
	if (mag > max_v * max_v)
	{
		mag = sqrt(mag);
		v[0] = max_v * v[0] / mag;
		v[1] = max_v * v[1] / mag;
	}
}

void WaveShip::draw(Surface *screen)
{
	frame = (frame + 1) % 8;
	for (int x = 1; x < 16; x++)
	{
		for (int y = 1; y < 16; y++)
		{
			int d = abs(x - 8) + abs(y - 8);
			Uint32 c = colors[(d + frame) % 8];
			if (hit)
				c = invertRGB(c);
			if (d < 8)
				draw_point(screen, c, dims[0] + x, dims[1] + y);
		}
	}
}

void WaveShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	for (int theta = -90; theta <= 90; theta += 30)
	{
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		double t = (90 + theta) * M_PI / 180.0;
		bullet->v[0] = 5 * cos(t);
		bullet->v[1] = 5 * sin(t);
	}
}



AssassinShip::AssassinShip(Level *l)
	:Ship(l, "player.png")
{
	dims[0] = randint(0, level->dims[2]);
	dims[1] = -1 * randint(150, 250);
	health = 25;
	max_y = randint(20, 150);
	set_pair(v, randint(-5, 5), randint(2, 6));
	speed = 0.5 * randint(2, 8);
	max_speed = randint(10, 22);
	color = 0;
	score = 1000;
	shoot_max = randint(120, 180);
	shoot_timer = randint(0, shoot_max);
	cone = randint(10, 30);
	for (int i = 0; i < 8; i++)
		colors[i] = mapRGB(32 * (7 - i) - 1, 0, 0);
	frame = randint(0, 7);
}

void AssassinShip::update()
{
	Ship::update();

	if (shoot_timer == 0)
		shoot_timer = shoot_max;
	else
	{
		shoot_timer--;
		if (shoot_timer < 20 && shoot_timer % 6 == 0)
			create_bullet();
	}

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
}

void AssassinShip::move()
{
	Ship::move();

	if (dims[0] < level->player->dims[0])
		v[0] += speed;
	if (dims[0] > level->player->dims[0])
		v[0] -= speed;
}

void AssassinShip::draw(Surface *screen)
{
	frame = (frame + 1) % 8;
	for (int x = 1; x < 16; x++)
	{
		for (int y = 1; y < 16; y++)
		{
			int d = abs(x - 8) + abs(y - 8);
			Uint32 c = colors[(d + frame) % 8];
			if (hit)
				c = invertRGB(c);
			if (d < 8)
				draw_point(screen, c, dims[0] + x, dims[1] + y);
		}
	}
}

void AssassinShip::create_bullet()
{
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	PlayerShip *player = level->player;
	double px = player->dims[0] + 0.5 * player->dims[2];
	double py = player->dims[1] + 0.5 * player->dims[3];
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = px - sx;
	double dy = py - sy;
	double dist = sqrt(dx * dx + dy * dy);
	double vx = 0.1;
	double vy = 0.1;
	int theta[] = { -cone, 0, cone };
	if (fabs(dist) > 0.001)
	{
		vx = 6 * dx / dist;
		vy = 6 * dy / dist;
	}
	for (int i = 0; i < 3; i++)
	{
		double t = theta[i] * M_PI / 180.0;
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
		double c = cos(t);
		double s = sin(t);
		bullet->v[0] = vx * c - vy * s;
		bullet->v[1] = vx * s + vy * c;
	}
}



BlackHole::BlackHole(Level *l)
	:Ship(l, "player.png")
{
	dims[0] = randint(0, level->dims[2]);
	dims[1] = -1 * randint(150, 250);
	health = 50;
	set_pair(v, 0.5 * randint(-5, 5), 0.5 * randint(2, 6));
	color = randint(0, 5);
	score = 1000;
	in_level = false;
	theta = 60;
	if (randint(1, 2) == 1)
		theta *= -1;
}

void BlackHole::update()
{
	Ship::update();

	if (dims[1] > 100)
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

	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	list<Bullet *> *bullets = level->get_bullets();
	list<Bullet *>::iterator b;
	for (b = bullets->begin(); b != bullets->end(); b++)
	{
		Bullet *bullet = *b;
		if (bullet->team != team)
		{
			double bx = bullet->dims[0] + 0.5 * bullet->dims[2];
			double by = bullet->dims[1] + 0.5 * bullet->dims[3];
			double dx = sx - bx;
			double dy = sy - by;
			if (dx * dx + dy * dy < 100 * 100)
			{
				double c = cos(theta * M_PI / 180.0);
				double s = sin(theta * M_PI / 180.0);
				double r[2] = { dx, dy };
				double a[] = { c * r[0] + s * r[1], -s * r[0] + c * r[1] };
				double alpha = 0.25;
				if (r[0] * r[0] + r[1] * r[1] < 25 * 25)
					alpha = 0.6;
				bullet->v[0] = alpha * a[0];
				bullet->v[1] = alpha * a[1];
				bullet->homing = false;
			}
		}
	}

	if (randint(1, 100) == 1)
	{
		double bw = BULLET_W, bh = BULLET_H;
		double bx = dims[0] + 0.5 * (dims[2] - bw);
		double by = dims[1] + 0.5 * (dims[3] - bh);
		Bullet *bullet = new SwerveBullet(level, bx, by, bw, bh, 1, color);
		//bullet->v[0] = 0;
		bullet->v[1] = 5;
		Bullet *bullet2 = new SwerveBullet(level, bx, by, bw, bh, 1, color);
		bullet2->v[0] = bullet->v[0] * -1;
		bullet2->v[1] = bullet->v[1];
	}
}

void BlackHole::draw(Surface *screen)
{
}
