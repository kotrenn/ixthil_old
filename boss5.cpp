#include "boss5.h"

MysteryShip::MysteryShip(Level *l)
    :Ship(l, "ballos1.png")
{
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = 3 * level->dims[3];
    set_pair(v, 0, -10);
    health = 600;
    invincible = true;
    level->boss = this;
    score = 25000;
    max_y = 50;
    orbs.push_back((OrbShip *)(new RedOrbShip(level, this, 0)));
    orbs.push_back((OrbShip *)(new YellowOrbShip(level, this, 1)));
    orbs.push_back((OrbShip *)(new GreenOrbShip(level, this, 2)));
    orbs.push_back((OrbShip *)(new TealOrbShip(level, this, 3)));
    orb_theta = 90 * randint(0, 3);
    orb_theta_v = 0;
    state = 0;
    for (int i = 0; i < 6; i++)
	colors[i] = i;
    cur_color  = 0;
    fire_timer = 0;
    for (int i = 0; i < 5; i++)
    {
	double t = (i * (360 / 5) - 90) * M_PI / 180.0;
	double x = 100 * cos(t) + 0.5 * level->dims[2];
	double y = 100 * sin(t) + 100 + max_y;
	set_pair(thetas[i], x, y);
    }
    cur_theta = 0;
}

double MysteryShip::get_health()
{
    int h = 0;
    list<OrbShip *>::iterator orb;
    for (orb = orbs.begin(); orb != orbs.end(); orb++)
	h += (*orb)->health;
    if (h > 0)
    {
	if (state == 0)
	    return h / 200.0;
	else if (state == 1)
	    return h / 200.0;
    }
    return health / 600.0;
}

void MysteryShip::update()
{
    Ship::update();

    int h = 0;
    list<OrbShip *>::iterator orb;
    for (orb = orbs.begin(); orb != orbs.end(); orb++)
	h += (*orb)->health;
    if (h == 0 && state < 2)
    {
	state++;
	if (state == 1)
	{
	    BlueOrbShip *blue = new BlueOrbShip(level, this, -1);
	    PurpleOrbShip *purple = new PurpleOrbShip(level, this, -1);
	    blue->friendly = purple;
	    purple->friendly = blue;
	    orbs.push_back(blue);
	    orbs.push_back(purple);
	}
	if (state == 2)
	    invincible = false;
    }

    if (dims[1] <= max_y && invincible)
    {
	for (orb = orbs.begin(); orb != orbs.end(); orb++)
	    (*orb)->invincible = false;
	set_pair(v, 0, 0);
	dims[1] = max_y;
    }

    OrbShip *cur_orb = get_cur_orb();
    if (orb_theta % 90 == 0 && cur_orb != NULL)
	orb_theta_v = 0;
    for (orb = orbs.begin(); orb != orbs.end(); orb++)
	if (state == 0)
	    (*orb)->invincible = true;
    if (orb_theta_v == 0 && cur_orb != NULL)
    {
	cur_orb->invincible = dims[1] > max_y;
	if (cur_orb->returning)
	    if (cur_orb->has_returned() && !cur_orb->invincible)
		orb_theta_v = 5 * (int[]){-1, 1}[randint(0, 1)];
    }
    if (orb_theta_v == 0 && cur_orb == NULL)
	orb_theta_v = 5 * (int[]){-1, 1}[randint(0, 1)];

    orb_theta = (orb_theta + orb_theta_v + 360) % 360;

    if (!invincible)
    {
	if (fire_timer % 15 == 0)
	    create_bullet();
	if (fire_timer % 120 == 0 && health < 400)
	    create_ring();
	if (fire_timer % 2 == 0 && health < 200)
	    create_extra_bullet();
	fire_timer++;
	double tx = thetas[cur_theta][0];
	double ty = thetas[cur_theta][1];
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (fabs(dx) > 0.001) v[0] = 0.1 * dx;
	if (fabs(dy) > 0.001) v[1] = 0.1 * dy;
	double dist = dx * dx + dy * dy;
	if (dist <= 10 * 10)
	    cur_theta = randint(0, 4);
    }
}

void MysteryShip::move()
{
    Ship::move();

    double sx = dims[0] + 0.5 * dims[2];
    double sy = dims[1] + 0.5 * dims[3];
    list<OrbShip *>::iterator orb;
    for (orb = orbs.begin(); orb != orbs.end(); orb++)
    {
	int i = (*orb)->get_i();
	if ((*orb)->invincible || dims[1] > max_y)
	{
	    double theta = 90 * i + orb_theta;
	    theta *= M_PI / 180.0;
	    (*orb)->dims[0] = sx + 100 * cos(theta);
	    (*orb)->dims[1] = sy + 100 * sin(theta);
	    (*orb)->dims[0] -= 0.5 * (*orb)->dims[2];
	    (*orb)->dims[1] -= 0.5 * (*orb)->dims[3];
	}
    }
}

void MysteryShip::draw(Surface *screen)
{
    list<OrbShip *>::iterator orb;
    for (orb = orbs.begin(); orb != orbs.end(); orb++)
    {
	if ((*orb)->invincible && (*orb)->health > 0)
	{
	    double ox = (*orb)->dims[0] + 0.5 * (*orb)->dims[2];
	    double oy = (*orb)->dims[1] + 0.5 * (*orb)->dims[3];
	    draw_lightning_circle(screen, (double[]){ox, oy},
				  mapRGB(255, 255, 255), 50);
	}
    }
    if (state < 2)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	Uint32 color = mapRGB(0, 255, 255);
	if (state == 1)
	    color = mapRGB(255, 0, 0);
	draw_lightning_circle(screen, (double[]){sx, sy},
			      color, 100);
    }
    else
    {
	Uint32 color = mapRGB(255, 0, 0);
	for (int i = 0; i < 5; i++)
	{
	    double *p1 = thetas[i];
	    double *p2 = thetas[(i + 1) % 5];
	    draw_lightning(screen, p1, p2, color);
	    p2 = thetas[(i + 2) % 5];
	    draw_lightning(screen, p1, p2, color);
	}
    }
    
    Ship::draw(screen);
}

OrbShip *MysteryShip::get_cur_orb()
{
    if (orbs.size() == 0)
	return NULL;
    int k = (int)((orb_theta + 180) / 90);
    k = 3 - k % 4;
    list<OrbShip *>::iterator orb;
    for (orb = orbs.begin(); orb != orbs.end(); orb++)
	if ((*orb)->my_i == k)
	    return *orb;
    return NULL;
}

void MysteryShip::create_bullet()
{
    color = colors[cur_color];
    cur_color = (cur_color + 1) % 6;
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    int n = 24;
    int dt = randint(1, 360);
    for (int i = 0; i < n; i++)
    {
	double theta = (i * (360 / n) + dt) % 360;
	theta *= M_PI / 180.0;
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	bullet->v[0] = 6 * cos(theta);
	bullet->v[1] = 6 * sin(theta);
    }
}

void MysteryShip::create_ring()
{
    color = colors[randint(0, 5)];
    double bw = BULLET_W, bh = BULLET_H;
    double sx = dims[0] + 0.5 * (dims[2] - bw);
    double sy = dims[1] + 0.5 * (dims[3] - bh);
    PlayerShip *player = level->player;
    double px = player->dims[0] + 0.5 * player->dims[2];
    double py = player->dims[1] + 0.5 * player->dims[3];
    int n = 24;
    int dt = randint(1, 360);
    for (int i = 0; i < n; i++)
    {
	int theta = (i * (360 / n) + dt) % 360;
	double bx = sx + 100 * cos(theta * M_PI / 180.0);
	double by = sy + 100 * sin(theta * M_PI / 180.0);
	double dx = px - bx;
	double dy = py - by;
	double mag = dx * dx + dy * dy;
	if (mag > 0)
	{
	    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	    mag = sqrt(mag);
	    bullet->v[0] = 4 * dx / mag;
	    bullet->v[1] = 4 * dy / mag;
	}
    }
}

void MysteryShip::create_extra_bullet()
{
    color = colors[cur_color];
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    double theta = (6 * fire_timer) * M_PI / 180.0;
    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
    bullet->v[0] = 10 * cos(theta);
    bullet->v[1] = 10 * sin(theta);
}



OrbShip::OrbShip(Level *l, MysteryShip *p, int i, const char *filename)
    :Ship(l, filename)
{
    dims[0] = -500;
    dims[1] = -500;
    parent = p;
    my_i = i;
    health = 50;
    invincible = true;
    color = -1;
    returning = false;
    tx = 0;
    ty = 0;
    score = 5000;
}

void OrbShip::select_target()
{
    tx = randint(0.2 * level->dims[2], 0.8 * level->dims[2]);
    ty = randint(0.2 * level->dims[3], 0.6 * level->dims[3]);
    if (randint(1, 8) == 1 && parent->dims[1] == parent->max_y)
	request_return();
}

void OrbShip::request_return()
{
}

bool OrbShip::has_returned()
{
    if (health <= 0)
	return true;
    double dr = 10;
    double px = parent->dims[0] + 0.5 * parent->dims[2];
    double py = parent->dims[1] + 0.5 * parent->dims[3];
    double sx = dims[0] + 0.5 * dims[2];
    double sy = dims[1] + 0.5 * dims[3];
    double dx = sx - px;
    double dy = sy - py;
    double dist = dx * dx + dy * dy;
    double r = (100 - dr) * (100 - dr);
    double R = (100 + dr) * (100 + dr);
    if (dist >= r && dist <= R && fabs(dx) <= dr && dy > 0)
	return true;
    return false;
}

void OrbShip::damage(int pain, int c)
{
    if (c == color)
	Ship::damage(2 * pain, c);
    else
	Ship::damage(pain, c);
}

void OrbShip::die()
{
    parent->orbs.remove(this);
    
    Ship::die();
}

void OrbShip::update()
{
    Ship::update();

    if (invincible)
    {
	if (parent->dims[1] == parent->max_y)
	{
	    if (randint(1, 50) == 1)
		create_bullet();
	    set_pair(v, 0, 0);
	}
	returning = false;
    }
}

void OrbShip::create_bullet()
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
	bullet->v[0] = 6 * dx / dist;
	bullet->v[1] = 6 * dy / dist;
    }
}



RedOrbShip::RedOrbShip(Level *l, MysteryShip *p, int i)
    :OrbShip(l, p, i, "orb_red.png")
{
    color = 0;
    tx = 0;
    ty = 0;
    select_target();
}

void RedOrbShip::request_return()
{
    double px = parent->dims[0] + 0.5 * parent->dims[2];
    double py = parent->dims[1] + 0.5 * parent->dims[3];
    tx = px;
    ty = py + 100;
    returning = true;
}

void RedOrbShip::update()
{
    OrbShip::update();

    if (!invincible && parent->dims[1] == parent->max_y)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double dx = tx - sx;
	double dy = ty - sy;
	if (fabs(dx) > 0.001) v[0] = 0.1 * dx;
	if (fabs(dy) > 0.001) v[1] = 0.1 * dy;
	double dist = dx * dx + dy * dy;
	if (dist <= 10 * 10 && !returning)
	    select_target();

	if (randint(1, 15) == 1)
	    create_bullet();
    }
}

void RedOrbShip::create_bullet()
{
    if (invincible)
	OrbShip::create_bullet();
    else
    {
	double bw = BULLET_W, bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] + 0.5 * (dims[3] - bh);
	Bullet *bullet;
	for (int i = -2; i <= 2; i++)
	{
	    bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	    set_pair(bullet->v, 2 * i, 8 - 2 * fabs(i));
	}
    }
}



YellowOrbShip::YellowOrbShip(Level *l, MysteryShip *p, int i)
    :OrbShip(l, p, i, "orb_yellow.png")
{
    color = 1;
    tx = 0;
    ty = 0;
    shooting = 0;
    tele = 1;
    num_states = 2;
    state = shooting;
    shoot_timer = 80;
    tele_timer = 0;
    select_target();
}

void YellowOrbShip::request_return()
{
    double px = parent->dims[0] + 0.5 * parent->dims[2];
    double py = parent->dims[1] + 0.5 * parent->dims[3];
    tx = px;
    ty = py + 100;
    returning = true;
}

void YellowOrbShip::update()
{
    OrbShip::update();

    if (!invincible && parent->dims[1] == parent->max_y)
    {
	if (state == shooting)
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
	    if (dist <= 30 * 30 && !returning)
		select_target();

	    if (shoot_timer < 60 && shoot_timer % 5 == 0)
		create_bullet();
	    if (shoot_timer == 0)
	    {
		shoot_timer = 80;
		if (randint(1, 2) == 1)
		{
		    while (state == shooting)
		    {
			state = randint(0, num_states - 1);
			set_pair(v, 0, 0);
		    }
		}
	    }
	    else
		shoot_timer--;

	}
	else if (state == tele)
	{
	    if (tele_timer > 0)
	    {
		tele_timer--;
		if (randint(1, 30) == 1)
		    create_bullet();
	    }
	    else
	    {
		tele_timer = 30;
		select_target();
		dims[0] = tx - 0.5 * dims[2];
		dims[1] = ty - 0.5 * dims[3];
		create_bullet();
		if (randint(1, 4) == 1)
		{
		    while (state == tele)
		    {
			state = randint(0, num_states - 1);
			set_pair(v, 0, 0);
		    }
		}
	    }
	}
    }
}



GreenOrbShip::GreenOrbShip(Level *l, MysteryShip *p, int i)
    :OrbShip(l, p, i, "orb_green.png")
{
    color = 2;
    set_pair(v2, 0, 0);
    while (v2[0] == 0 || v2[1] == 0)
    {
	set_pair(v2, 3 * randint(-3, 3), 3 * randint(-3, 3));
    }
    cpy_pair(v, v2);
}

void GreenOrbShip::request_return()
{
    double px = parent->dims[0] + 0.5 * parent->dims[2];
    double py = parent->dims[1] + 0.5 * parent->dims[3] + 100;
    double sx = dims[0] + 0.5 * dims[2];
    double sy = dims[1] + 0.5 * dims[3];
    set_pair(v2, px - sx, py - sy);
    double mag = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    double s = 6.0;
    v2[0] *= s / mag;
    v2[1] *= s / mag;
    returning = true;
}

void GreenOrbShip::update()
{
    OrbShip::update();

    if (!invincible && parent->dims[1] == parent->max_y &&
	!returning)
	if (randint(1, 3) == 1)
	    create_bullet();
}

void GreenOrbShip::move()
{
    if (!invincible)
    {
	cpy_pair(v, v2);
	OrbShip::move();

	bool bounced = false;
	if (dims[0] < 0)
	{
	    dims[0] = 0;
	    v2[0] *= -1;
	    bounced = true;
	}
	if (dims[0] + dims[2] > level->dims[2])
	{
	    dims[0] = level->dims[2] - dims[2];
	    v2[0] *= -1;
	    bounced = true;
	}
	if (dims[1] < 0)
	{
	    dims[1] = 0;
	    v2[1] *= -1;
	    bounced = true;
	}
	if (dims[1] + dims[3] > 0.6 * level->dims[3])
	{
	    dims[1] = 0.6 * level->dims[3] - dims[3];
	    v2[1] *= -1;
	    bounced = true;
	}
	if (bounced && randint(1, 8) == 1)
	    request_return();
    }
    else
	OrbShip::move();
}

void GreenOrbShip::create_bullet()
{
    if (invincible)
	OrbShip::create_bullet();
    else
    {
	if (randint(1, 8) == 1)
	{
	    double bx = dims[0] + 0.5 * dims[2];
	    double by = dims[1];
	    new GreenLightning(level, bx, by);
	}
	else
	{
	    double bw = BULLET_W, bh = BULLET_H;
	    double bx = dims[0] + 0.5 * (dims[2] - bw);
	    double by = dims[1] + 0.5 * (dims[3] - bh);
	    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	    set_pair(bullet->v, 2 * randint(-3, 3), randint(4, 6));
	}
    }
}



GreenLightning::GreenLightning(Level *l, double x, double y)
    :Bullet(l, x, y, 1, 1, 1, 2)
{
    image = rm->get_image("lightning.png");
    dims[2] = image->get_width();
    dims[3] = image->get_height();
    set_pair(v, 0, 8);
}

void GreenLightning::draw(Surface *screen)
{
    double p1[] = {dims[0] + 0.5 * dims[2], dims[1]};
    double p2[] = {p1[0], p1[1] + dims[3]};
    Uint32 color = mapRGB(0, 255, 0);
    int radius = (int)(dims[2]);
    for (int i = 0; i < 3; i++)
	draw_lightning(screen, p1, p2, color, radius);
}



TealOrbShip::TealOrbShip(Level *l, MysteryShip *p, int i)
    :OrbShip(l, p, i, "orb_teal.png")
{
    color = 3;
    tx = 0;
    ty = 0;
    target_delay = 210;
    set_pair(v2, 0, 0);
    while (v2[0] == 0 || v2[1] == 0)
    {
	set_pair(v2, 3 * randint(-3, 3), 3 * randint(-3, 3));
    }
    set_pair(v, 0, 0);
    lx = 0;
    ly = 0;
    ux = 0;
    uy = 0;
    lw = 0;
}

void TealOrbShip::request_return()
{
    double px = parent->dims[0] + 0.5 * parent->dims[2];
    double py = parent->dims[1] + 0.5 * parent->dims[3] + 100;
    double sx = dims[0] + 0.5 * dims[2];
    double sy = dims[1] + 0.5 * dims[3];
    set_pair(v2, px - sx, py - sy);
    double mag = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
    double s = 1.0 * randint(3, 6);
    v2[0] *= s / mag;
    v2[1] *= s / mag;
    returning = true;
}

void TealOrbShip::create_bullet()
{
    if (invincible)
	OrbShip::create_bullet();
    else
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
	double vx = 0;
	double vy = 0;
	int theta[] = {-30, -15, 0, 15, 30};
	if (fabs(dist) > 0.001)
	{
	    vx = 6 * dx / dist;
	    vy = 6 * dy / dist;
	}
	for (int i = 0; i < 5; i++)
	{
	    double t = theta[i] * M_PI / 180.0;
	    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	    double c = cos(t);
	    double s = sin(t);
	    bullet->v[0] = vx * c - vy * s;
	    bullet->v[1] = vx * s + vy * c;
	}
    }
}

void TealOrbShip::update()
{
    OrbShip::update();

    if (!invincible && parent->dims[1] == parent->max_y &&
	!returning)
    {
	if (target_delay > 30)
	    if (randint(1, 15) == 1)
		create_bullet();

	PlayerShip *player = level->player;
	double px = player->dims[0] + 0.5 * player->dims[2];
	double py = player->dims[1] + 0.5 * player->dims[3];
	lx = dims[0] + 0.5 * dims[2];
	ly = dims[1] + dims[3];
	ux = tx - lx;
	uy = ty - ly;
	double mag = sqrt(ux * ux + uy * uy);
	ux *= 1 / mag;
	uy *= 1 / mag;
	if (target_delay == 210)
	{
	    set_pair(v2, 0, 0);
	    while (v2[0] == 0 || v2[1] == 0)
	    {
		set_pair(v2, 3 * randint(-3, 3), 3 * randint(-3, 3));
	    }
	}
	if (target_delay > 120)
	{
	    tx = -100;
	    ty = -100;
	}
	else if (target_delay > 30)
	{
	    tx = px;
	    ty = py;
	}
	else if (target_delay < 0)
	{
	    double hx = px - lx;
	    double hy = py - ly;
	    double H = hx * ux + hy * uy;
	    double tx0 = hx - H * ux;
	    double ty0 = hy - H * uy;
	    double mag = sqrt(tx0 * tx0 + ty0 * ty0);
	    if (mag <= lw)
		player->damage(1, -1);
	}
	target_delay--;
	if (target_delay <= -60)
	{
	    target_delay = 210;
	    request_return();
	}
    }
}

void TealOrbShip::move()
{
    if (target_delay > 30 && !invincible)
    {
	cpy_pair(v, v2);
	OrbShip::move();

	if (dims[0] < 0)
	{
	    dims[0] = 0;
	    v2[0] *= -1;
	}
	if (dims[0] + dims[2] > level->dims[2])
	{
	    dims[0] = level->dims[2] - dims[2];
	    v2[0] *= -1;
	}
	if (dims[1] < 0)
	{
	    dims[1] = 0;
	    v2[1] *= -1;
	}
	if (dims[1] + dims[3] > 0.6 * level->dims[3])
	{
	    dims[1] = 0.6 * level->dims[3] - dims[3];
	    v2[1] *= -1;
	}
    }
    else if (invincible)
	OrbShip::move();
}

void TealOrbShip::draw(Surface *screen)
{
    OrbShip::draw(screen);

    if (target_delay <= 120 && target_delay > 0)
    {
	Surface *target = rm->get_image("target.png");
	if (target_delay <= 30 && target_delay % 2 == 0)
	    target = rm->get_image("target_red.png");
	double x = tx - 0.5 * target->get_width();
	double y = ty - 0.5 * target->get_height();
	screen->blit(target, x, y);
    }
    else if (target_delay < 0)
    {
	lw = 40;
	if (target_delay > -20)
	    lw = -2 * target_delay;
	draw_laser(screen, mapRGB(255, 0, 0), (double[]){lx, ly},
		   (double[]){lx + 1000 * ux, ly + 1000 * uy}, lw);
    }
}



BlueOrbShip::BlueOrbShip(Level *l, MysteryShip *p, int i)
    :OrbShip(l, p, i, "orb_blue.png")
{
    dims[0] = 0.5 * (parent->dims[2] - image->get_width());
    dims[1] = 0.5 * (parent->dims[3] - image->get_height());
    dims[0] += parent->dims[0];
    dims[1] += parent->dims[1];
    health = 100;
    set_pair(v, -5, 0);
    color = 4;
    friendly = NULL;
    bt = 0;
    score *= 2;
}

void BlueOrbShip::die()
{
    if (friendly != NULL)
	friendly->friendly = NULL;
    OrbShip::die();
}

void BlueOrbShip::update()
{
    OrbShip::update();

    bt = (bt + 1) % 360;
    int n = 8;
    if (friendly == NULL)
	n = 4;
    if (bt % n == 0)
	create_bullet();
}

void BlueOrbShip::move()
{
    OrbShip::move();

    bool bounced = false;
    if (dims[0] < 0)
    {
	dims[0] = 0;
	v[0] *= -1;
	bounced = true;
    }
    if (dims[0] + dims[2] > level->dims[2])
    {
	dims[0] = level->dims[2] - dims[2];
	v[0] *= -1;
	bounced = true;
    }
    if (dims[1] < 0)
    {
	dims[1] = 0;
	v[1] *= -1;
	bounced = true;
    }
    if (dims[1] + dims[3] > 0.6 * level->dims[3])
    {
	dims[1] = 0.6 * level->dims[3] - dims[3];
	v[1] *= -1;
	bounced = true;
    }
    
    if (bounced)
    {
	int n = randint(0, 1);
	if (v[0] == 0) n = 0;
	int dv = randint(-1, 1);
	v[n] += dv;
	if (v[n] == 0)
	    v[n] -= dv;
	if (v[n] < -6)
	    v[n] = -6;
	if (v[n] > 6)
	    v[n] = 6;
    }
}

void BlueOrbShip::draw(Surface *screen)
{
    OrbShip::draw(screen);

    if (friendly != NULL)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double fx = friendly->dims[0] + 0.5 * friendly->dims[2];
	double fy = friendly->dims[1] + 0.5 * friendly->dims[3];
	double dx = sx - fx;
	double dy = sy - fy;
	if (dx * dx + dy * dy < 200 * 200)
	    draw_lightning(screen, (double[]){sx, sy},
			   (double[]){fx, fy}, mapRGB(0, 0, 255));
    }
}

void BlueOrbShip::create_bullet()
{
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    for (int i = 0; i < 4; i++)
    {
	double t = 90.0 * i * M_PI / 180.0 + bt;
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	bullet->v[0] = 5 * cos(t);
	bullet->v[1] = 5 * sin(t);
    }
}



PurpleOrbShip::PurpleOrbShip(Level *l, MysteryShip *p, int i)
    :OrbShip(l, p, i, "orb_purple.png")
{
    dims[0] = 0.5 * (parent->dims[2] - image->get_width());
    dims[1] = 0.5 * (parent->dims[3] - image->get_height());
    dims[0] += parent->dims[0];
    dims[1] += parent->dims[1];
    health = 100;
    set_pair(v, 5, 0);
    color = 5;
    friendly = NULL;
    bt = 0;
    score *= 2;
}

void PurpleOrbShip::die()
{
    if (friendly != NULL)
	friendly->friendly = NULL;
    OrbShip::die();
}

void PurpleOrbShip::update()
{
    OrbShip::update();

    if (bt == 0)
	create_bullet();
    int n = 14;
    if (friendly == NULL)
	n = 7;
    bt = (bt + 1) % n;
}

void PurpleOrbShip::move()
{
    OrbShip::move();

    bool bounced = false;
    if (dims[0] < 0)
    {
	dims[0] = 0;
	v[0] *= -1;
	bounced = true;
    }
    if (dims[0] + dims[2] > level->dims[2])
    {
	dims[0] = level->dims[2] - dims[2];
	v[0] *= -1;
	bounced = true;
    }
    if (dims[1] < 0)
    {
	dims[1] = 0;
	v[1] *= -1;
	bounced = true;
    }
    if (dims[1] + dims[3] > 0.6 * level->dims[3])
    {
	dims[1] = 0.6 * level->dims[3] - dims[3];
	v[1] *= -1;
	bounced = true;
    }
    
    if (bounced)
    {
	int n = randint(0, 1);
	if (v[0] == 0) n = 0;
	int dv = randint(-1, 1);
	v[n] += dv;
	if (v[n] == 0)
	    v[n] -= dv;
	if (v[n] < -6)
	    v[n] = -6;
	if (v[n] > 6)
	    v[n] = 6;
    }
}

void PurpleOrbShip::draw(Surface *screen)
{
    OrbShip::draw(screen);

    if (friendly != NULL)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + 0.5 * dims[3];
	double fx = friendly->dims[0] + 0.5 * friendly->dims[2];
	double fy = friendly->dims[1] + 0.5 * friendly->dims[3];
	double dx = sx - fx;
	double dy = sy - fy;
	if (dx * dx + dy * dy < 200 * 200)
	    draw_lightning(screen, (double[]){sx, sy},
			   (double[]){fx, fy}, mapRGB(255, 0, 255));
    }
}

void PurpleOrbShip::create_bullet()
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
    int theta[] = {-30, -15, 0, 15, 30};
    if (fabs(dist) > 0.001)
    {
	vx = 6 * dx / dist;
	vy = 6 * dy / dist;
    }
    for (int i = 0; i < 5; i++)
    {
	double t = theta[i] * M_PI / 180.0;
	Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
	double c = cos(t);
	double s = sin(t);
	bullet->v[0] = vx * c - vy * s;
	bullet->v[1] = vx * s + vy * c;
    }
}
