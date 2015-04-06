#include "menu.h"
#include "level.h"

Menu::Menu(Level *l)
{
	level = l;
	rm = ResourceManager::get_instance();
	im = InputManager::get_instance();
	keys = SDL_GetKeyState(NULL);
	num_options = 0;
	cur = 0;
	select_delay = 0;
	icon = rm->get_image("player.png");
	parent = level->menu;
	level->menu = this;
	mx = 0.5 * level->dims[2] - 55;
	my = 0.4 * level->dims[3];
	hide_title = false;
}

Menu::~Menu()
{
	if (num_options > 0)
	{
		for (int i = 0; i < num_options; i++)
			free(options[i]);
		free(options);
		free(show);
	}
}

void Menu::init_memory(int n)
{
	num_options = n;
	options = (char **)malloc(sizeof(char *) * num_options);
	for (int i = 0; i < num_options; i++)
		options[i] = (char *)malloc(sizeof(char *) * 80);
	show = (bool *)malloc(sizeof(bool) * num_options);
	for (int i = 0; i < num_options; i++)
		show[i] = true;
}

void Menu::add_option(const char *option)
{
	strncpy(options[cur], option, 80);
	cur = (cur + 1) % num_options;
}

void Menu::key_pressed(int key)
{
	if (im->is_pressed(IM_SHOOT) ||
	    im->is_pressed(IM_PAUSE))
		process_input();
}

void Menu::joy_pressed(int button)
{
	if (im->is_pressed(IM_SHOOT) ||
	    im->is_pressed(IM_PAUSE))
		process_input();
}

void Menu::process_input()
{
}

char *Menu::get_selection()
{
	return options[cur];
}

void Menu::finish()
{
	level->exit_menu();
}

void Menu::update()
{
	bool done;
	if (select_delay == 0)
	{
		if (im->is_pressed(IM_UP))
		{
			done = false;
			while (!done)
			{
				cur = (cur + num_options - 1) % num_options;
				done = show[cur];
			}
			select_delay = 5;
		}
		if (im->is_pressed(IM_DOWN))
		{
			done = false;
			while (!done)
			{
				cur = (cur + num_options + 1) % num_options;
				done = show[cur];
			}
			select_delay = 5;
		}
	}
	else
		select_delay--;
}

void Menu::draw(Surface *screen)
{
	Uint32 white = mapRGB(255, 255, 255);
	int c = 0;
	for (int i = 0; i < num_options; i++)
	{
		if (i == cur)
			screen->blit(icon, mx - 30, my + 20 * c);
		if (show[i])
		{
			fflush(stdout);
			draw_string(screen, white, options[i], mx, my + 20 * c);
			c++;
		}
	}
}



#include "options.h"

MainMenu::MainMenu(Level *l)
	:Menu(l)
{
	init_memory(6);
	add_option("New Game");
	add_option("Extra Stage");
	add_option("Endless");
	add_option("How to Play");
	add_option("Options");
	add_option("Exit");
	show[1] = show[2] = false;
}

void MainMenu::process_input()
{
	if (strcmp(get_selection(), "New Game") == 0)
		new StageSelect(level);
	if (strcmp(get_selection(), "Options") == 0)
		new OptionsMenu(level);
	if (strcmp(get_selection(), "Exit") == 0)
		level->exit_menu();
}



StageSelect::StageSelect(Level *l)
	:Menu(l)
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

	init_memory(num_stages + 1);
	char buf[80];
	for (int i = 1; i <= num_stages; i++)
	{
		if (i == 7)
			snprintf(buf, 80, "Extra Stage");
		else if (i == 8)
			snprintf(buf, 80, "Super Extra Stage");
		else
			snprintf(buf, 80, "Stage %d", i);
		add_option(buf);
	}
	add_option("Back");
}

void StageSelect::process_input()
{
	PlayerShip *player = level->player;

	int stage = 0;
	if (strcmp(get_selection(), "Stage 1") == 0) stage = 1;
	if (strcmp(get_selection(), "Stage 2") == 0) stage = 2;
	if (strcmp(get_selection(), "Stage 3") == 0) stage = 3;
	if (strcmp(get_selection(), "Stage 4") == 0) stage = 4;
	if (strcmp(get_selection(), "Stage 5") == 0) stage = 5;
	if (strcmp(get_selection(), "Stage 6") == 0) stage = 6;
	if (strcmp(get_selection(), "Extra Stage") == 0)
		stage = 7;
	if (strcmp(get_selection(), "Super Extra Stage") == 0)
		stage = 8;
	if (stage == 0)
	{
		level->exit_menu();
		return;
	}

	int next_level = 4 * (stage - 1) + 1;
	if (stage == 7) next_level = 26;
	if (stage == 8) next_level = 31;
	int blank = min(4 * (stage - 1), 8);
	int num_subspace = 0;
	if (stage > 6)
		num_subspace = num_crystals;

	level->end(next_level);
	player->demo = false;
	player->reset();
	player->blank = blank;
	player->num_subspace = num_subspace;
	for (int i = 0; i < num_subspace; i++)
		player->subspace[i] = true;
}
