#include "ballos.h"

Ballos::Ballos(Level *l)
    :Ship(l, "ballos1.png")
{
    image = rm->get_image("ballos1.png");
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = -800;
    strength = 3;
    health = 100 * strength;
    set_pair(v, 0, 10);
    max_y = 50;
    for (int i = 0; i < 8; i++)
	eyes.push_back(new BallosEye(level, this, i));
    moving = true;
    level->boss = this;
    invincible = true;
    score = 10000;
}

double Ballos::get_health()
{
    int h = 0;
    list<BallosEye *>::iterator eye;
    for (eye = eyes.begin(); eye != eyes.end(); eye++)
	if ((*eye)->health > 0)
	    h += (*eye)->health;
    if (h > 0)
	return h / (strength * 80.0);
    else
	return health / (strength * 100.0);
}

void Ballos::update()
{
    Ship::update();

    if (moving)
    {
	if (dims[1] >= max_y)
	{
	    moving = false;
	    set_pair(v, -8, 0);
	}
    }

    if (eyes.size()  == 0)
	invincible = false;

    if (health <= 70 * strength)
	if (randint(1, 15) == 1 && eyes.size() == 0)
	    create_fire();
    if (randint(1, 30) == 1 && eyes.size() == 0)
	create_lightning();
}

void Ballos::move()
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

    if (v[1] <= -20)
	v[1] = 0;
    else if (dims[1] > 50)
	v[1] -= 1;
    if (health <= 30 * strength && health > 0 && dims[1] == 50)
	if (randint(1, health) == 1)
	    v[1] = 20;
}

void Ballos::create_fire()
{
    double x = dims[0];
    if (randint(0, 1) == 0)
	x += dims[2];
    double y= dims[1] + 0.5 * dims[3];
    new BallosFire(level, x, y);
}

void Ballos::create_lightning()
{
    double x = dims[0] + 0.5 * dims[2];
    double y = dims[1] + 0.5 * dims[3];
    new BallosLightning(level, x, y);
}



BallosEye::BallosEye(Level *l, Ballos *b, int i)
    :Ship(l, "eye.png")
{
    image = rm->get_image("eye.png");
    double x = 120 * cos(i * M_PI / 4) - 0.5 * image->get_width();
    double y = 120 * sin(i * M_PI / 4) - 0.5 * image->get_height();
    x += parent->dims[0] + 0.5 * parent->dims[2];
    y += parent->dims[1] + 0.5 * parent->dims[3];
    angle = i * 6;
    parent = b;
    health = 10 * parent->strength;
    score = 1000;
}

void BallosEye::die()
{
    parent->eyes.remove(this);
    Ship::die();
}

void BallosEye::update()
{
    Ship::update();

    if (parent->moving)
	health = 10 * parent->strength;
    else if (randint(1, 125) == 1)
	create_bullet();
}

void BallosEye::move()
{
    angle = (angle + 1) % 48;
    double t = angle;
    double x = 120 * cos(t * M_PI / 24) - 0.5 * dims[2];
    double y = 120 * sin(t * M_PI / 24) - 0.5 * dims[3];
    x += parent->dims[0] + 0.5 * parent->dims[2];
    y += parent->dims[1] + 0.5 * parent->dims[3];
    set_pair(dims, x, y);
}

void BallosEye::create_bullet()
{
    double x = dims[0] + 0.5 * dims[2];
    double y = dims[1] + 0.5 * dims[3];
    new BallosFire(level, x, y);
}



BallosFire::BallosFire(Level *l, double x, double y)
    :Bullet(l, x, y, 1, 1, 1, 0)
{
    char filename[80];
    for (int i = 0; i < 4; i++)
    {
	snprintf(filename, 80, "fire%d.png", i);
	images[i] = rm->get_image(filename);
    }
    frame = 0;
    dims[2] = images[0]->get_width();
    dims[3] = images[1]->get_width();
    set_pair(v, randint(-2, 2), 6);
    v[0] *= 2;
}

void BallosFire::update()
{
    Bullet::update();
    frame = (frame + 1) % 4;
}

void BallosFire::draw(Surface *screen)
{
    image = images[frame];
    screen->blit(image, dims[0], dims[1]);
}



BallosLightning::BallosLightning(Level *l, double x, double y)
    :Bullet(l, x, y, 1, 1, 1, -1)
{
    image = rm->get_image("lightning.png");
    dims[2] = image->get_width();
    dims[3] = image->get_height();
    set_pair(v, 0, 8);
}

void BallosLightning::draw(Surface *screen)
{
    double p1[] = {dims[0] + 0.5 * dims[2], dims[1]};
    double p2[] = {p1[0], p1[1] + dims[3]};
    Uint32 white = mapRGB(255, 255, 255);
    int radius = (int)(dims[2]);
    for (int i = 0; i < 3; i++)
	draw_lightning(screen, p1, p2, white, radius);
}
