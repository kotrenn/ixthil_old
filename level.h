class Level;

#ifndef LEVEL_H
#define LEVEL_H

#include "resourcemanager.h"

#include "background.h"
#include "actor.h"
#include "ship.h"
#include "bullet.h"
#include "item.h"
#include "menu.h"

#include "player.h"

#include "enemies.h"
#include "quickship.h"

class Actor;

class Level
{
public:
	Level(Background *bg, int l, int w, int h);
	~Level();

	void end(int next_l);
	void exit_menu();
	void create_portal();
	void enter_subspace(PortalItem *portal);
	void exit_subspace();
	void random_ships();
	void copy(Level *old);
	void pause();

	void update();
	void draw(Surface *screen);

	void remove_actor(Actor *actor);
	void remove_ship(Ship *ship);
	void remove_bullet(Bullet *bullet);
	void remove_item(Item *item);
	list<Bullet *> *get_collisions(Ship *ship);
	list<Actor *> *get_actors();
	list<Bullet *> *get_bullets();
	list<Ship *> *get_ships();
	list<Item *> *get_items();
	list<Actor *> *get_add_actor_list();
	list<Bullet *> *get_add_bullet_list();
	list<Ship *> *get_add_ship_list();
	list<Item *> *get_add_item_list();
	void add_actor(Actor *actor);
	void add_ship(Ship *ship);
	void add_bullet(Bullet *bullet);
	void add_item(Item *item);
	
	double dims[4];
	Background *background;
	bool paused;
	int level;
	int next_level;
	bool done;
	bool portal;
	bool portal_created;
	bool subspace;
	Ship *boss;
	Menu *menu;
	PlayerShip *player;
private:
	ResourceManager *rm;
	
	list<Actor *> actors;
	list<Bullet *> bullets;
	list<Ship *> ships;
	list<Item *> items;
	list<Actor *> add_actor_list;
	list<Ship *> add_ship_list;
	list<Bullet *> add_bullet_list;
	list<Item *> add_item_list;
	list<Actor *> remove_actor_list;
	list<Ship *> remove_ship_list;
	list<Bullet *> remove_bullet_list;
	list<Item *> remove_item_list;
	int prev_rand[2];
	Menu *old_menu;

	void add_enemies();
};

#endif
