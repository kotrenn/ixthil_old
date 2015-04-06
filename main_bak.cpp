#include "common.h"
#include "surface.h"
#include "background.h"
#include "level.h"
#include "resourcemanager.h"
#include "pausemenu.h"

int main(int argc, char **argv)
{
    srand(time(0));

    double size[] = {640, 480};
    double width = size[0] - 140, height = size[1];
    Uint32 black = mapRGB(0, 0, 0);
    Uint32 white = mapRGB(255, 255, 255);
    Uint32 clear = mapRGBA(0, 0, 0, 0);
    bool fullscreen = false;
    InputManager *im = InputManager::get_instance();

    //SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    //Note: maybe enable this in OptionsMenu only
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

    int displayFlags = SDL_ANYFORMAT | SDL_HWSURFACE;
    Surface *main_screen = new Surface(SDL_SetVideoMode(size[0], size[1],
							32, displayFlags));

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
    {
	fprintf(stderr, "Could not open audio channels\n");
	SDL_Quit();
    }

    SDL_WM_SetCaption("Ixthil", 0);

    SDL_Event event;

    Surface *screen = new Surface(640, 480);

    Background *bg = new Background((double[]){0, 0, size[0], size[1]});
    Level *level = new Level(0, size[0], size[1]);
    PlayerShip *player = PlayerShip::get_instance(level);
    //level->remove_ship(player);

    int frames_per_second = 30;
    int frame = 0;
    bool cap = true;
    Uint32 total = SDL_GetTicks();
    Uint32 fps;

    bool running = true;
    while (running)
    {
	if (SDL_GetTicks() - total >= 1000)
	{
	    total = SDL_GetTicks();
	    frame = 0;
	}
	fps = SDL_GetTicks();
	
	while (SDL_PollEvent(&event))
	{
	    if (event.type == SDL_QUIT)
		running = 0;
	    else if (event.type == SDL_JOYBUTTONDOWN)
	    {
		if (level->menu != NULL)
		    level->menu->joy_pressed(event.jbutton.button);
	    }
	    else if (event.type == SDL_KEYDOWN)
	    {
		if (level->menu != NULL)
		    level->menu->key_pressed(event.key.keysym.sym);
		if (event.key.keysym.sym == SDLK_f)
		{
		    int flags = displayFlags;
		    fullscreen = !fullscreen;
		    if (fullscreen)
			flags |= SDL_FULLSCREEN;
		    delete main_screen;
		    SDL_Surface *ms = SDL_SetVideoMode(size[0], size[1], 32,
						       flags);
		    main_screen = new Surface(ms);
		}
		if (event.key.keysym.sym == SDLK_o)
		    cap = !cap;
	    }
	    if (im->is_pressed(IM_MENU))
	    {
		if (level->level == 0)
		    int foo = 0;
		else if (!level->paused)
		    new PauseMenu(level);
	    }
	    else if (level->level > 0)
	    {
		if ((im->is_pressed(IM_SHOOT) ||
		     im->is_pressed(IM_PAUSE)) &&
		    player->health <= 0)
		{
		    Level *old = level;
		    delete old;
		    level = new Level(0, size[0], size[1]);
		}
	    }
	    if (level->level == 0 && level->menu == NULL)
		running = false;
	}

	bg->update();
	level->update();

	//main_screen->fill(green);

	screen->fill(level->background);

	bg->draw(screen);
	level->draw(screen);

	frames_per_second = 30;
	if (player->energizing)
	    frames_per_second = 30;

	char fps_string[80];
	snprintf(fps_string, 80, "fps: %lf",
		 frame / ((SDL_GetTicks() - total) / 1000.0));
	draw_string(screen, white, fps_string, 5, 5);

	main_screen->blit(screen, 0, 0);
	main_screen->flip();

	if (cap && ((int)(SDL_GetTicks() - fps) < 1000.0 / frames_per_second))
	{
	    int delay = (1000 / frames_per_second) - (SDL_GetTicks() - fps);
	    if (delay > 0)
		SDL_Delay(delay);
	    //SDL_Delay((1000 / frames_per_second) - (SDL_GetTicks() - fps));
	}
	frame++;

	if (level->get_ships()->size() == 1 &&
	    level->get_items()->size() == 0 &&
	    level->get_add_ship_list()->size() == 0 &&
	    level->get_add_item_list()->size() == 0 &&
	    player->health > 0)
	{
	    if (level->level != 0 && level->level != -4)
	    {
		if (level->next_level != -1)
		{
		    if (level->portal && !level->subspace &&
			player->energy >= player->required_energy &&
			!level->portal_created)
			level->create_portal();
		    else
		    {
			Level *old = level;
			if (old->next_level == 0)
			    level = new Level(0, size[0], size[1]);
			else
			    level = new Level(old->next_level, width, height);
			level->copy(old);
			if (old->subspace)
			    old->exit_subspace();
			delete old;
		    }
		}
		else
		{
		    delete level;
		    level = new Level(0, width, height);
		}
	    }
	    else
		level->random_ships();
	}
	if (level->done)
	{
	    Level *old = level;
	    if (old->next_level == 0)
		level = new Level(0, size[0], size[1]);
	    else
		level = new Level(old->next_level, width, height);
	    player->demo = (level->level == 0);
	    delete old;
	}
    }

    delete screen;
    delete main_screen;
    SDL_Quit();

    return 0;
}
