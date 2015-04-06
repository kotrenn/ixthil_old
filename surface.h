#ifndef SURFACE_H
#define SURFACE_H

#include "common.h"

Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b);
Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a=255);
Uint32 mapRGBA(Uint32 c, Uint8 a=255);
Uint32 invertRGB(Uint32 c);

class Surface
{
public:
    Surface(int w, int h);
    Surface(SDL_Surface *src);
    Surface(const char *filename);
    ~Surface();
    
    void blit(Surface *src, int x, int y, int w=0, int h=0);
    void fill(Uint32 color);
    void set_alpha(Uint8 alpha);
    double *get_rect() { return dims; }
    int get_width() { return dims[2]; }
    int get_height() { return dims[3]; }
    SDL_Surface *get_surface() { return m_surface; }
    Surface *subsurface(int x, int y, int w, int h);
    void invert();
    void flip();
    
    double dims[4];
private:
    bool m_new_surface;
    SDL_Surface *m_surface;
};

void draw_point(Surface *dst, Uint32 color, int x, int y);
void draw_line(Surface *dst, Uint32 color, double *p1, double *p2);
void draw_line(Surface *dst, Uint32 color, double x1, double y1,
	       double x2, double y2);
void draw_rect(Surface *dst, Uint32 color, const double *rect, int width = 0);
void draw_rect_3d(Surface *dst, Uint32 color, const double *rect);
void draw_circle(Surface *dst, Uint32 color, double x, double y,
		 double radius, double width = 0);
void draw_string(Surface *dst, Uint32 color, const char *text,
		 double x, double y);

Uint32 map_hue(int hue, Uint8 alpha = 255);
Uint32 map_hueB(int hue, Uint8 brightness);

void draw_lightning(Surface *dst, double *p1, double *p2,
		    Uint32 color, int radius = 12);
void draw_lightning_circle(Surface *dst, double *center, Uint32 color,
			   int radius, int width = 12);
void draw_laser(Surface *dst, Uint32 color, double *p1, double *p2, int w);

#endif
