#include "background.h"

Background::Background(double *d)
{
	cpy_rect(dims, d);
	for (int i = 0; i < 300; i++)
	{
		double *star = (double *)malloc(sizeof(double) * 2);
		set_pair(star, randint(0, dims[2] - 1), randint(0, dims[3] - 1));
		if (i % 4 == 0)
			fore.push_back(star);
		else
			back.push_back(star);
	}
	star_speed = 15;

	subspace = false;
	cell_w = 5;
	cell_h = 5;
	w = dims[2] / cell_w;
	h = dims[3] / cell_h;
	hue = 0;
	max_mag = 0.03;
	speed = 3.0;
	speed = 1.0;
	fuzzy = 1;
	max_brightness = 255.0;
	max_brightness = 32.0;
	max_brightness = 64.0;
	cells = (bg_cell_t *)malloc(sizeof(bg_cell_t) * w * h);
	new_cells = (bg_cell_t *)malloc(sizeof(bg_cell_t) * w * h);
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			init_cell(get_cell(x, y));
}

void Background::init_cell(bg_cell_t *cell)
{
	cell->value = -1;
	cell->life = 0;
}

Background::~Background()
{
	list<double *>::iterator i;
	for (i = fore.begin(); i != fore.end(); i++)
		free(*i);
	for (i = back.begin(); i != back.end(); i++)
		free(*i);
	free(cells);
	free(new_cells);
}

void Background::update()
{
	list<double *>::iterator i;
	for (i = fore.begin(); i != fore.end(); i++)
	{
		(*i)[1] += star_speed;
		if ((*i)[1] >= dims[3])
		{
			(*i)[1] -= dims[3];
			(*i)[0] = randint(0, dims[2] - 1) - randint(0, 25);
		}
	}
	for (i = back.begin(); i != back.end(); i++)
	{
		(*i)[1] += 0.5 * star_speed;
		if ((*i)[1] >= dims[3])
		{
			(*i)[1] -= dims[3];
			(*i)[0] = randint(0, dims[2] - 1) - randint(0, 25);
		}
	}
    
	if (subspace && randint(1, 100) == 1)
		get_cell(randint(1, w), randint(1, h))->value = hue;
    
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			update_cell(x, y);

	bg_cell_t *temp = cells;
	cells = new_cells;
	new_cells = temp;
}

void Background::update_cell(int x, int y)
{
	int k = 1;
	bg_cell_t *dst = get_new_cell(x, y);
	dst->value = get_dominant(x, y, k);
	if (subspace)
		dst->life = MAX_CELL_LIFE;
	else
		dst->life = max(0, avg_life(x, y, k) - 1);
	if (dst->life <= 0)
	{
		dst->value = -1;
		dst->life = 0;
	}
	/*
	  if (dst->value < 0)
	  {
	  dst->life = 0;
	  }
	*/
}

void Background::draw(Surface *screen)
{
	for (int x = 0; x < w; x++)
		for (int y = 0; y < h; y++)
			draw_cell(screen, x, y);
    
	Uint32 c1 = mapRGB(127, 127, 127);
	Uint32 c2 = mapRGB(63, 63, 63);
	list<double *>::iterator i;
	for (i = fore.begin(); i != fore.end(); i++)
		draw_point(screen, c1, (*i)[0], (*i)[1]);
	for (i = back.begin(); i != back.end(); i++)
		draw_point(screen, c2, (*i)[0], (*i)[1]);

	/*
	  int x, y;
	  SDL_GetMouseState(&x, &y);
	  int real_x = x / cell_w;
	  int real_y = y / cell_h;
	  char buf[80];
	  bg_cell_t *cell = get_cell(real_x, real_y);
	  snprintf(buf, 80, "value: %d life: %d hue %d",
	  cell->value, cell->life, get_hue_cell(real_x, real_y));
	  draw_string(screen, mapRGB(255, 255, 255), buf, x, y);
	*/
}

void Background::enter_subspace(double *point, int value)
{
	if (point == NULL)
	{
		subspace = false;
		return;
	}

	int x = point[0] / cell_w;
	int y = point[1] / cell_h;
	int k = 6;
	for (int dx = -k; dx <= k; dx++)
		for (int dy = -k; dy <= k; dy++)
			if (fabs(dx) + fabs(dy) <= k)
				get_cell(x + dx, y + dy)->value = value;
	/*
	  for (int i = 0; i < 5; i++)
	  get_cell(randint(0, w - 1), randint(0, h - 1))->value = value;
	*/
	hue = value;
	subspace = true;
}

bg_cell_t *Background::get_cell(int x, int y)
{
	while (x < 0)
		x += w;
	while (x >= w)
		x -= w;
	while (y < 0)
		y += h;
	while (y >= h)
		y -= h;
	return cells + w * y + x;
}

bg_cell_t *Background::get_new_cell(int x, int y)
{
	while (x < 0)
		x += w;
	while (x >= w)
		x -= w;
	while (y < 0)
		y += h;
	while (y >= h)
		y -= h;
	return new_cells + w * y + x;
}

int Background::get_hue_cell(int x, int y)
{
	int n = 2 * fuzzy + 1;
	int area = n * n;
	int theta[area];
	int i = 0;
	for (int dx = -fuzzy; dx <= fuzzy; dx++)
	{
		for (int dy = -fuzzy; dy <= fuzzy; dy++)
		{
			bg_cell_t *cur_cell = get_cell(x + dx, y + dy);
			if (cur_cell->value >= 0)
				theta[i++] = 60 * cur_cell->value;
			/*
			  else if (hue >= 0)
			  theta[i++] = (360 - (270 + 60 * hue)) % 360;
			*/
		}

	}
	return avg_theta(theta, i);
}

void Background::draw_cell(Surface *screen, int x, int y)
{
	bg_cell_t *cell = get_cell(x, y);
	if (cell->value < 0)
		return;
	int cell_hue = get_hue_cell(x, y);
	int brightness = (max_brightness * cell->life) / MAX_CELL_LIFE;
	Uint32 color = map_hueB((cell_hue + hue) % 360, brightness);
	double rect[] = { cell_w * x, cell_h * y, cell_w, cell_h };
	draw_rect(screen, color, rect);
}

int Background::get_dominant(int x, int y, int k)
{
	int count[6];
	for (int v = 0; v < 6; v++)
		count[v] = 0;
	int val;
	for (int dx = -k; dx <= k; dx++)
	{
		for (int dy = -k; dy <= k; dy++)
		{
			val = get_cell(x + dx, y + dy)->value;
			if (val >= 0 && val < 6)
				count[val]++;
		}
	}
	if (subspace && hue >= 0 && hue < 6)
		count[hue] = (count[hue] * 6) / 5;
	return rand_value_dict(count, 6);
}

int Background::avg_life(int x, int y, int k)
{
	int n = 2 * k + 1;
	int area = n * n;
	int vals[area];
	int i = 0;
	for (int dx = -k; dx <= k; dx++)
		for (int dy = -k; dy <= k; dy++)
			vals[i++] = get_cell(x + dx, y + dy)->life;
	return avg_value(vals, area);
}

void Background::add_value(int value, double *v, double x, double y)
{
	int k = 1;
	int real_x = x / cell_w;
	int real_y = y / cell_h;
	if (real_x < k || real_y < k ||
	    real_x >= w - k || real_y >= h - k)
		return;
	for (int dx = -k; dx <= k; dx++)
	{
		for (int dy = -k; dy <= k; dy++)
		{
			bg_cell_t *cell = get_cell(real_x + dx, real_y + dy);
			cell->value = value;
			cell->life = MAX_CELL_LIFE;
		}
	}
}
