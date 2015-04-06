#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "surface.h"

class ResourceManager
{
    public:
        static ResourceManager *get_instance();
        ~ResourceManager();

	int get_size() { return sfx.size(); }
	Surface *get_image(const char *filename);
	void play_music(const char *filename);
	void play_sound(const char *filename);
	TTF_Font *get_font() { return font; }
	double *number_dims() { return numbers[0]->get_rect(); }
	void draw_number(Surface *dst, int n, double *pos, int length=0);
    private:
	ResourceManager();
	static ResourceManager *instance;

	map<string, Surface *> images;
	map<string, Mix_Music *> songs;
	map<string, Mix_Chunk *> sfx;
	char cur_song[80];
	TTF_Font *font;
	Surface **numbers;
};

#endif
