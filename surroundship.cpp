#include "surroundship.h"

SurroundManager::SurroundManager(Level *l)
    :Ship(l, "surroundmanager.png")
{
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = -200;
    set_pair(v, 0, 5);
    health = 100;
    score = 20000;
    level->boss = this;
    num_ships = 8;
    r = 200.0;
    guns.push_back(new SurroundGun(level, this, -100));
    guns.push_back(new SurroundGun(level, this, 100));
    fire_timer = 0;
    max_y = 0.5 * dims[3];
    flip_timer = 0;
}

double SurroundManager::get_health()
{
    return health / 100.0;
}

void SurroundManager::die()
{
    list<SurroundShip *>::iterator s;
    for (s = ships.begin(); s != ships.end(); s++)
    {
	(*s)->parent = NULL;
	(*s)->invincible = false;
	(*s)->damage((*s)->health, -1);
    }
    list<SurroundGun *>::iterator g;
    for (g = guns.begin(); g != guns.end(); g++)
    {
	(*g)->parent = NULL;
	(*g)->invincible = false;
	(*g)->damage((*g)->health, -1);
    }
    Ship::die();
}

void SurroundManager::update()
{
    Ship::update();

    if (health < 50 && ships.size() == 0)
	for (int i = 0; i < num_ships; i++)
	    ships.push_back(new SurroundShip(level, this, i));

    if (fire_timer <= 0)
    {
	create_bullet();
	fire_timer = 25;
    }
    else
	fire_timer--;

    if (flip_timer < 50)
	flip_timer++;
    if (randint(0, 100) == 0 && flip_timer == 50)
    {
	v[0] *= -1;
	flip_timer = 0;
    }

    if (dims[1] > max_y)
    {
	dims[1] = max_y;
	v[0] = 3;
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

    Ship::move();
}

void SurroundManager::move()
{
}

void SurroundManager::create_bullet()
{
    if (dims[1] < max_y)
	return;

    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * dims[2];
    double by = dims[1] + dims[3];
    Bullet *bullet1 = new Bullet(level, bx, by, bw, bh, 1, 2);
    Bullet *bullet2 = new Bullet(level, bx, by, bw, bh, 1, 2);
    set_pair(bullet1->v, -1, 3);
    set_pair(bullet2->v, 1, 3);
}



SurroundGun::SurroundGun(Level *l, SurroundManager *p, double x)
    :Ship(l, "orb_teal.png")
{
    parent = p;
    dims[0] = parent->dims[0] - x + 0.5 * image->get_width();
    dims[1] = parent->dims[1] + 0.5 * parent->dims[3];
    set_pair(v, 0, 0);
    health = 5;
    invincible = true;
    items = false;
    dx = x;
    fire_timer = 0;
}

void SurroundGun::update()
{
    Ship::update();

    if (fire_timer <= 0)
    {
	create_bullet();
	fire_timer = 4;
    }
    else
	fire_timer--;
}

void SurroundGun::move()
{
    if (parent == NULL)
	return;
    
    dims[0] = parent->dims[0] - dx + 0.5 * dims[2];
    dims[1] = parent->dims[1] + 0.5 * parent->dims[3];
    dims[1] -= 0.5 * dims[3];
}

void SurroundGun::create_bullet()
{
    if (parent == NULL)
	return;
    
    if (parent->dims[1] < parent->max_y)
	return;
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + (0.5 * dims[2] - bw);
    double by = dims[1] + dims[3];
    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, 1);
    set_pair(bullet->v, 0, 4);
}



SurroundShip::SurroundShip(Level *l, SurroundManager *p, int c_n)
    :Ship(l, "orb_teal.png")
{
    set_pair(v, 0, 0);
    parent = p;
    n = c_n;
    r = 600;
    PlayerShip *player = level->player;
    double px = player->dims[0] + 0.5 * player->dims[2];
    double py = player->dims[1] + 0.5 * player->dims[3];
    theta = n * (360.0 / parent->num_ships);
    double t = theta * M_PI / 180.0;
    dims[0] = px + r * cos(t);
    dims[1] = py + r * sin(t);
    health = 5;
    invincible = true;
    items = false;
    fire_timer = 0;
}

void SurroundShip::update()
{
    Ship::update();

    if (parent == NULL)
	return;
    
    if (r <= parent->r)
    {
	r = parent->r;
	theta += 3.0;
	if (theta >= 360.0)
	    theta -= 360.0;
    }
    else
	r -= 8.0;

    if (fire_timer <= 0)
    {
	create_bullet();
	fire_timer = 150;
    }
    else
	fire_timer--;
}

void SurroundShip::move()
{
    Ship::move();

    if (parent == NULL)
	return;

    PlayerShip *player = level->player;
    double px = player->dims[0] + 0.5 * player->dims[2];
    double py = player->dims[1] + 0.5 * player->dims[3];
    double t = theta * M_PI / 180.0;
    dims[0] = px - 0.5 * dims[2] + r * cos(t);
    dims[1] = py - 0.5 * dims[3] + r * sin(t);
}

void SurroundShip::create_bullet()
{
    if (parent == NULL)
	return;
    
    if (parent->dims[1] < parent->max_y)
	return;
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, 5);
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
	bullet->v[0] = 2.0 * dx / dist;
	bullet->v[1] = 2.0 * dy / dist;
    }
}
