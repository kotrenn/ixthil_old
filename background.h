#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "surface.h"

#define MAX_CELL_LIFE 60

typedef struct bg_cell_type
{
    int value;
    int life;
} bg_cell_t;

class Background
{
    public:
        Background(double *d);
	~Background();

	void update();
	void draw(Surface *screen);

	void enter_subspace(double *point, int value = -1);
	void add_value(int value, double *v, double x, double y);
    private:
	bg_cell_t *get_cell(int x, int y);
	bg_cell_t *get_new_cell(int x, int y);
	void init_cell(bg_cell_t *cell);
	int get_hue_cell(int x, int y);
	void draw_cell(Surface *screen, int x, int y);
	int get_dominant(int x, int y, int k);
	int avg_life(int x, int y, int k);
	void update_cell(int x, int y);
	double dims[4];
	list<double *> fore;
	list<double *> back;
	double star_speed;
	int hue;
	bg_cell_t *cells;
	bg_cell_t *new_cells;
	int w, h;
	int cell_w, cell_h;
	double max_mag;
	double speed;
	int fuzzy;
	double max_brightness;
	bool subspace;
};

#endif
