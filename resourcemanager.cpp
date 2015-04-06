#include "resourcemanager.h"

ResourceManager *ResourceManager::instance = NULL;

ResourceManager *ResourceManager::get_instance()
{
    if (instance == NULL)
	instance = new ResourceManager();
    return instance;
}

ResourceManager::ResourceManager()
{
    cur_song[0] = '\0';
    char fontname[] = "freesansbold.ttf";
    if ((font = TTF_OpenFont(fontname, 20)) == NULL)
    {
	fprintf(stderr, "Could not open %s\n", fontname);
	fprintf(stderr, "%s\n", SDL_GetError());
	exit(1);
    }
    numbers = (Surface **)malloc(sizeof(Surface *) * 10);
    Surface *nums = get_image("numbers.png");
    int w = nums->get_width() / 10;
    int h = nums->get_height();
    for (int i = 0; i < 10; i++)
	numbers[i] = nums->subsurface(i * w, 0, w, h);
}

ResourceManager::~ResourceManager()
{
    for (int i = 0; i < 10; i++)
	delete numbers[i];
    map<string, Surface *>::iterator i;
    for (i = images.begin(); i != images.end(); i++)
	delete (*i).second;
    map<string, Mix_Music *>::iterator m;
    for (m = songs.begin(); m != songs.end(); m++)
	Mix_FreeMusic((*m).second);
    map<string, Mix_Chunk *>::iterator s;
    for (s = sfx.begin(); s != sfx.end(); s++)
	Mix_FreeChunk((*s).second);
}

Surface *ResourceManager::get_image(const char *filename)
{
    if (images.find(string(filename)) == images.end())
    {
	char fullname[80];
	snprintf(fullname, 80, "images/%s", filename);
	images[string(filename)] = new Surface(fullname);
    }
    return images[string(filename)];
}

void ResourceManager::play_music(const char *filename)
{
	//return;
	
    string s_name = string(filename);
    if (songs.find(s_name) == songs.end())
    {
	char fullname[80];
	snprintf(fullname, 80, "music/%s", filename);
	songs[s_name] = Mix_LoadMUS(fullname);
	if (songs[s_name] == NULL)
	    fprintf(stderr, "Mix_LoadMUS failed: %s\n", SDL_GetError());
    }
    if (strcmp(filename, cur_song) == 0)
	return;
    else
    {
	strncpy(cur_song, filename, 80);
	Mix_PlayMusic(songs[s_name], -1);
    }
}

void ResourceManager::play_sound(const char *filename)
{
	return;
	
    string s_name = string(filename);
    if (sfx.find(s_name) == sfx.end())
    {
	char fullname[80];
	snprintf(fullname, 80, "music/%s", filename);
	printf("Loading sfx <%s>\n", fullname);
	sfx[s_name] = Mix_LoadWAV(fullname);
	if (sfx[s_name] == NULL)
	    fprintf(stderr, "Mix_LoadWAV failed: %s\n", SDL_GetError());
    }
    Mix_PlayChannel(-1, sfx[s_name], 0);
}

void ResourceManager::draw_number(Surface *dst, int n,
				  double *pos, int length)
{
    char buf[80];
    snprintf(buf, 80, "%d", n);
    int len = strlen(buf);
    int max = len > length? len : length;
    int min = length < len? 0 : length - len;
    int number[max];
    for (int i = 0; i < min; i++)
	number[i] = 0;
    for (int i = 0; i < (int)strlen(buf); i++)
	number[i + min] = buf[i] - '0';
    for (int i = 0; i < max; i++)
    {
	Surface *image = numbers[number[i]];
	dst->blit(image, i * image->get_width() + pos[0], pos[1]);
    }
}
