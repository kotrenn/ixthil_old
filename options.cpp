#include "options.h"
#include "level.h"

OptionsMenu::OptionsMenu(Level *l)
    :Menu(l)
{
    init_memory(13);
    add_option("Input");
    add_option("Shoot/Select");
    add_option("Bomb");
    add_option("Blank");
    add_option("Energy");
    add_option("Previous");
    add_option("Next");
    add_option("Up");
    add_option("Down");
    add_option("Left");
    add_option("Right");
    add_option("Pause");
    add_option("Menu");
    input_choices = (char **)malloc(sizeof(char *) * 2);
    for (int i = 0; i < 2; i++)
	input_choices[i] = (char *)malloc(sizeof(char) * 80);
    strcpy(input_choices[0], "Keyboard");
    strcpy(input_choices[1], "Joystick");
    buttons = (int *)malloc(sizeof(int) * num_options);
    buttons[0]  = -1;
    buttons[1]  = IM_SHOOT;
    buttons[2]  = IM_BOMB;
    buttons[3]  = IM_BLANK;
    buttons[4]  = IM_ENERGY;
    buttons[5]  = IM_PREV;
    buttons[6]  = IM_NEXT;
    buttons[7]  = IM_UP;
    buttons[8]  = IM_DOWN;
    buttons[9]  = IM_LEFT;
    buttons[10] = IM_RIGHT;
    buttons[11] = IM_PAUSE;
    buttons[12] = IM_MENU;
    select_delay = 5;
    selecting = false;
    select_timer = 0;
    hide_title = true;
}

OptionsMenu::~OptionsMenu()
{
    //Menu::~Menu();
    for (int i = 0; i < 2; i++)
	free(input_choices[i]);
    free(input_choices);
}

void OptionsMenu::key_pressed(int key)
{
    if (selecting)
    {
	selecting = false;
	if (buttons[cur] >= 0)
	    im->set_button(buttons[cur], key);
    }
    else if (key == SDLK_ESCAPE)
	finish();
}

void OptionsMenu::joy_pressed(int button)
{
    if (selecting)
    {
	selecting = false;
	if (buttons[cur] >= 0)
	    im->set_button(buttons[cur], button);
    }
}

void OptionsMenu::update()
{
    if (selecting)
    {
	select_timer--;
	if (select_timer == 0)
	    selecting = false;
    }
    else
    {
	bool done;
	if (select_delay == 0)
	{
	    if (keys[SDLK_UP])
	    {
		done = false;
		while (!done)
		{
		    cur += num_options - 1;
		    cur %= num_options;
		    done = true;
		    if (im->get_mode() == IM_JOYSTICK && (
			    buttons[cur] == IM_UP ||
			    buttons[cur] == IM_DOWN ||
			    buttons[cur] == IM_LEFT ||
			    buttons[cur] == IM_RIGHT))
			done = false;
		}
		select_delay = 5;
	    }
	    if (keys[SDLK_DOWN])
	    {
		done = false;
		while (!done)
		{
		    cur += num_options + 1;
		    cur %= num_options;
		    done = true;
		    if (im->get_mode() == IM_JOYSTICK && (
			    buttons[cur] == IM_UP ||
			    buttons[cur] == IM_DOWN ||
			    buttons[cur] == IM_LEFT ||
			    buttons[cur] == IM_RIGHT))
			done = false;
		}
		select_delay = 5;
	    }
	    if (keys[SDLK_LEFT])
	    {
		if (strcmp(options[cur], "Input") == 0 &&
		    SDL_NumJoysticks() > 0)
		{
		    if (im->get_mode() == IM_KEYBOARD)
		    {
			im->get_joystick();
			im->set_mode(IM_JOYSTICK);
		    }
		    else if(im->get_mode() == IM_JOYSTICK)
		    {
			im->clear_joystick();
			im->set_mode(IM_KEYBOARD);
		    }
		    select_delay = 5;
		}
	    }
	    if (keys[SDLK_RIGHT])
	    {
		if (strcmp(options[cur], "Input") == 0 &&
		    SDL_NumJoysticks() > 0)
		{
		    if (im->get_mode() == IM_KEYBOARD)
		    {
			im->get_joystick();
			im->set_mode(IM_JOYSTICK);
		    }
		    else if (im->get_mode() == IM_JOYSTICK)
		    {
			im->clear_joystick();
			im->set_mode(IM_KEYBOARD);
		    }
		    select_delay = 5;
		}
	    }
	    if (keys[SDLK_SPACE] || keys[SDLK_RETURN])
	    {
		selecting = true;
		select_timer = 30 * 5;
	    }
	}
	else
	    select_delay--;
    }
}

void OptionsMenu::draw(Surface *screen)
{
    Uint32 white = mapRGB(255, 255, 255);
    mx = 0.4 * level->dims[2] - 80;
    my = 0.1 * level->dims[3];
    int c = 0;
    for (int i = 0; i < num_options; i++)
    {
	if (im->get_mode() == IM_KEYBOARD ||
	    (im->get_mode() == IM_JOYSTICK && !(
		buttons[i] == IM_UP ||
		buttons[i] == IM_DOWN ||
		buttons[i] == IM_LEFT ||
		buttons[i] == IM_RIGHT)))
	{
	    if (i == cur)
		screen->blit(icon, mx - 30, my + 20 * c);

	    draw_string(screen, white, options[i], mx, my + 20 * c);
	    if (buttons[i] >= 0)
	    {
		char *value = NULL;;
		if (im->get_mode() == IM_KEYBOARD)
		{
		    int key = im->get_button(buttons[i]);
		    value = SDL_GetKeyName((SDLKey)key);
		}
		else if (im->get_mode() == IM_JOYSTICK)
		{
		    int button = im->get_button(buttons[i]);
		    char buf[80];
		    snprintf(buf, 80, "Button %d", button);
		    value = buf;
		}
		draw_string(screen, white, value, mx + 200, my + 20 * c);
	    }
	    c++;
	}
    }
    char *option = input_choices[im->get_mode()];
    draw_string(screen, white, option, mx + 200, my);
    if (selecting)
    {
	char wait[80];
	snprintf(wait, 80, "Waiting...%d", (int)(select_timer / 30.0) + 1);
	draw_string(screen, white, wait, mx, my + 20 * num_options + 20);
    }

    char inst[80];
    snprintf(inst, 80, "Press space/enter followed by what you want");
    draw_string(screen, white, inst, mx, my + 20 * num_options + 40);
    snprintf(inst, 80, "Press escape when done");
    draw_string(screen, white, inst, mx, my + 20 * num_options + 60);
}
