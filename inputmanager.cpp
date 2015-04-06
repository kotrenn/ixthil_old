#include "inputmanager.h"

InputManager *InputManager::instance = NULL;

InputManager *InputManager::get_instance()
{
    if (instance == NULL)
	instance = new InputManager();
    return instance;
}

InputManager::InputManager()
{
    mode = IM_KEYBOARD;
    buttons[IM_KEYBOARD][IM_SHOOT] = SDLK_z;
    buttons[IM_KEYBOARD][IM_BOMB] = SDLK_x;
    buttons[IM_KEYBOARD][IM_BLANK] = SDLK_c;
    buttons[IM_KEYBOARD][IM_ENERGY] = SDLK_d;
    buttons[IM_KEYBOARD][IM_PREV] = SDLK_a;
    buttons[IM_KEYBOARD][IM_NEXT] = SDLK_s;
    buttons[IM_KEYBOARD][IM_UP] = SDLK_UP;
    buttons[IM_KEYBOARD][IM_DOWN] = SDLK_DOWN;
    buttons[IM_KEYBOARD][IM_LEFT] = SDLK_LEFT;
    buttons[IM_KEYBOARD][IM_RIGHT] = SDLK_RIGHT;
    buttons[IM_KEYBOARD][IM_PAUSE] = SDLK_RETURN;
    buttons[IM_KEYBOARD][IM_MENU] = SDLK_ESCAPE;
    buttons[IM_JOYSTICK][IM_SHOOT] = 1;
    buttons[IM_JOYSTICK][IM_BOMB] = 2;
    buttons[IM_JOYSTICK][IM_BLANK] = 0;
    buttons[IM_JOYSTICK][IM_ENERGY] = 3;
    buttons[IM_JOYSTICK][IM_PREV] = 4;
    buttons[IM_JOYSTICK][IM_NEXT] = 5;
    buttons[IM_JOYSTICK][IM_UP] = -1;
    buttons[IM_JOYSTICK][IM_DOWN] = -1;
    buttons[IM_JOYSTICK][IM_LEFT] = -1;
    buttons[IM_JOYSTICK][IM_RIGHT] = -1;
    buttons[IM_JOYSTICK][IM_PAUSE] = 9;
    buttons[IM_JOYSTICK][IM_MENU] = 8;
    keys = SDL_GetKeyState(NULL);
    joystick = NULL;
}

InputManager::~InputManager()
{
    if (joystick != NULL)
	clear_joystick();
}

SDL_Joystick *InputManager::get_joystick()
{
    return joystick;
}

void InputManager::clear_joystick()
{
    SDL_JoystickClose(joystick);
}

bool InputManager::is_pressed(int button)
{
    int k = get_button(button);
    if (mode == IM_JOYSTICK)
    {
	double dx = SDL_JoystickGetAxis(joystick, 0) / 32767;
	double dy = SDL_JoystickGetAxis(joystick, 1) / 32767;
	if (button == IM_UP)
	    return dy < -0.5;
	if (button == IM_DOWN)
	    return dy > 0.5;
	if (button == IM_LEFT)
	    return dx < -0.5;
	if (button == IM_RIGHT)
	    return dx > 0.5;
	return SDL_JoystickGetButton(joystick, k);
    }
    return keys[k];
}

void InputManager::set_button(int button, int value)
{
    buttons[mode][button] = value;
}

int InputManager::get_button(int button)
{
    return buttons[mode][button];
}

void InputManager::set_mode(int m)
{
    if (m == IM_JOYSTICK)
    {
	joystick = SDL_JoystickOpen(0);
	if (joystick == NULL)
	    return;
	mode = m;
    }
    else
	mode = m;
}

int InputManager::get_mode()
{
    return mode;
}
