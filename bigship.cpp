#include "bigship.h"
#include "enemies.h"

BigShip::BigShip(Level *l)
    :Ship(l, "big_ship_center.png")
{
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = -400;
    set_pair(v, 0, 6);
    invincible = true;
    health = 300;
    score = 12000;
    level->boss = this;

    left_conn = new BigShipConnector(level, this);
    left_laser = new BigShipLaser(level, this);
    right_conn = new BigShipConnector(level, this);
    right_laser = new BigShipLaser(level, this);
    left_conn->laser = left_laser;
    right_conn->laser = right_laser;
    left_laser->conn = left_conn;
    right_laser->conn = right_conn;
    reposition();

    tx = -100;
    ty = -100;
    target_delay = 240;
    gun_theta = 0;
    gx1 = dims[0] + 0.25 * dims[2];
    gx2 = dims[0] + 0.75 * dims[3];
    gy = dims[1] + 0.8 * dims[3];
    gv = 3;
    ship_timer = -1;
}

double BigShip::get_health()
{
    int h = 0;
    if (left_laser != NULL)
	h += left_laser->health;
    if (right_laser != NULL)
	h += right_laser->health;
    if (h > 0)
	return h / 300.0;
    else
	return health / 300.0;
}

void BigShip::reposition()
{
    double x = 0;
    double y = 0;
    Surface *conn_image = rm->get_image("big_ship_connector.png");
    Surface *laser_image = rm->get_image("big_ship_laser.png");
    double cx = x - conn_image->get_width();
    double cy = y + 0.5 * (image->get_height() - conn_image->get_height());
    left_conn->offset[0] = cx;
    left_conn->offset[1] = cy;
    double lx = cx - laser_image->get_width();
    double ly = y;
    left_laser->offset[0] = lx;
    left_laser->offset[1] = ly;
    cx = x + image->get_width();
    right_conn->offset[0] = cx;
    right_conn->offset[1] = cy;
    lx = cx + conn_image->get_width();
    right_laser->offset[0] = lx;
    right_laser->offset[1] = ly;
}

void BigShip::die()
{
    list<Ship *>::iterator s;
    for (s = ships.begin(); s != ships.end(); s++)
	(*s)->parent = NULL;
    
    Ship::die();
}

void BigShip::update()
{
    Ship::update();

    if (dims[1] >= 50 && left_laser != NULL)
    {
	if (left_laser->invincible)
	{
	    dims[1] = 50;
	    set_pair(v, -3, 0);
	    left_laser->invincible = false;
	    right_laser->invincible = false;
	}
    }

    list<Ship *>::iterator s;
    if (left_laser == NULL && right_laser == NULL)
    {
	invincible = false;
	if (ship_timer <= 0 && ships.size() < 2)
	{
	    double sx = dims[0] + 0.5 * dims[2];
	    double sy = dims[1] + dims[3];
	    CloudManager *cm = CloudManager::get_instance();
	    cm->set_center((double[]){sx, sy});
	    for (int i = 0; i < 6; i++)
	    {
		Ship *enemy = new CloudShip(level);
		enemy->dims[0] = dims[0] + 0.5 * dims[2];
		enemy->dims[0] -= 0.5 * enemy->dims[2];
		enemy->dims[1] = dims[1] + dims[3];
		enemy->dims[1] -= enemy->dims[3];
		enemy->parent = this;
		ships.push_back(enemy);
	    }
	    ship_timer = 300;
	}
	else
	    ship_timer--;
    }
    else
    {
	if (target_delay > 135)
	{
	    tx = -100;
	    ty = -100;
	}
	else if (target_delay > 45)
	{
	    PlayerShip *player = level->player;
	    tx = player->dims[0] + 0.5 * player->dims[2];
	    ty = player->dims[1] + 0.5 * player->dims[3];
	}
	target_delay--;
	if (target_delay <= -90)
	    target_delay = 240;
    }
    ResourceManager *rm = ResourceManager::get_instance();
    if (target_delay == 45)
	rm->play_sound("laser.wav");
    else if (target_delay == 0)
	rm->play_sound("laser2.wav");

    if (abs(gun_theta) % 15 == 0)
	create_bullet();
    if (gun_theta == 0 &&
	gv > 0 &&
	left_laser == NULL &&
	right_laser == NULL &&
	health <= 100)
	main_gun();
    gun_theta += gv;
    if (abs(gun_theta) == 60)
	gv *= -1;

    double left_w = 0;
    double right_w = 0;
    if (left_laser != NULL)
	left_w = left_laser->dims[2] + left_conn->dims[2];
    if (right_laser != NULL)
	right_w = right_laser->dims[2] + right_conn->dims[2];
    if (dims[0] - left_w < 0)
    {
	dims[0] = left_w;
	v[0] *= -1;
    }
    if (dims[0] + dims[2] + right_w > level->dims[2])
    {
	dims[0] = level->dims[2] - dims[2] - right_w;
	v[0] *= -1;
    }
    
    Ship::move();
}

void BigShip::move()
{
}

void BigShip::ship_died(Ship *ship)
{
    int count = 0;
    list<Ship *>::iterator s;
    for (s = ships.begin(); s != ships.end(); s++)
	if (*s == ship)
	    count++;
    if (count > 0)
	ships.remove(ship);
}

void BigShip::create_bullet()
{
    gx1 = dims[0] + 0.25 * dims[2];
    gx2 = dims[0] + 0.75 * dims[2];
    gy = dims[1] + 0.8 * dims[3];
    double bw = BULLET_W, bh = BULLET_H;
    double by = gy - 0.5 * bh;
    Bullet *bullet1 = new Bullet(level, gx1 - 0.5 * bw, by, bw, bh, 1, 4);
    Bullet *bullet2 = new Bullet(level, gx2 - 0.5 * bw, by, bw, bh, 1, 4);
    double vel = 8;
    double t = (gun_theta - 90) * M_PI / 180.0;
    set_pair(bullet1->v,  vel * cos(t), -vel * sin(t));
    set_pair(bullet2->v, -vel * cos(t), -vel * sin(t));
}

void BigShip::main_gun()
{
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * dims[2] - 0.5 * bw;
    double by = dims[1] + 0.5 * dims[3] - bh;
    Bullet *bullet1 = new Bullet(level, bx, by, bw, bh, 1, 2);
    Bullet *bullet2 = new Bullet(level, bx, by, bw, bh, 1, 2);
    Bullet *bullet3 = new Bullet(level, bx, by, bw, bh, 1, 2);
    set_pair(bullet1->v, 0, 8);
    set_pair(bullet2->v, -2, 6);
    set_pair(bullet3->v, 2, 6);
}

void BigShip::draw(Surface *screen)
{
    Ship::draw(screen);

    if (target_delay <= 135 && target_delay > 0 &&
	(left_laser != NULL || right_laser != NULL))
    {
	Surface *target = rm->get_image("target.png");
	if (target_delay <= 45 && target_delay % 2 == 0)
	    target = rm->get_image("target_red.png");
	double x = tx - 0.5 * target->get_width();
	double y = ty - 0.5 * target->get_height();
	screen->blit(target, x, y);
    }
}



BigShipConnector::BigShipConnector(Level *l, BigShip *p)
    :Ship(l, "big_ship_connector.png")
{
    parent = p;
    set_pair(dims, -1000, -1000);
    set_pair(v, 0, 0);
    items = false;
    invincible = true;
    health = 1;
    laser = NULL;
}

void BigShipConnector::damage(int pain, int c)
{
    Ship::damage(pain, c);

    if (invincible)
	laser->damage(pain, c);
}

void BigShipConnector::die()
{
    Ship::die();

    if (parent->left_conn == this)
	parent->left_conn = NULL;
    if (parent->right_conn == this)
	parent->right_conn = NULL;
}

void BigShipConnector::move()
{
    for (int i = 0; i < 2; i++)
	dims[i] = parent->dims[i] + offset[i];
}



BigShipLaser::BigShipLaser(Level *l, BigShip *p)
    :Ship(l, "big_ship_laser.png")
{
    parent = p;
    set_pair(dims, -1000, -1000);
    set_pair(v, 0, 0);
    invincible = true;
    health = 150;
    score = 8000;
    lx = dims[0] + 0.5 * dims[2];
    ly = dims[1] + dims[3];
    ux = 0;
    uy = 0;
    lw = 0;
    conn = NULL;
}

void BigShipLaser::die()
{
    Ship::die();

    if (parent->left_laser == this)
	parent->left_laser = NULL;
    if (parent->right_laser == this)
	parent->right_laser = NULL;
    conn->invincible = false;
    conn->damage(conn->health, -1);
}

void BigShipLaser::update()
{
    Ship::update();

    if (hit)
	conn->hit = true;

    if (parent->target_delay < 0)
    {
	PlayerShip *player = level->player;
	double px = player->dims[0] + 0.5 * player->dims[2];
	double py = player->dims[1] + 0.5 * player->dims[3];
	double hx = px - lx;
	double hy = py - ly;
	double H = hx * ux + hy * uy;
	double tx = hx - H * ux;
	double ty = hy - H * uy;
	double mag = sqrt(tx * tx + ty * ty);
	if (mag <= lw)
	    player->damage(1, -1);
    }

}

void BigShipLaser::move()
{
    for (int i = 0; i < 2; i++)
	dims[i] = parent->dims[i] + offset[i];
    
    lx = dims[0] + 0.5 * dims[2];
    ly = dims[1] + dims[3];
    ux = parent->tx - lx;
    uy = parent->ty - ly;
    double mag = sqrt(ux * ux + uy * uy);
    ux *= 1 / mag;
    uy *= 1 / mag;
}

void BigShipLaser::draw(Surface *screen)
{
    Ship::draw(screen);

    Uint32 red = mapRGB(255, 0, 0);
    if (parent->target_delay < 0)
    {
	lw = 40;
	if (parent->target_delay > -20)
	    lw = -2 * parent->target_delay;
	draw_laser(screen, red, (double[]){lx, ly},
		   (double[]){lx + 1000 * ux, ly + 1000 * uy}, lw);
    }
}
