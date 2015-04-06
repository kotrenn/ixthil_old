#ifndef OPTIONS_H
#define OPTIONS_H

#include "menu.h"

class OptionsMenu : public Menu
{
    public:
        OptionsMenu(Level *l);
	~OptionsMenu();

	void key_pressed(int key);
	void joy_pressed(int button);

	void update();
	void draw(Surface *screen);
    private:
	char **input_choices;
	int *buttons;
	int selecting;
	int select_timer;
};

#endif
