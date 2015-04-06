#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "common.h"

#define IM_KEYBOARD 0
#define IM_JOYSTICK 1

#define IM_SHOOT 0
#define IM_BOMB 1
#define IM_BLANK 2
#define IM_ENERGY 3
#define IM_PREV 4
#define IM_NEXT 5
#define IM_UP 6
#define IM_DOWN 7
#define IM_LEFT 8
#define IM_RIGHT 9
#define IM_PAUSE 10
#define IM_MENU 11

class InputManager
{
    public:
        static InputManager *get_instance();

	SDL_Joystick *get_joystick();
	void clear_joystick();

	bool is_pressed(int button);
	void set_button(int button, int value);
	int get_button(int button);
	void set_mode(int m);
	int get_mode();
    private:
	InputManager();
	~InputManager();

	static InputManager *instance;

	SDL_Joystick *joystick;
	int mode;
	int buttons[2][12];
	Uint8 *keys;
};

#endif
