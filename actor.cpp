#include "actor.h"
#include "level.h"

Actor::Actor(Level *l, double x, double y, double w, double h)
{
    level = l;
    set_rect(dims, x, y, w, h);
    set_pair(v, 0, 0);
    rm = ResourceManager::get_instance();
    image = NULL;
}

Actor::~Actor()
{
}

void Actor::move()
{
    dims[0] += v[0];
    dims[1] += v[1];
}

void Actor::update()
{
}

void Actor::draw(Surface *screen)
{
    if (image != NULL)
	screen->blit(image, dims[0], dims[1]);
}

void Actor::die()
{
}



TimedParticle::TimedParticle(Level *l, double x, double y,
			     double *vel, int t)
    :Actor(l, x, y, 1, 1)
{
    image = rm->get_image("particle.png");
    dims[2] = image->get_width();
    dims[3] = image->get_height();
    dims[0] -= 0.5 * dims[2];
    dims[1] -= 0.5 * dims[3];
    cpy_pair(v, vel);
    timer = t;
    level->add_actor(this);
}

void TimedParticle::update()
{
    Actor::update();

    if (timer == 0)
	level->remove_actor(this);
    else
	timer--;
}



LaserEffect::LaserEffect(Level *l, double *a, double *b,
			 Uint32 c, int t)
    :Actor(l, a[0], a[1], 1, 1)
{
    cpy_pair(p1, a);
    cpy_pair(p2, b);
    color = c;
    timer_max = t;
    timer = timer_max;
    level->add_actor(this);
}

void LaserEffect::update()
{
    if (timer > 0)
	timer--;
    else
	level->remove_actor(this);
}

void LaserEffect::draw(Surface *screen)
{
    double alpha = (255.0 * timer) / timer_max;
    draw_laser(screen, mapRGBA(color, alpha), p1, p2, 5);
}
