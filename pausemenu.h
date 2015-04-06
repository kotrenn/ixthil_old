#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "menu.h"

class PauseMenu : public Menu
{
    public:
        PauseMenu(Level *l);

	void key_pressed(int key);
	void process_input();
};

#endif
