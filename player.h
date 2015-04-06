class PlayerShip;

#ifndef PLAYER_H
#define PLAYER_H

#include "actor.h"
#include "bullet.h"
#include "ship.h"
#include "level.h"

class PlayerShip : public Ship
{
    public:
        static PlayerShip *get_instance(Level *l);

	~PlayerShip();
	
	void reset();
	void damage(int pain, int c);
	void die();
	void update();
	void move();
	void bullet_hit();
	void create_bullet();
	void draw(Surface *screen);
	void draw_interface(Surface *screen);

	void cheat();
	
	bool demo;
	int cur_color;
	int bullets;
	int bullet_hits;
	int blank;
	int energy;
	int required_energy;
	bool energizing;
	bool subspace[6];
	int num_subspace;
    private:
	PlayerShip(Level *l);
	static PlayerShip *instance;

	int fire_timer;
	int fire_delay;
	double upgrades[6];
	int blank_timer;
	int max_health;
	double shield;
	double max_shield;
	int invincible_timer;
	int bombs;
	int bomb_rad;
	double bomb_max;
	int energy_select;
	int energy_hue;
	int energy_timer;
	int energy_theta;
	int energy_rad;
	int energy_bullet_timer;
	int laser_timer;
	int select_delay;
	double tx;
	double ty;
	int combo;
};

#endif
