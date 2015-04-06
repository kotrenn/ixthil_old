#ifndef MENU_H
#define MENU_H

#include "inputmanager.h"
#include "resourcemanager.h"

class Level;

class Menu
{
public:
	Menu(Level *l);
	~Menu();

	virtual void key_pressed(int key);
	virtual void joy_pressed(int button);
	virtual void process_input();
	virtual char *get_selection();
	virtual void finish();
	virtual void update();
	virtual void draw(Surface *screen);
	
	int cur;
	Menu *parent;
	bool hide_title;
protected:
	void init_memory(int n);
	void add_option(const char *option);
	
	Level *level;
	ResourceManager *rm;
	InputManager *im;
	Uint8 *keys;
	int num_options;
	char **options;
	bool *show;
	int select_delay;
	Surface *icon;
	double mx;
	double my;
};

class MainMenu : public Menu
{
public:
	MainMenu(Level *l);

	void process_input();
};

class StageSelect : public Menu
{
public:
	StageSelect(Level *l);

	void process_input();
private:
	int num_stages;
	int num_crystals;
};

#endif
