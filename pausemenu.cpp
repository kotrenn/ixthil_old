#include "pausemenu.h"
#include "level.h"
#include "options.h"

PauseMenu::PauseMenu(Level *l)
	:Menu(l)
{
	init_memory(3);
	add_option("Return to Game");
	add_option("Options");
	add_option("Return to Title");
	level->pause();
}

void PauseMenu::key_pressed(int key)
{
	Menu::key_pressed(key);

	if (key == IM_MENU || key == IM_PAUSE)
		level->exit_menu();
}

void PauseMenu::process_input()
{
	if (strcmp(get_selection(), "Return to Game") == 0)
	{
		level->exit_menu();
		level->pause();
	}
	if (strcmp(get_selection(), "Options") == 0)
		new OptionsMenu(level);
	if (strcmp(get_selection(), "Return to Title") == 0)
		level->end(0);
}

