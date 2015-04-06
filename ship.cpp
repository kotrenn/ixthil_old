#include "ship.h"
#include "level.h"

Ship::Ship(Level *level, const char *filename)
	:Actor(level, -50000, -50000, 1, 1)
{
	image = rm->get_image(filename);
	dims[2] = image->get_width();
	dims[3] = image->get_height();
	health = 1;
	team = 1;
	hit = false;
	level->add_ship(this);
	items = true;
	particles = true;
	item_rate = 1;
	invincible = false;
	score = 0;
	collisions = true;
	color = -1;
	color_hit = false;
	parent = NULL;
}

Ship::~Ship()
{
}

void Ship::update()
{
	Actor::update();

	if (collisions)
	{
		ResourceManager *rm = ResourceManager::get_instance();
		PlayerShip *player = level->player;
		list<Bullet *> *bullets = level->get_collisions(this);
		if (bullets->size() > 0)
		{
			list<Bullet *>::iterator b;
			int count = 0;
			for (b = bullets->begin(); b != bullets->end(); b++)
				if (!((*b)->dead))
					count++;
			if (health > 0)
			{
				damage(count, bullets->back()->color);
				if (this != player)
					rm->play_sound("enemy_hit.wav");
			}
			for (b = bullets->begin(); b != bullets->end(); b++)
			{
				if (!((*b)->dead))
				{
					level->remove_bullet(*b);
					(*b)->die(this);
					if ((*b)->team == player->team && !invincible)
						player->bullet_hit();
					//hit = true;
				}
			}
			if (invincible)
				rm->play_sound("ricochet.wav");
		}
		delete bullets;
	}
}

void Ship::move()
{
	Actor::move();
}

void Ship::draw(Surface *screen)
{
	if (hit)
		image->invert();
	Actor::draw(screen);
	if (hit)
		image->invert();
}

void Ship::damage(int pain, int c)
{
	if (health < 0)
		return;
	hit = true;
	color_hit = false;
	if (c >= 0)
		if (color == c || color == (c + 3) % 6)
			color_hit = true;
	if (!invincible)
		health -= pain;
	if (health <= 0)
		die();
}

double Ship::get_health()
{
	return 0;
}

void Ship::create_bullet()
{
}

void Ship::die()
{
	Actor::die();

	if (parent != NULL)
		parent->ship_died(this);

	PlayerShip *player = level->player;
	if (this != player)
		player->score += score;

	int k = 2;
	if (level->subspace)
		k = 4;
	int p = randint(1, k * item_rate);
	if ((p == 1 || level->boss == this) && items)
	{
		double x = dims[0];
		double y = dims[1];
		int n = randint(0, 5);
		int t = randint(1, 60);
		if (player->subspace[0])
			t = randint(1, 50);
		if (level->boss == this)
			t = randint(1, 13);
		if (t <= 6) n = IT_BLANK;
		if (t > 6 && t <= 7) n = IT_HEALTH;
		if (t > 7 && t <= 8) n = IT_SHIELD;
		if (t > 10 && t <= 11) n = IT_BOMB;
		if (t > 11 && t <= 12) n = IT_HEALTH_PLUS;
		if (t > 12 && t <= 13) n = IT_SHIELD_PLUS;
		new Item(level, x, y, n);
	}
    
	ResourceManager *rm = ResourceManager::get_instance();
	if (level->boss == this)
	{
		level->boss = NULL;
		rm->play_sound("boss_die.wav");
	}
	else if (player != this)
	{
		char buf[80];
		int die_num = 1 + color % 2;
		if (!(die_num == 1 || die_num == 2))
			die_num = randint(1, 2);
		snprintf(buf, 80, "enemy_die%d.wav", die_num);
		rm->play_sound(buf);
	}

	level->remove_ship(this);
}

void Ship::ship_died(Ship *ship)
{
}
