#include "level.h"

#include "dark.h"
#include "enemies.h"

#include "quickship.h"
#include "teleportership.h"
#include "ballos.h"
#include "bigship.h"
#include "boss5.h"
#include "boss6.h"

#include "slideship.h"
#include "orbitship.h"
#include "colorship.h"
#include "surroundship.h"
#include "spinship.h"
#include "stateship.h"

#include "credits.h"

Level::Level(Background *bg, int l, int w, int h)
{
	background = bg;
	set_rect(dims, 0, 0, w, h);
	rm = ResourceManager::get_instance();
	paused = false;
	level = l;
	next_level = level + 1;
	done = false;
	portal = false;
	portal_created = false;
	subspace = false;
	boss = NULL;
	set_pair(prev_rand, -1, -1);
	menu = NULL;
	old_menu = NULL;
	player = PlayerShip::get_instance(this);
	player->level = this;
	add_enemies();
}

Level::~Level()
{
	if (menu != NULL)
		delete menu;
	menu = NULL;
}

void Level::add_enemies()
{
	CloudManager *cm = CloudManager::get_instance();
	if (level == 0)
	{
		rm->play_music("title.mp3");
		player->demo = true;
		player->reset();
		ships.push_back(player);
		random_ships();
		new MainMenu(this);
	}
	else if (level == -1) // win screen
	{
		int foo = -1;
	}
	else if (level == -2) // help screen
	{
		player->reset();
		ships.push_back(player);
		/*new HelpScreen(this);*/
	}
	else if (level == -3) // credits
	{
		rm->play_music("credits.mp3");
		ships.push_back(player);
		new Credits(this);
	}
	else
	{
		ships.push_back(player);
		player->demo = false;
		if (level == 1)
		{
			rm->play_music("music1.mp3");
			//player->reset();
			player->required_energy = 30;
			for (int i = 0; i < 10; i++)
			{
				new SimpleShip(this);
				//new BlackHole(this);
			}
		}
		else if (level == 2)
		{
			for (int i = 0; i < 10; i++)
				new SimpleShip(this);
			for (int i = 0; i < 5; i++)
				new StrongShip(this);
		}
		else if (level == 3)
		{
			portal = true;
			for (int i = 0; i < 5; i++)
			{
				new SimpleShip(this);
				new StrongShip(this);
				new AttractorShip(this);
			}
		}
		else if (level == 4)
		{
			rm->play_music("boss1.mp3");
			new QuickShip(this);
		}
		else if (level == 5)
		{
			rm->play_music("music1.mp3");
			for (int i = 0; i < 10; i++)
				new AttractorShip(this);
		}
		else if (level == 6)
		{
			for (int i = 0; i < 10; i++)
				new AttractorShip(this);
			for (int i = 0; i < 5; i++)
				new SmartShip(this);
		}
		else if (level == 7)
		{
			portal = true;
			for (int i = 0; i < 8; i++)
				new SmartShip(this);
			for (int i = 0; i < 4; i++)
				new BounceShip(this);
		}
		else if (level == 8)
		{
			rm->play_music("boss2.mp3");
			new TeleporterShip(this);
		}
		else if (level == 9)
		{
			rm->play_music("music2.mp3");
			cm->set_center((double[]){0.5 * dims[2], -50});
			for (int i = 0; i < 10; i++)
				new CloudShip(this);
		}
		else if (level == 10)
		{
			cm->set_center((double[]){0.5 * dims[2], -50});
			for (int i = 0; i < 4; i++)
			{
				new BounceShip(this);
				new CloudShip(this);
				new CloudShip(this);
				new CloudShip(this);
				new CloudShip(this);
			}
		}
		else if (level == 11)
		{
			portal = true;
			for (int i = 0; i < 6; i++)
				new GroupCreator(this);
			cm->set_center((double[]){0.2 * dims[2], -200});
			for (int i = 0; i < 15; i++)
				new CloudShip(this);
			cm->set_center((double[]){0.8 * dims[2], -200});
			for (int i = 0; i < 15; i++)
				new CloudShip(this);
		}
		else if (level == 12)
		{
			rm->play_music("boss3.mp3");
			new Ballos(this);
		}
		else if (level == 13)
		{
			rm->play_music("music2.mp3");
			for (int i = 0; i < 5; i++)
				new DizzyShip(this);
		}
		else if (level == 14)
		{
			for (int i = 0; i < 6; i++)
				new DizzyShip(this);
			cm->set_center((double[]){0.2 * dims[2], -200});
			for (int i = 0; i < 12; i++)
				new CloudShip(this);
			cm->set_center((double[]){0.8 * dims[2], -200});
			for (int i = 0; i < 12; i++)
				new CloudShip(this);
		}
		else if (level == 15)
		{
			portal = true;
			for (int i = 0; i < 6; i++)
			{
				new DizzyShip(this);
				new GroupCreator(this);
			}
		}
		else if (level == 16)
		{
			rm->play_music("boss4.mp3");
			new BigShip(this);
		}
		else if (level == 17)
		{
			rm->play_music("music3.mp3");
			cm->set_center((double[]){0.5 * dims[2], -200});
			for (int i = 0; i < 5; i++)
				new CrystalShip(this);
			for (int i = 0; i < 15; i++)
				new CloudShip(this);
		}
		else if (level == 18)
		{
			for (int i = 0; i < 6; i++)
			{
				new CrystalShip(this);
				new SmartShip(this);
				new AttractorShip(this);
			}
		}
		else if (level == 19)
		{
			portal = true;
			for (int i = 0; i < 3; i++)
			{
				new CrystalShip(this);
				new DizzyShip(this);
				new BottomShip(this);
				new BottomShip(this);
			}
		}
		else if (level == 20)
		{
			rm->play_music("boss5.mp3");
			new MysteryShip(this);
		}
		else if (level == 21)
		{
			rm->play_music("music3.mp3");
			for (int i = 0; i < 5; i++)
			{
				new CrystalShip(this);
				new BottomShip(this);
				new SmartShip(this);
				new SmartShip(this);
			}
		}
		else if (level == 22)
		{
			random_ships();
			random_ships();
		}
		else if (level == 23)
		{
			portal = true;
			random_ships();
			random_ships();
			random_ships();
		}
		else if (level == 24)
		{
			rm->play_music("music3.mp3");
			for (int i = 0; i < 100; i++)
				new SimpleShip(this);
		}
		else if (level == 25)
		{
			rm->play_music("boss6.mp3");
			new PrismShip(this);
			next_level = -3; // credits;
			if (player->num_subspace >= 3)
				next_level = 26;
		}
		else if (level == 26)
		{
			rm->play_music("music4.mp3");
			for (int i = 0; i < 5; i++)
			{
				new HomerShip(this);
				new SpinnerShip(this);
			}
		}
		else if (level == 27)
		{
			for (int i = 0; i < 5; i++)
			{
				new DodgeShip(this);
				new WaveShip(this);
			}
		}
		else if (level == 28)
		{
			for (int i = 0; i < 5; i++)
			{
				new AssassinShip(this);
				new SneakShip(this);
			}
		}
		else if (level == 29)
		{
			int n = randint(1, 4);
			for (int i = 0; i < 5; i++)
			{
				switch (n)
				{
				case 1:
					new AssassinShip(this);
					new HomerShip(this);
					new SpinnerShip(this);
					break;
				case 2:
					new DodgeShip(this);
					new SneakShip(this);
					new WaveShip(this);
					break;
				case 3:
					new AssassinShip(this);
					new SneakShip(this);
					new WaveShip(this);
					break;
				case 4:
					new DodgeShip(this);
					new HomerShip(this);
					new SpinnerShip(this);
					break;
				}
			}
		}
		else if (level == 30)
		{
			rm->play_music("boss6.mp3");
			new PrismShip(this);
			next_level = -3;
		}
		else
			for (int i = 0; i < level - 24; i++)
				random_ships();
	}
}

void Level::create_portal()
{
	portal_created = true;
	new PortalItem(this, player->num_subspace);
}

void Level::enter_subspace(PortalItem *portal)
{
	subspace = true;
	double point[2];
	point[0] = portal->dims[0] + 0.5 * portal->dims[2];
	point[1] = portal->dims[1] + 0.5 * portal->dims[3];
	background->enter_subspace(point, portal->get_color());
	rm->play_music("subspace.mp3");

	int n = player->num_subspace;
	if (n == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			new DarkSimpleShip(this);
			new BackAndForthShip(this);
			new DarkAttractorShip(this);
		}
		new SlideShip(this);
	}
	else if (n == 1)
	{
		for (int i = 0; i < 2; i++)
		{
			new SplitShip(this);
			new DarkSmartShip(this);
			new DarkSmartShip(this);
		}
		new OrbitShip(this);
	}
	else if (n == 2)
	{
		for (int i = 0; i < 4; i++)
		{
			new BugShip(this);
			new BugShip(this);
			new OctaGroupCreator(this);
		}
		new ColorShip(this);
	}
	else if (n == 3)
	{
		for (int i = 0; i < 2; i++)
		{
			new BugShip(this);
			new TriangleShip(this);
		}
		new SurroundManager(this);
	}
	else if (n == 4)
	{
		for (int i = 0; i < 2; i++)
		{
			new TriangleShip(this);
			new NetworkShip(this);
			new VerticalShip(this);
		}
		new SpinShip(this);
	}
	else if (n == 5)
	{
		for (int i = 0; i < 3; i++)
		{
			new NetworkShip(this);
			new VerticalShip(this);
			new VerticalShip(this);
		}
		new StateShip(this);
	}
}

void Level::exit_subspace()
{
	int levels[] = {30, 50, 100, 200, 300, 280, 500};
	player->subspace[player->num_subspace] = true;
	player->num_subspace++;
	player->required_energy = levels[player->num_subspace];
	background->enter_subspace(NULL);
}

void Level::end(int next_l)
{
	next_level = next_l;
	done = true;
}

void Level::exit_menu()
{
	old_menu = menu;
	menu = menu->parent;
	if (menu != NULL)
		menu->cur = 0;
}

void Level::random_ships()
{
	CloudManager *cm = CloudManager::get_instance();
	cm->set_center((double[]){0.5 * dims[2], -50});
	int s;
	bool done = false;
	while (!done)
	{
		done = true;
		s = randint(1, 8);
		for (int i = 0; i < 2; i++)
			if (s == prev_rand[i])
				done = false;
	}
	if (randint(1, 5) > 1 || level > 0)
	{
		int n = randint(5, 10);
		for (int i = 0; i < n; i++)
		{
			if (s == 1) new AttractorShip(this);
			if (s == 2) new SmartShip(this);
			if (s == 3) new BounceShip(this);
			if (s == 4) new CloudShip(this);
			if (s == 4) new CloudShip(this);
			if (s == 5) new GroupCreator(this);
			if (s == 6) new DizzyShip(this);
			if (s == 7) new CrystalShip(this);
			if (s == 8) new BottomShip(this);
		}
		for (int i = 0; i < 1; i++)
			prev_rand[i + 1] = prev_rand[i];
		prev_rand[0] = s;
	}
	else
	{
		s = randint(1, 4);
		if (s == 1) new QuickShip(this);
		if (s == 2) new TeleporterShip(this);
		if (s == 3) new Ballos(this);
		if (s == 4) new BigShip(this);
	}
}

void Level::copy(Level *old)
{
	list<Actor *>::iterator a;
	for (a = old->get_actors()->begin();
	     a != old->get_actors()->end(); a++)
	{
		add_actor(*a);
		(*a)->level = this;
	}
	for (a = old->get_add_actor_list()->begin();
	     a != old->get_add_actor_list()->end(); a++)
	{
		add_actor(*a);
		(*a)->level = this;
	}
	list<Bullet *>::iterator b;
	for (b = old->get_bullets()->begin();
	     b != old->get_bullets()->end(); b++)
	{
		add_bullet(*b);
		(*b)->level = this;
	}
	for (b = old->get_add_bullet_list()->begin();
	     b != old->get_add_bullet_list()->end(); b++)
	{
		add_bullet(*b);
		(*b)->level = this;
	}
	list<Item *>::iterator i;
	for (i = old->get_items()->begin();
	     i != old->get_items()->end(); i++)
	{
		add_item(*i);
		(*i)->level = this;
	}
	for (i = old->get_add_item_list()->begin();
	     i != old->get_add_item_list()->end(); i++)
	{
		add_item(*i);
		(*i)->level = this;
	}
}

void Level::pause()
{
	if (level <= 0)
		return;
	paused = !paused;
}

bool out_of_bounds(const Actor *a)
{
	Level *level = a->level;
	double bx = level->dims[0] - 0.1 * level->dims[2];
	double by = level->dims[1] - 0.1 * level->dims[3];
	double bw = 1.2 * level->dims[2];
	double bh = 1.2 * level->dims[3];
	return (
		a->dims[0] + a->dims[2] < bx ||
		a->dims[1] + a->dims[3] < by ||
		a->dims[0] > bw ||
		a->dims[1] > bh);
}

void Level::update()
{
	list<Actor *>::iterator a;
	list<Bullet *>::iterator b;
	list<Ship *>::iterator s;
	list<Item *>::iterator i;
    
	if (menu != NULL)
		menu->update();

	if (paused)
		return;

	for (b = bullets.begin(); b != bullets.end(); b++)
		background->add_value((*b)->color, (*b)->v,
		                      (*b)->dims[0] + (*b)->dims[2] / 2,
		                      (*b)->dims[1] + (*b)->dims[3] / 2);
	for (s = ships.begin(); s != ships.end(); s++)
	{
		int c = (*s)->color;
		if (c >= 0)
			background->add_value((*s)->color, (*s)->v,
			                      (*s)->dims[0] + (*s)->dims[2] / 2,
			                      (*s)->dims[1] + (*s)->dims[3] / 2);
	}

	for (s = ships.begin(); s != ships.end(); s++)
		(*s)->hit = false;

	/*
	  tree = QuadTree(self);
	  for (b in bullets)
	  tree.insert(b);
	*/

	for (a = actors.begin(); a != actors.end(); a++)
		(*a)->update();
	for (b = bullets.begin(); b != bullets.end(); b++)
		(*b)->update();
	for (s = ships.begin(); s != ships.end(); s++)
		(*s)->update();
	for (i = items.begin(); i != items.end(); i++)
		(*i)->update();
    
	for (a = actors.begin(); a != actors.end(); a++)
		(*a)->move();
	for (b = bullets.begin(); b != bullets.end(); b++)
		(*b)->move();
	for (s = ships.begin(); s != ships.end(); s++)
		(*s)->move();
	for (i = items.begin(); i != items.end(); i++)
		(*i)->move();

	for (b = bullets.begin(); b != bullets.end(); b++)
		if (out_of_bounds(*b))
			remove_bullet(*b);
	for (i = items.begin(); i != items.end(); i++)
		if (out_of_bounds(*i))
			remove_item(*i);

	for (a = add_actor_list.begin(); a != add_actor_list.end(); a++)
		actors.push_back(*a);
	for (s = add_ship_list.begin(); s != add_ship_list.end(); s++)
		ships.push_back(*s);
	for (b = add_bullet_list.begin(); b != add_bullet_list.end(); b++)
		bullets.push_back(*b);
	for (i = add_item_list.begin(); i != add_item_list.end(); i++)
		items.push_back(*i);

	add_actor_list.clear();
	add_ship_list.clear();
	add_bullet_list.clear();
	add_item_list.clear();

	actors.sort();
	ships.sort();
	bullets.sort();
	items.sort();
	actors.unique();
	ships.unique();
	bullets.unique();
	items.unique();

	remove_actor_list.sort();
	remove_actor_list.unique();
	for (a = remove_actor_list.begin(); a != remove_actor_list.end(); a++)
	{
		actors.remove(*a);
		delete *a;
	}
	remove_ship_list.sort();
	remove_ship_list.unique();
	for (s = remove_ship_list.begin(); s != remove_ship_list.end(); s++)
	{
		ships.remove(*s);
		for (b = add_bullet_list.begin(); b != add_bullet_list.end(); b++)
			(*b)->ship_died(*s);
		for (b = bullets.begin(); b != bullets.end(); b++)
			(*b)->ship_died(*s);
		if (*s != player)
			delete *s;
	}
	remove_bullet_list.sort();
	remove_bullet_list.unique();
	for (b = remove_bullet_list.begin(); b != remove_bullet_list.end(); b++)
	{
		bullets.remove(*b);
		delete *b;
	}
	remove_item_list.sort();
	remove_item_list.unique();
	for (i = remove_item_list.begin(); i != remove_item_list.end(); i++)
	{
		items.remove(*i);
		(*i)->die();
		delete *i;
	}

	remove_actor_list.clear();
	remove_ship_list.clear();
	remove_bullet_list.clear();
	remove_item_list.clear();

	if (old_menu != NULL)
	{
		delete old_menu;
		old_menu = NULL;
	}
}

void Level::draw(Surface *screen)
{
	list<Actor *>::iterator a;
	list<Ship *>::iterator s;
	list<Bullet *>::iterator b;
	list<Item *>::iterator i;

	for (i = items.begin(); i != items.end(); i++)
		(*i)->draw(screen);
	for (a = actors.begin(); a != actors.end(); a++)
		(*a)->draw(screen);
	for (b = bullets.begin(); b != bullets.end(); b++)
		(*b)->draw(screen);
	for (s = ships.begin(); s != ships.end(); s++)
		(*s)->draw(screen);

	/*
	  if (draw_tree)
	  tree->draw(screen);
	*/

	if (menu != NULL)
		menu->draw(screen);

	if (level != 0 && level != -4)
	{
		if (boss != NULL)
		{
			if (boss->health > 0)
			{
				double p = boss->get_health();
				double w = 0.8 * dims[2];
				double h = 20;
				double x = 0.1 * dims[2];
				double y = 0.05 * dims[3];
				Uint32 color = mapRGB(255, 0, 0);
				draw_rect(screen, color, (double[]){x, y, w, h}, 1);
				draw_rect(screen, color, (double[]){x, y, p * w, h});
			}
		}
		player->draw_interface(screen);
	}

	bool hidden = false;
	if (menu != NULL)
		if (menu->hide_title)
			hidden = 1;
	if (level == 0 && !hidden)
	{
		Surface *title = rm->get_image("title.png");
		screen->blit(title, 0.5 * (dims[2] - title->get_width()), 100);
	}

	if (paused && level > 0)
	{
		Surface *pause = rm->get_image("paused.png");
		screen->blit(pause,
		             0.5 * (dims[2] - pause->get_width()),
		             0.5 * (dims[3] - pause->get_height()));
	}
}

void Level::remove_actor(Actor *actor)
{
	remove_actor_list.push_back(actor);
}

void Level::remove_ship(Ship *ship)
{
	remove_ship_list.push_back(ship);
}

void Level::remove_bullet(Bullet *bullet)
{
	remove_bullet_list.push_back(bullet);
}

void Level::remove_item(Item *item)
{
	remove_item_list.push_back(item);
}

list<Bullet *> *Level::get_collisions(Ship *ship)
{
	list<Bullet *> *hits = new list<Bullet *>();
	list<Bullet *>::iterator b;
	int count = 0;
	for (b = bullets.begin(); b != bullets.end(); b++)
	{
		count++;
		if (rect_collides((*b)->dims, ship->dims) &&
		    (*b)->team != ship->team)
			hits->push_back(*b);
	}
	return hits;
}

list<Actor *> *Level::get_actors()
{
	return &actors;
}

list<Bullet *> *Level::get_bullets()
{
	return &bullets;
}

list<Ship *> *Level::get_ships()
{
	return &ships;
}

list<Item *> *Level::get_items()
{
	return &items;
}

list<Actor *> *Level::get_add_actor_list()
{
	return &add_actor_list;
}

list<Bullet *> *Level::get_add_bullet_list()
{
	return &add_bullet_list;
}

list<Ship *> *Level::get_add_ship_list()
{
	return &add_ship_list;
}

list<Item *> *Level::get_add_item_list()
{
	return &add_item_list;
}

void Level::add_actor(Actor *actor)
{
	add_actor_list.push_back(actor);
}

void Level::add_ship(Ship *ship)
{
	add_ship_list.push_back(ship);
}

void Level::add_bullet(Bullet *bullet)
{
	add_bullet_list.push_back(bullet);
}

void Level::add_item(Item *item)
{
	add_item_list.push_back(item);
}
