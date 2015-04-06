#include "colorship.h"

ColorShip::ColorShip(Level *l)
    :Ship(l, "color_main.png")
{
    dims[0] = 0.5 * level->dims[2] - 0.5 * image->get_width();
    dims[1] = -200;
    set_pair(v, 0, 5);
    health = 100;
    score = 15000;
    level->boss = this;
    for (int i = 0; i < 4; i++)
	colors[i] = randint(0, 5);

    /* data for side building */
    Surface *armor_image = rm->get_image("color_armor0.png");
    Surface *gun_image = rm->get_image("color_gun.png");
    Surface *body_image = rm->get_image("color_body.png");
    double aw = armor_image->get_width();
    double gw = gun_image->get_width();
    double sw = dims[2];
    double bh = body_image->get_height();

    left_gun = new ColorGun(level, this, -2 * (aw + gw), 0, 0);
    right_gun = new ColorGun(level, this, sw + 2 * aw + gw, 0, 3);

    /* left side */
    parts.push_back(new ColorBody(level, this, -1 * aw, 0, 0));
    parts.push_back(new ColorArmor(level, this, -1 * aw, bh, 0));
    parts.push_back(new ColorGun(level, this, -1 * (aw + gw), bh, 1));
    parts.push_back(new ColorBody(level, this, -1 * (aw + gw), 0, 1));
    parts.push_back(new ColorArmor(level, this, -2 * aw - gw, 0, 0));
    parts.push_back(left_gun);

    /* right side */
    parts.push_back(new ColorBody(level, this, sw, 0, 0));
    parts.push_back(new ColorArmor(level, this, sw, bh, 1));
    parts.push_back(new ColorGun(level, this, sw + aw, bh, 2));
    parts.push_back(new ColorBody(level, this, sw + aw, 0, 1));
    parts.push_back(new ColorArmor(level, this, sw + aw + gw, 0, 1));
    parts.push_back(right_gun);

    max_y = 50;
    tx = 0.5 * level->dims[2];
    laser_timer = 360;

    new ColorActor(level);
}

double ColorShip::get_health()
{
    return health / 100.0;
}

void ColorShip::damage(int pain, int c)
{
    for (int i = 0; i < 4; i++)
	if (c == colors[i])
	    return;
    Ship::damage(pain, c);
}

void ColorShip::die()
{
    Ship::die();

    list<Ship *>::iterator s;
    for (s = parts.begin(); s != parts.end(); s++)
    {
	(*s)->parent = NULL;
	(*s)->invincible = false;
	(*s)->damage((*s)->health, -1);
    }
}

void ColorShip::select_target()
{
    tx = randint(0.1 * level->dims[2], 0.9 * level->dims[2]);
}

void ColorShip::update()
{
    Ship::update();

    if (left_gun->dims[0] < 0)
    {
	dims[0] = -1 * left_gun->pos[0];
	v[0] *= -1;
	select_target();
    }

    if (right_gun->dims[0] + right_gun->dims[2] >
	level->dims[2])
    {
	dims[0] = level->dims[2] - right_gun->dims[2] -
	    right_gun->pos[0];
	v[0] *= -1;
	select_target();
    }

    if (randint(0, 50) == 0)
    {
	int n = randint(0, 3);
	colors[n] = (colors[n] + 1) % 6;
    }

    if (dims[1] > max_y)
    {
	dims[1] = max_y;
	if (randint(0, 1) == 1)
	    v[0] = -3;
	else
	    v[0] = 3;
	v[1] = 0;
    }

    if (dims[1] == max_y)
    {
	if (laser_timer >= 90)
	{
	    double sx = dims[0] + 0.5 * dims[2];
	    double dx = tx - sx;
	    if (fabs(dx) < 5)
	    {
		set_pair(v, 0, 0);
		select_target();
	    }
	    else
		if (fabs(dx) > 0.001) v[0] = 0.08 * dx;
	}
	else
	{
	    set_pair(v, 0, 0);
	}
    }

    if (laser_timer > 30 && laser_timer < 90 &&
	laser_timer % 5 == 0)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + dims[3];
	double theta = randint(1, 360) * M_PI / 180.0;
	double r = randint(30, 60);
	double px = sx + r * cos(theta);
	double py = sy + r * sin(theta);
	double vel[] = {-0.2 * r * cos(theta), -0.2 * r * sin(theta)};
	new TimedParticle(level, px, py, vel, 5);
    }

    if (laser_timer > -90)
	laser_timer--;
    else
	laser_timer = 360;
    if (laser_timer <= 0)
    {
	double lw = min(0 - laser_timer, 20);
	PlayerShip *player = level->player;
	double px = player->dims[0] + 0.5 * player->dims[2];
	double sx = dims[0] + 0.5 * dims[2];
	if (fabs(px - sx) <= lw)
	    player->damage(1, -1);
    }

    Ship::move();
}

void ColorShip::move()
{
}

void ColorShip::draw(Surface *screen)
{
    Ship::draw(screen);

    if (laser_timer <= 0)
    {
	double sx = dims[0] + 0.5 * dims[2];
	double sy = dims[1] + dims[3];
	double lw = min(0 - laser_timer, 20);
	draw_laser(screen, mapRGB(255, 255, 255),
		   (double[]){sx - 1, sy},
		   (double[]){sx - 1, sy + level->dims[3]}, lw);
    }
}



ColorBody::ColorBody(Level *l, ColorShip *p, double px,
		     double py, int side)
    :Ship(l, "color_body.png")
{
    if (side == 1)
    {
	image = rm->get_image("color_gun2.png");
	dims[2] = image->get_width();
	dims[3] = image->get_height();
    }
    parent = p;
    dims[0] = parent->dims[0] + px;
    dims[1] = parent->dims[1] + py;
    set_pair(pos, px, py);
    health = 1;
    invincible = true;
    items = false;
    set_pair(v, 0, 0);
}

void ColorBody::move()
{
    if (parent == NULL)
	return;
    
    dims[0] = parent->dims[0] + pos[0];
    dims[1] = parent->dims[1] + pos[1];
}



ColorArmor::ColorArmor(Level *l, ColorShip *p, double px,
		     double py, int side)
    :Ship(l, "color_armor0.png")
{
    char filename[80];
    snprintf(filename, 80, "color_armor%d.png", side);
    image = rm->get_image(filename);
    dims[2] = image->get_width();
    dims[3] = image->get_height();
    parent = p;
    dims[0] = parent->dims[0] + px;
    dims[1] = parent->dims[1] + py;
    set_pair(pos, px, py);
    health = 1;
    invincible = true;
    items = false;
    set_pair(v, 0, 0);
}

void ColorArmor::move()
{
    if (parent == NULL)
	return;
    
    dims[0] = parent->dims[0] + pos[0];
    dims[1] = parent->dims[1] + pos[1];
}



ColorGun::ColorGun(Level *l, ColorShip *p, double px,
		   double py, int c_id)
    :Ship(l, "color_gun.png")
{
    parent = p;
    dims[0] = parent->dims[0] + px;
    dims[1] = parent->dims[1] + py;
    set_pair(pos, px, py);
    health = 1;
    invincible = true;
    items = false;
    set_pair(v, 0, 0);
    color_id = c_id;
    color = p->colors[color_id];
    fire_timer = 0;
}

void ColorGun::update()
{
    if (parent == NULL)
	return;
    
    Ship::update();

    /* parent != NULL? */
    color = ((ColorShip *)parent)->colors[color_id];

    if (fire_timer == 0)
    {
	create_bullet();
	fire_timer = 15;
    }
    else
	fire_timer--;
}

void ColorGun::move()
{
    if (parent == NULL)
	return;
    
    dims[0] = parent->dims[0] + pos[0];
    dims[1] = parent->dims[1] + pos[1];
}

void ColorGun::create_bullet()
{
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + dims[3];
    Bullet *bullet = new Bullet(level, bx, by, bw, bh, 1, color);
    set_pair(bullet->v, 0, 5);
}



ColorActor::ColorActor(Level *l)
    :Actor(l, 1, 1, 1, 1)
{
    set_pair(v, 0, 2);
    char text[80];
    snprintf(text, 80, "...colors...");
    TTF_Font *font = ResourceManager::get_instance()->get_font();
    SDL_Color c;
    c.r = c.g = c.b = c.unused = 255;
    SDL_Surface *message = TTF_RenderText_Solid(font, text, c);
    image = new Surface(message);
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = -250;
    level->add_actor(this);
}

ColorActor::~ColorActor()
{
    SDL_FreeSurface(image->get_surface());
    delete image;
}

void ColorActor::update()
{
    Actor::update();

    if (dims[1] >level->dims[3])
	level->remove_actor(this);
}

void ColorActor::draw(Surface *screen)
{
    screen->blit(image, dims[0], dims[1]);
}
