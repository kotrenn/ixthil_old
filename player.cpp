#include "player.h"
#include "inputmanager.h"

PlayerShip *PlayerShip::instance = NULL;

PlayerShip *PlayerShip::get_instance(Level *l)
{
	if (instance == NULL)
		instance = new PlayerShip(l);
	return instance;
}

PlayerShip::PlayerShip(Level *l)
	:Ship(l, "player.png")
{
	demo = false;
	reset();
}

PlayerShip::~PlayerShip()
{
}

void PlayerShip::cheat()
{
	for (int i = 0; i < 6; i++)
	{
		upgrades[i] = 4;
		subspace[i] = true;
	}
	num_subspace = 0;
	blank = 100;
	max_health = 10;
	max_shield = 1000;
	health = max_health;
	shield = max_shield;
	bombs = 12;
	energy = 1000;
}

void PlayerShip::reset()
{
	dims[0] = 0.5 * (level->dims[2] - dims[2]);
	dims[1] = level->dims[3] - dims[3];
	fire_timer = 0;
	fire_delay = 5;
	for (int i = 1; i < 6; i++)
		upgrades[i] = 0;
	upgrades[0] = 1;
	cur_color = 0;
	blank = 0;
	blank_timer = 0;
	bullets = 0;
	bullet_hits = 0;
	team = 0;
	max_health = 3;
	max_shield = 300;
	health = max_health;
	shield = max_shield;
	score = 0;
	invincible_timer = 0;
	bombs = 3;
	bomb_rad = 0;
	bomb_max = 0.5 * max(level->dims[2], level->dims[3]);
	energy = 0;
	required_energy = 1;
	energizing = false;
	energy_select = 0;
	energy_timer = 0;
	energy_theta = 0;
	energy_hue = 0;
	for (int i = 0; i < 6; i++)
		subspace[i] = false;
	num_subspace = 0;
	energy_bullet_timer = 0;
	laser_timer = 0;
	items = false;
	select_delay = 0;
	tx = randint(0, level->dims[2] - dims[2]);
	ty = randint(0.7 * level->dims[3], level->dims[3] - dims[3]);
	combo = 0;
	if (demo)
	{
		for (int i = 0; i < 6; i++)
			upgrades[i] = 4;
		cur_color = randint(0, 5);
		energy = 100000;
	}
}

void PlayerShip::damage(int pain, int c)
{
	combo = 0;
	if (!invincible)
	{
		if (shield <= 100)
		{
			health--;
			shield = 0;
		}
		else
			shield -= 100;
		invincible = true;
		invincible_timer = 30;
		if (!demo)
		{
			upgrades[cur_color] -= 1;
			if (upgrades[cur_color] < 0)
				upgrades[cur_color] = 0;
			if (upgrades[0] < 1 && !subspace[4])
				upgrades[0] = 1;
			while (upgrades[cur_color] < 1 &&
			       !subspace[4])
				cur_color = (cur_color + 1) % 6;
		}
	}
	if (health <= 0)
	{
		level->remove_ship(this);
		die();
	}
}

void PlayerShip::die()
{
	FILE *inFile;
	if ((inFile = fopen("profile.dat", "r")) == NULL)
	{
		FILE *outFile;
		if ((outFile = fopen("profile.dat", "w")) == NULL)
		{
			fprintf(stderr, "Could not open profile.dat for writing\n");
			exit(1);
		}
		fprintf(outFile, "1 0\n");
		fclose(outFile);
		if ((inFile = fopen("profile.dat", "r")) == NULL)
		{
			fprintf(stderr, "Could not open profile.dat for reading\n");
			exit(1);
		}
	}

	int num_stages, num_crystals;
	if (fscanf(inFile, "%d %d", &num_stages, &num_crystals) < 2)
	{
		FILE *outFile;
		if ((outFile = fopen("profile.dat", "w")) == NULL)
		{
			fprintf(stderr, "Could not open profile.dat for writing\n");
			exit(1);
		}
		fprintf(outFile, "1 0\n");
		fclose(outFile);
		num_stages = 1;
		num_crystals = 0;
	}
	fclose(inFile);

	int stages[] = { 1, 5, 9, 13, 17, 21, 26, 31 };
	int stage = 0;
	for (int i = 0; i < 8; i++)
		if (level->level >= stages[i])
			stage++;
	int crystals = num_subspace;
	stage = max(stage, num_stages);
	crystals = max(crystals, num_crystals);

	if (stage <= num_stages && crystals <= num_crystals)
		return;
    
	FILE *outFile;
	if ((outFile = fopen("profile.dat", "w")) == NULL)
	{
		fprintf(stderr, "Could not open profile.dat for writing\n");
		exit(1);
	}
	fprintf(outFile, "%d %d\n", stage, crystals);
	fclose(outFile);
}

void PlayerShip::update()
{
	Ship::update();
	InputManager *im = InputManager::get_instance();

	if (health > max_health)
		health = max_health;

	if (invincible_timer > 0)
		invincible_timer--;
	else
		invincible = false;

	if (shield < max_shield)
	{
		double r = 0.33;
		if (subspace[1])
			r = 0.5;
		shield += r;
	}
	if (shield > max_shield)
		shield = max_shield;

	if (bomb_rad > bomb_max)
		bomb_rad = 0;
	else if (bomb_rad > 0)
	{
		bomb_rad += 10;
		double sx = dims[0] + 0.5 * dims[2];
		double sy = dims[1] + 0.5 * dims[3];
		double r = bomb_rad * bomb_rad;
		list<Bullet *>::iterator b;
		list<Bullet *> *bullets = level->get_bullets();
		for (b = bullets->begin(); b != bullets->end(); b++)
		{
			if ((*b)->team != team)
			{
				double bx = (*b)->dims[0] + 0.5 * (*b)->dims[2];
				double by = (*b)->dims[1] + 0.5 * (*b)->dims[3];
				double dx = sx - bx;
				double dy = sy - by;
				if (dx * dx + dy * dy <= r)
				{
					(*b)->die(this);
					level->remove_bullet(*b);
				}
			}
		}
		list<Ship *>::iterator t;
		list<Ship *> *ships = level->get_ships();
		for (t = ships->begin(); t != ships->end(); t++)
		{
			if (*t != this)
			{
				double tx = (*t)->dims[0] + 0.5 * (*t)->dims[2];
				double ty = (*t)->dims[1] + 0.5 * (*t)->dims[3];
				double dx = sx - tx;
				double dy = sy - ty;
				if (dx * dx + dy * dy <= r)
					(*t)->damage(1, -1);
			}
		}
	}

	if (demo)
		health = bombs = 1337;

	list<Item *> *items = level->get_items();
	list<Item *>::iterator i;
	for (i = items->begin(); i != items->end(); i++)
	{
		double dx = dims[0] - (*i)->dims[0];
		double dy = dims[1] - (*i)->dims[1];
		double dist = dx * dx + dy * dy;
		if (rect_collides(dims, (*i)->dims))
		{
			level->remove_item(*i);
			int j = (*i)->item_type;
			if (j >= 0)
			{
				upgrades[j] += 0.5;
				if (upgrades[j] > 4)
					upgrades[j] = 4;
			}
			else if (j == IT_BLANK)
				blank += 1;
			else if (j == IT_BOMB)
				bombs += 1;
			else if (j == IT_HEALTH)
				health += 1;
			else if (j == IT_SHIELD)
				shield += 100;
			else if (j == IT_HEALTH_PLUS)
			{
				max_health += 1;
				health += 1;
			}
			else if (j == IT_SHIELD_PLUS)
			{
				max_shield += 100;
				shield += 100;
			}
			else if (j == IT_ENERGY)
				energy += 1;
			else if (j == IT_PORTAL)
			{
				level->enter_subspace((PortalItem *)(*i));
				energy -= required_energy;
			}
			if (health > max_health)
				health = max_health;
			if (shield > max_shield)
				shield = max_shield;
			rm->play_sound("item_grab.wav");
		}
		else if (((!im->is_pressed(IM_SHOOT) ||
		           demo) && dist < 200 * 200 &&
		          (*i)->item_type != IT_PORTAL) ||
		         (*i)->pulled)
		{
			(*i)->pulled = true;
			double theta = 45;
			if (level->level % 2 == 0)
				theta = -45;
			double c = cos(theta * M_PI / 180.0);
			double s = sin(theta * M_PI / 180.0);
			double r[2];
			r[0] = dims[0] + 0.5 * dims[2];
			r[1] = dims[1] + 0.5 * dims[3];
			r[0] -= (*i)->dims[0] + (*i)->dims[2];
			r[1] -= (*i)->dims[1] + (*i)->dims[3];;
			double a[] = { c * r[0] + s * r[1], -s * r[0] + c * r[1] };
			double alpha = 0.25;
			if (r[0] * r[0] + r[1] * r[1] < 25 * 25)
				alpha = 0.6;
			(*i)->v[0] = alpha * a[0];
			(*i)->v[1] = alpha * a[1];
		}
	}

	if (energy_bullet_timer == 0)
	{
		if (subspace[5])
		{
			energy_bullet_timer = 8;
			double s = 10;
			double vel[][2] = {{-1, s}, {1, s}};
			double bw = BULLET_W;
			double bh = BULLET_H;
			double bx = dims[0] + 0.5 * (dims[2] - bw);
			double by = dims[1] - bh;
			for (int vi = 0; vi < 2; vi++)
			{
				int n = randint(0, 5);
				Bullet *bullet = new AccelBullet(level, bx, by,
				                                 bw, bh, -2, 0, n);
				bullet->colorific = true;
				cpy_pair(bullet->v, vel[vi]);
			}
		}
	}
	else
		energy_bullet_timer--;

	for (int j = 1; j <= 4; j++)
	{
		int n = 0;
		for (int i = 0; i < 6; i++)
			if (j <= (int)upgrades[i])
				n++;
		if (n > 0)
		{
			int cur = 0;
			double dw = 1.0 / n;
			for (int i = 0; i < 6; i++)
			{
				if (j <= (int)upgrades[i])
				{
					double per = randint(0, 500) / 500.0;
					per = per * dw + cur * dw;
					double sx = level->dims[0];
					double sy = level->dims[1];
					if (j == 4) sx += per * level->dims[2];
					if (j == 2) sy += per * level->dims[3];
					if (j == 1)
					{
						sx += per * level->dims[2];
						sy += level->dims[3];
					}
					if (j == 3)
					{
						sy += per * level->dims[3];
						sx += level->dims[2];
					}
					/*
					  new ColorParticle(level, Bullet::get_color(i), sx, sy,
					  (double[]){0, 0}, 10, 0.8, randint(15, 25));
					*/
					cur++;
				}
			}
		}
	}
}

void PlayerShip::move()
{
	//Ship::move();
	InputManager *im = InputManager::get_instance();
    
	double s = 5;
	if (!demo)
	{
		if (im->is_pressed(IM_UP))
			v[1] = -s;
		else if (im->is_pressed(IM_DOWN))
			v[1] = s;
		else
			v[1] = 0;
		if (im->is_pressed(IM_LEFT))
			v[0] = -s;
		else if (im->is_pressed(IM_RIGHT))
			v[0] = s;
		else
			v[0] = 0;
	}
	else
	{
		v[0] = tx - dims[0];
		if (v[0] < -s)
			v[0] = -s;
		else if (v[0] > s)
			v[0] = s;
		v[1] = ty - dims[1];
		if (v[1] < -s)
			v[1] = -s;
		else if (v[1] > s)
			v[1] = s;
		for (int i = 0; i < 2; i++)
		{
			dims[i] += v[i];
			v[i] = 0;
		}
		double dx = tx - dims[0];
		double dy = ty - dims[1];
		if (dx * dx + dy * dy < 20)
		{
			tx = randint((int)(0.2 * level->dims[2]),
			             (int)(0.8 * level->dims[2]));
			ty = randint((int)(0.7 * level->dims[3]),
			             (int)(level->dims[3] - dims[3]));
		}
	}
	Ship::move();

	if (dims[0] < 0)
		dims[0] = 0;
	if (dims[0] + dims[2] > level->dims[2])
		dims[0] = level->dims[2] - dims[2];
	if (dims[1] < 0.3 * level->dims[3])
		dims[1] = 0.3 * level->dims[3];
	if (dims[1] + dims[3] > level->dims[3])
		dims[1] = level->dims[3] - dims[3];

	fire_timer++;
	if (im->is_pressed(IM_SHOOT) || demo)
	{
		if (fire_timer >= fire_delay)
		{
			create_bullet();
			fire_timer = 0;
		}
	}

	if (select_delay == 0)
	{
		if ((im->is_pressed(IM_PREV) && !demo) ||
		    (randint(1, 600) == 1 && demo))
		{
			cur_color = (cur_color + 5) % 6;
			while (upgrades[cur_color] < 1 &&
			       blank == 0 && !subspace[4])
				cur_color = (cur_color + 5) % 6;
			select_delay = 3;
		}
		if ((im->is_pressed(IM_NEXT) && !demo) ||
		    (randint(1, 600) == 1 && demo))
		{
			cur_color = (cur_color + 1) % 6;
			while (upgrades[cur_color] < 1 &&
			       blank == 0 && !subspace[4])
				cur_color = (cur_color + 1) % 6;
			select_delay = 3;
		}
	}
	else
		select_delay--;

	if ((im->is_pressed(IM_BOMB) && bombs > 0) ||
	    (demo && randint(1, 2100) == 1))
	{
		if (bomb_rad == 0)
		{
			bomb_rad = 1;
			bombs--;
		}
	}

	if (blank > 0 && upgrades[cur_color] < 4)
	{
		if (im->is_pressed(IM_BLANK))
		{
			if (blank_timer == 0)
			{
				blank_timer = 1;
				blank--;
				upgrades[cur_color] += 0.5;
			}
		}
		else
			blank_timer = 0;
	}
    
	if (energy_select > 0)
		energy_select--;
	if ((energy > 0 && im->is_pressed(IM_ENERGY) &&
	     energy_select == 0 && !demo) ||
	    (demo && randint(1, 3000) == 1))
	{
		energizing = !energizing;
		energy_select = 5;
		energy_rad = 0;
	}
	int energy_speed = (int)(16 * sin(energy_rad * M_PI / 180.0));
	//energy_theta = (energy_theta + 360 + (int)(16 * sin(energy_rad * M_PI / 180.0))) % 360;
	energy_theta = (energy_theta + 360 + energy_speed) % 360;
	energy_rad = (energy_rad + 3) % 360;
	if (energizing)
	{
		if (energy_timer <= 0)
		{
			energy--;
			energy_timer = 5;
		}
		else
			energy_timer--;
		if (subspace[3] || demo)
		{
			if (laser_timer <= 0)
			{
				list<Bullet *> *bullets = level->get_bullets();
				list<Bullet *>::iterator b = bullets->begin();
				Bullet *closest = *b;
				while (((*b)->team == team) && b != bullets->end())
					b++;
				if (b != bullets->end())
				{
					double px = dims[0] + 0.5 * dims[2];
					double py = dims[1] + 0.5 * dims[3];
					double dx = px - ((*b)->dims[0] + 0.5 * (*b)->dims[2]);
					double dy = py - ((*b)->dims[1] + 0.5 * (*b)->dims[3]);
					double dist = dx * dx + dy * dy;
					for (b++; b != bullets->end(); b++)
					{
						if ((*b)->team != team)
						{
							dx = px - ((*b)->dims[0] + 0.5 * (*b)->dims[2]);
							dy = py - ((*b)->dims[1] + 0.5 * (*b)->dims[3]);
							double d = dx * dx + dy * dy;
							if (d < dist)
							{
								closest = *b;
								dist = d;
							}
						}
					}
					if (closest->team != team)
					{
						level->remove_bullet(closest);
						closest->die(this);
						double bx = closest->dims[0];
						double by = closest->dims[1];
						bx += 0.5 * closest->dims[2];
						by += 0.5 * closest->dims[3];
						Uint32 color = closest->get_color(closest->color);
						new LaserEffect(level,
						                (double[]){ px, py },
						                (double[]){ bx, by },
						                color, 4);
						rm->play_sound("energy_laser.wav");
					}
					laser_timer = 10;
				}
			}
			else
				laser_timer--;
		}
	}
	if (energy <= 0)
	{
		energy = 0;
		energizing = false;
	}
}

void PlayerShip::bullet_hit()
{
	bullet_hits++;
	combo++;
	if (combo % 5 == 0)
		score += combo / 5;
}

void PlayerShip::create_bullet()
{
	int up = (int)(upgrades[cur_color]);
	if (subspace[4])
		up++;
	if (energizing)
		up++;
	if (up < 1)
		return;
	double bw = BULLET_W;
	double bh = BULLET_H;
	double bx = dims[0] + 0.5 * (dims[2] - bw);
	double by = dims[1] - bh;
	int c = -1;
	if (cur_color == 0)
	{
		fire_delay = 6;
		c = 0;
		if (up < 2)
		{
			Bullet *bullet = new Bullet(level, bx, by, bw, bh, 0, c);
			set_pair(bullet->v, 0, -10);
		}
		else
		{
			fire_delay = 12;
			bx -= 0.5 * bw;
			int limit = 2 * (up - 1);
			for (int i = 0; i < limit; i++)
			{
				Bullet *bullet1 = new Bullet(level, bx, by - i * bh,
				                             bw, bh, 0, c);
				Bullet *bullet2 = new Bullet(level, bx + bw, by - i * bh,
				                             bw, bh, 0, c);
				set_pair(bullet1->v, 0, -10);
				set_pair(bullet2->v, 0, -10);
			}
		}
	}
	else if (cur_color == 1)
	{
		fire_delay = 6 + up;
		c = 1;
		int radius = 50 + up * 25;
		Bullet *bullet = new LightningBullet(level, bx, by,
		                                     bw, bh, c, radius);
		set_pair(bullet->v, 0, -10);
		for (int i = 0; i < up; i++)
		{
			bullet = new Bullet(level, bx, by, bw, bh, 0, c);
			set_pair(bullet->v, 0, -10);
		}
	}
	else if (cur_color == 2)
	{
		fire_delay = 12;
		c = 2;
		int n = 2 * (up - 1);
		if (n == 0)
			n = 1;
		for (int i = 0; i < n; i++)
		{
			Bullet *bullet = new HomingBullet(level, bx, by, bw, bh, c);
			set_pair(bullet->v, 0, -10);
		}
	}
	else if (cur_color == 3)
	{
		fire_delay = 4;
		c = 3;
		for (int i = 0; i < up; i++)
		{
			Bullet *bullet = new Bullet(level, bx, by, bw, bh, 0, c);
			bullet->v[0] = 0.1 * randint(-20, 20);
			bullet->v[1] = -10 + 0.5 * fabs(bullet->v[0]);
		}
	}
	else if (cur_color == 4)
	{
		fire_delay = 12;
		c = 4;
		bx -= 0.5 * bw;
		for (int i = 1; i < up + 1; i++)
		{
			Bullet *bullet1 = new Bullet(level, bx, by - i * bh,
			                             bw, bh, 0, c);
			Bullet *bullet2 = new Bullet(level, bx + bw, by - i * bh,
			                             bw, bh, 0, c);
			set_pair(bullet1->v, -0.5 * i, -6);
			set_pair(bullet2->v, 0.5 * i, -6);
		}
		Bullet *bullet3 = new Bullet(level, bx + 0.5 * bw, by - bh,
		                             bw, bh, 0, c);
		set_pair(bullet3->v, 0, -6);
	}
	else if (cur_color == 5)
	{
		int purple = up;
		fire_delay = 6 / purple + 1;
		c = 5;
		Bullet *bullet = new Bullet(level, bx, by, bw, bh, 0, c);
		set_pair(bullet->v, 0, -15);
		bullet = new Bullet(level, bx, by - bh, bw, bh, 0, c);
		set_pair(bullet->v, 0, -15);
		if (up >= 5)
		{
			for (int i = 5; i <= up; i++)
			{
				bullet = new Bullet(level, bx, by - (i - 3) * bh,
				                    bw, bh, 0, c);
				set_pair(bullet->v, 0, -15);
			}
		}
	}

	ResourceManager *rm = ResourceManager::get_instance();
	rm->play_sound("player_shoot.wav");
}

void PlayerShip::draw(Surface *screen)
{
	if (invincible_timer % 2 == 0 || shield >= 100)
		Ship::draw(screen);

	if (invincible_timer > 0 && shield >= 10)
	{
		double cx = dims[0] + 0.5 * dims[2];
		double cy = dims[1] + 0.5 * dims[3];
		draw_lightning_circle(screen, (double[]){cx, cy},
		                      mapRGB(255, 255, 255), 100);
	}

	if (bomb_rad > 0)
	{
		double cx = dims[0] + 0.5 * dims[2];
		double cy = dims[1] + 0.5 * dims[3];
		draw_lightning_circle(screen, (double[]){cx, cy},
		                      mapRGB(255, 255, 255), bomb_rad);
	}

	if (energizing)
	{
		double cx = dims[0] + 0.5 * dims[2];
		double cy = dims[1] + 0.5 * dims[3];
		double r = 2 * max(dims[2], dims[3]);
		r *= sin(energy_rad * M_PI / 180.0);
		int size = 8;
		Uint32 fade[size];
		for (int i = 0; i < 6; i++)
		{
			double t = (energy_theta + 60 * i) * M_PI / 180.0;
			double x = cx + r * cos(t);
			double y = cy + r * sin(t);
			for (int f = 0; f < size; f++)
				fade[f] = mapRGBA(Bullet::get_color(i),
				                  (Uint8)((size - f) * 32 - 1));
			draw_point(screen, fade[0], x, y);
			for (int f = 1; f < size; f++)
			{
				for (int a = 0; a < f; a++)
				{
					draw_point(screen, fade[f], x - f + a, y + a);
					draw_point(screen, fade[f], x + f - a, y - a);
					draw_point(screen, fade[f], x - a, y - f + a);
					draw_point(screen, fade[f], x + a, y + f - a);
				}
			}
		}
	}
}

void PlayerShip::draw_interface(Surface *screen)
{
	Uint32 white = mapRGB(255, 255, 255);
    
	if (level->paused)
	{
		double cx = level->dims[0] + 0.5 * level->dims[2];
		double cy = level->dims[1] + 0.5 * level->dims[3];
		for (int i = 0; i < 6; i++)
		{
			double t = (-90.0 - 60.0 * i) * M_PI / 180.0;
			double x = cx + 150.0 * cos(t);
			double y = cy + 150.0 * sin(t);
			Uint32 c = Bullet::get_color(i);
			if (subspace[i])
				draw_circle(screen, c, x, y, 20);
			else
				draw_circle(screen, c, x, y, 20, 2);
		}
	}

	double iw = screen->get_width() - level->dims[2];
	double ih = level->dims[3];
	double ix = level->dims[2];
	double iy = 0;
	int n = (level->level - 1) / 4;
	if (level->level == 25) n = 5;
	if (level->level > 25) n = 6;
	if (n < 0) n = 0;
	char filename[80];
	snprintf(filename, 80, "interface%d.png", n);
	Surface *background = rm->get_image(filename);
	double *bounds = background->get_rect();
	screen->blit(background, bounds[0] + ix, bounds[1] + iy);

	/* draw score */
	double sx = 0;
	double sy = 0;
	sx += ix;
	sy += iy;
	double sw = 8 * rm->number_dims()[2];
	double sh = rm->number_dims()[3];
	sx += 0.5 * (iw - sw);
	sy += 0.5 * (iw - sw);
	rm->draw_number(screen, score, (double[]){sx, sy}, 8);

	/* draw lives */
	double pw = 0, ph = 0, px = 0;
	double py = sy + sh + iy;
	int lives = 5;
    
	/* draw health */
	Uint32 color = mapRGB(255, 0, 0);
	double hx = ix + 0.1 * iw;
	double hy = iy + 100;
	double hw = 0.8 * bounds[2];
	double hh = 20;
	draw_rect(screen, color, (double[]){hx, hy, hw, hh}, 1);
	double hgap = 5.0;
	double hgw = hw - hgap * (max_health + 1);
	double hrw = (1.0 * hgw) / max_health;
	/*
	  for (int i = 0; i < health; i++)
	  draw_rect(screen, color,
	  (double[]){hx + hgap + (hgap + hrw) * i, hy, hrw, hh});
	*/
	for (int i = 0; i < health; i++)
	{
		double lx1 = hx + hgap + (hgap + hrw) * i;
		double lx2 = lx1 + hrw - 1;
		double ly = hy + 0.5 * hh;
		draw_laser(screen, color, (double[]){lx1, ly},
		           (double[]){lx2, ly}, 0.5 * hh);
	}

	/* draw shield */
	color = mapRGB(0, 0, 255);
	if (shield <= 100)
		color = mapRGB(255, 255, 255);
	sx = ix + 0.1 * iw;
	sy = iy + 140;
	sw = 0.8 * bounds[2];
	sh = 20;
	draw_rect(screen, color, (double[]){sx, sy, sw, sh}, 1);
	if (shield > 0)
	{
		double p = (1.0 * shield) / max_shield;
		//draw_rect(screen, color, (double[]){sx, sy, p * sw, sh});
		draw_laser(screen, color, (double[]){sx, sy + 0.5 * sh},
		           (double[]){sx + p * sw - 1, sy + 0.5 * sh}, 0.5 * sh);
	}

	/* draw energy */
	color = mapRGB(255, 255, 255);
	if (energy >= required_energy || num_subspace >= 6)
	{
		color = map_hue(energy_hue);
		energy_hue = (energy_hue + 6) % 360;
	}
	double ex = ix + 0.1 * iw;
	double ey = iy + 180;
	double ew = 0.8 * bounds[2];
	double eh = 20;
	draw_rect(screen, mapRGB(0, 0, 0),
	          (double[]){ex, ey, ew, eh}, 2);
	if (energy > 0)
	{
		double p = (1.0 * energy) / (2.0 * required_energy);
		if (p > 1.0) p = 1.0;
		if (energy < required_energy && num_subspace < 6)
			draw_rect(screen, color,
			          (double[]){ex, ey, p * ew, eh});
		else
		{
			for (int i = 0; i < (int)(p * ew); i++)
			{
				color = map_hue((energy_hue + 360 - i) % 360);
				draw_line(screen, color,
				          (double[]){ex + i, ey},
				          (double[]){ex + i, ey + eh - 1});
			}
		}
	}
	draw_rect(screen, mapRGB(0, 0, 0),
	          (double[]){ex + 0.5 * ew - 1, ey, 3, eh});

	/* draw bombs */
	int bomb = min(bombs, 12);
	Surface *icon = rm->get_image("bomb.png");
	n = 6;
	double space = 3;
	double bw = icon->get_rect()[2];
	double bh = icon->get_rect()[3];
	double bx = (iw - (n * bw + space * (n - 1))) / 2;
	double by = bx + py + (ph + space) * ((int)((lives - 1) / 5) + 1);
	bx += ix;
	by += iy;
	for (int i = 0; i < bomb; i++)
	{
		screen->blit(icon,
		             bx + (i % n) * (bw + space),
		             by + (int)((i / n)) * (bh + space));
	}

	/* draw level */
	if (level->level > 0)
	{
		char level_str[80];
		snprintf(level_str, 80, "Level: %d", level->level);
		if (level->subspace)
			snprintf(level_str, 80, "??");
		draw_string(screen, white, level_str, 5, 25);

		char blank_str[80];
		snprintf(blank_str, 80, "Blanks: %d", blank);
		draw_string(screen, white, blank_str, 5, 45);

		char energy_str[80];
		snprintf(energy_str, 80, "Energy needed: %d", required_energy);
		//draw_string(screen, white, energy_str, 5, 65);

		char combo_str[80];
		snprintf(combo_str, 80, "Combo: %d", combo);
		draw_string(screen, white, combo_str, 5, 65);

		char size_str[80];
		snprintf(size_str, 80, "sfx: %d", rm->get_size());
		draw_string(screen, white, size_str, 5, 85);
	}

	/* draw upgrades */
	Surface *empty = rm->get_image("up_empty.png");
	double uw = 4 * (4 + empty->get_rect()[2]);
	double uh = 6 * (4 + empty->get_rect()[3]);
	double ux = ix + 0.5 * (iw - uw) + 2;
	double uy = iy + (ih - uh) - 0.5 * (iw - uw);
	Surface *im = NULL;
	bounds = (double[]){0, 0, 0, 0};
	for (int i = 0; i < 6; i++)
	{
		snprintf(filename, 80, "up%d.png", i);
		im = rm->get_image(filename);
		cpy_rect(bounds, im->get_rect());
		bounds[1] = i * (bounds[3] + 4);
		bounds[1] += uy;
		for (int j = 0; j < 4; j++)
		{
			bounds[0] = j * (bounds[2] + 4);
			bounds[0] += ux;
			if (j < (int)(upgrades[i]))
				screen->blit(im, bounds[0], bounds[1]);
			else
				screen->blit(empty, bounds[0], bounds[1]);
		}
		if (fabs((upgrades[i] - (int)(upgrades[i])) - 0.5) < 0.01)
		{
			snprintf(filename, 80, "ups%d.png", i);
			Surface *ims = rm->get_image(filename);
			if (upgrades[i] >= 1)
			{
				int u = (int)(upgrades[i]);
				bounds[0] = (u - 1) * (bounds[2] + 4);
				bounds[0] += 4 + bounds[2];
			}
			else
				bounds[0] = 0;
			bounds[0] += 0.5 * bounds[2] - 0.5 * ims->get_rect()[2];
			bounds[1] += 0.5 * bounds[3] - 0.5 * ims->get_rect()[3];
			bounds[0] += ux;
			screen->blit(ims, bounds[0], bounds[1]);
		}
	}
	n = cur_color;
	bounds[0] = -2;
	bounds[1] = n * (im->get_rect()[3] + 4) - 2;
	bounds[2] = 16 + 4 * im->get_rect()[2];
	bounds[3] = 4 + im->get_rect()[3];
	bounds[0] += ux;
	bounds[1] += uy;
	draw_rect(screen, white, bounds, 2);
}
