#include "stateship.h"

StateShip::StateShip(Level *l)
    :Ship(l, "stateship.png")
{
    dims[0] = 0.5 * level->dims[2] - 0.5 * image->get_width();
    dims[1] = -200;
    level->boss = this;
    for (int i = 0; i < 6; i++)
	code[i] = i;
    for (int i = 0; i < 6; i++)
    {
	for (int j = 0; j < 3; j++)
	{
	    int k = randint(0, 5);
	    int temp = code[i];
	    code[i] = code[k];
	    code[k] = temp;
	}
    }
    if (code[0] == 0)
    {
	code[0] = code[5];
	code[5] = 0;
    }
    color = code[0];
    state_health = 30;
    final_health = 150;
    health = state_health;
    center[0] = 0.5 * level->dims[2];
    center[1] = 0.3 * level->dims[3];
    dt = 0;
    ddt = 0;
    score = 40000;
    new PasswordActor(level);
}

double StateShip::get_health()
{
    if (state < 6)
	return (1.0 * health) / state_health;
    else
	return (1.0 * health) / final_health;
}

void StateShip::damage(int pain, int c)
{
    if (health < 0)
	return;
    color_hit = false;
    if (c >= 0)
	if (color == c)
	    color_hit = true;
    if (!invincible)
	health -= pain;
    if (health <= 0)
	die();
}

void StateShip::die()
{
    if (color_hit)
    {
	state++;
	if (state == 6)
	{
	    invincible = true;
	    ddt = 8;
	}
	else if (state < 6)
	{
	    invincible = true;
	    color = code[state];
	}
	else
	    Ship::die();
    }
    else
    {
	if (state == 6)
	    Ship::die();
	else
	{
	    invincible = true;
	    state = 0;
	    color = code[0];
	    dt = 0;
	}
    }
    if (state < 6)
	ddt = state;
}

void StateShip::update()
{
    Ship::update();

    dt = (dt + ddt) % 360;

    double r = 0.2 * level->dims[3];
    double t = (state * 60 - 90 + dt) * M_PI / 180.0;
    double tx = center[0] + r * cos(t);
    double ty = center[1] + r * sin(t);
    double sx = dims[0] + 0.5 * dims[2];
    double sy = dims[1] + 0.5 * dims[3];
    double dx = tx - sx;
    double dy = ty - sy;
    set_pair(v, 0, 0);
    if (fabs(dx) > 0.001) v[0] = 0.5 * dx;
    if (fabs(dy) > 0.001) v[1] = 0.5 * dy;

    if (invincible)
    {
	if (state == 6)
	{
	    if (health >= final_health)
	    {
		health = final_health;
		invincible = false;
	    }
	    else
		health += 3;
	}
	else
	{
	    if (health >= state_health)
	    {
		health = state_health;
		invincible = false;
	    }
	    else
		health += 1;
	}
    }

    int k = 15;
    if (state == 6)
	k = 5;
    if (randint(1, k) == 1)
	create_bullet();
}

void StateShip::draw(Surface *screen)
{
    Uint32 c = Bullet::get_color(state);

    double r0 = 0.2 * level->dims[3];
    for (int j = 0; j < 5; j++)
    {
	double r = r0 + 10 * (j - 2);
	for (int i = 0; i < 6; i++)
	{
	    double t1 = (i * 60 - 90 + dt) * M_PI / 180.0;
	    double t2 = ((i + 1) * 60 - 90 + dt) * M_PI / 180.0;
	    double x1 = center[0] + r * cos(t1);
	    double y1 = center[1] + r * sin(t1);
	    double x2 = center[0] + r * cos(t2);
	    double y2 = center[1] + r * sin(t2);
	    draw_line(screen, c, (double[]){x1, y1}, (double[]){x2, y2});
	}
    }

    Ship::draw(screen);
}

void StateShip::create_bullet()
{
    double bw = BULLET_W, bh = BULLET_H;
    double bx = dims[0] + 0.5 * (dims[2] - bw);
    double by = dims[1] + 0.5 * (dims[3] - bh);
    if (state == 6)
    {
	Bullet *bullet = new Bullet(level, bx, by, bw,
				    bh, 1, randint(0, 5));
	bullet->colorific = true;
	int t = randint(-45, 45) + 90;
	bullet->v[0] = 6 * cos(t * M_PI / 180.0);
	bullet->v[1] = 6 * sin(t * M_PI / 180.0);
    }
    else
    {
	Bullet *bullet = new Bullet(level, bx, by, bw, bh,
				    1, state);
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
}



PasswordActor::PasswordActor(Level *l)
    :Actor(l, 1, 1, 1, 1)
{
    set_pair(v, 0, 2);
    char text[80];
    snprintf(text, 80, "...password...");
    TTF_Font *font = ResourceManager::get_instance()->get_font();
    SDL_Color c;
    c.r = c.g = c.b = c.unused = 255;
    SDL_Surface *message = TTF_RenderText_Solid(font, text, c);
    image = new Surface(message);
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = -250;
    level->add_actor(this);
}

PasswordActor::~PasswordActor()
{
    SDL_FreeSurface(image->get_surface());
    delete image;
}

void PasswordActor::update()
{
    Actor::update();

    if (dims[1] >level->dims[3])
	level->remove_actor(this);
}

void PasswordActor::draw(Surface *screen)
{
    screen->blit(image, dims[0], dims[1]);
}
