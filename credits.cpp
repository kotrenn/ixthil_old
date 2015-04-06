#include "credits.h"

Credits::Credits(Level *l)
    :Ship(l, "player.png")
{
    dims[0] = 0.5 * dims[2];
    dims[1] = -9001;
    health = 1;
    invincible = true;
    items = false;
    level->next_level = 0;

    const char *lines[] =
	            { "---Credits---", NULL, NULL, NULL,
		      "--Programming--", NULL, "Rob Seay", NULL, NULL,
		      "--Art--", NULL, "Cave Story", "Galaga",
		      "Perfect Cherry Blossom", "Rob Seay", NULL, NULL,
		      "--Music--", NULL, "Newgrounds", "Project Chaos",
		      "Sonic 2", NULL, NULL, "And Weena Mercator as",
		      "\'The Hopping Woman\'", NULL, NULL,
		      "--Inspiration--", NULL, "Perfect Cherry Blossom",
		      "Phoenix", "Sonic The Hedgehog", NULL, NULL,
		      "--Special Thanks--", NULL, "Rudy Busch",
		      "Matthew Daniels", "Mark Gomer", "Karen Greene",
		      "Aubrey Harley", "Adam Hassler", "Patrick Johnson",
		      "Bryce Richards", NULL, NULL, NULL, NULL, NULL,
		      "---Thanks For Play!---" };
    int num_lines = 51;
    list<WordShip *> words;
    
    for (int i = 0; i < num_lines; i++)
    {
	if (lines[i] != NULL)
	{
	    WordShip *word = new WordShip(level, 60 + 30 * i, lines[i]);
	    words.push_back(word);
	}
    }
    words.back()->invincible = true;
}

void Credits::update()
{
    Ship::update();
    
    invincible = false;
    damage(health, -1);
}



WordShip::WordShip(Level *l, double y, const char *msg)
    :Ship(l, "player.png")
{
    TTF_Font *font = ResourceManager::get_instance()->get_font();
    SDL_Color c;
    c.r = c.g = c.b = c.unused = 255;
    SDL_Surface *message = TTF_RenderText_Solid(font, msg, c);
    image = new Surface(message);
    dims[0] = 0.5 * (level->dims[2] - image->get_width());
    dims[1] = level->dims[3] + y;
    dims[2] = image->get_width();
    dims[3] = image->get_height();
    health = 50;
    set_pair(v, 0, -1);
    items = false;
    particles = false;
}

WordShip::~WordShip()
{
    SDL_FreeSurface(image->get_surface());
    delete image;
}

void WordShip::update()
{
    Ship::update();
    
    if (dims[1] < -250)
    {
	invincible = false;
	damage(health, -1);
    }
}
