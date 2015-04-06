#include "surface.h"
#include "resourcemanager.h"

Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b)
{
    return mapRGBA(r, g, b, 255);
}

Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    Uint32 color = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    color |= r << 24;
    color |= g << 16;
    color |= b << 8;
    color |= a;
#else
    color |= r;
    color |= g << 8;
    color |= b << 16;
    color |= a << 24;
#endif
    return color;
}

Uint32 mapRGBA(Uint32 c, Uint8 a)
{
    Uint32 color = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    color = (c & 0xffffff00) | (a);
#else
    color = (c & 0x00ffffff) | (a << 24);
#endif
    return color;
}

Uint32 invertRGB(Uint32 color)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 RGBmask = 0xffffff00;
    Uint32 Amask   = 0x000000ff;
#else
    Uint32 RGBmask = 0x00ffffff;
    Uint32 Amask   = 0xff000000;
#endif
    return (color & Amask) | (~color & RGBmask);
}

Surface::Surface(int w, int h)
{
    set_rect(dims, 0, 0, w, h);

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    m_surface = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCALPHA, w, h, 32,
				     rmask, gmask, bmask, amask);
    if (m_surface == NULL)
    {
	fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
	exit(1);
    }
    SDL_SetAlpha(m_surface, 0, 255);
    m_new_surface = true;
}

Surface::Surface(SDL_Surface *src)
{
    m_surface = src;
    set_rect(dims, 0, 0, src->w, src->h);
    m_new_surface = false;
}

Surface::Surface(const char *filename)
{
    m_surface = IMG_Load(filename);
    set_rect(dims, 0, 0, m_surface->w, m_surface->h);
    m_new_surface = true;
}

Surface::~Surface()
{
    if (m_new_surface)
	SDL_FreeSurface(m_surface);
}

void Surface::blit(Surface *src, int x, int y, int w, int h)
{
    //SDL_Rect srcrect = (SDL_Rect){dims[0], dims[1], dims[2], dims[3]};
    //SDL_Rect offset = (SDL_Rect){x, y, src->get_width(), src->get_height()};
    SDL_Rect srcrect = (SDL_Rect){src->get_rect()[0],
				  src->get_rect()[1],
				  src->get_width(), src->get_height()};
    SDL_Rect dstrect = (SDL_Rect){x, y, 0, 0};
    SDL_BlitSurface(src->get_surface(), &srcrect, m_surface, &dstrect);
}

void Surface::fill(Uint32 color)
{
    SDL_FillRect(m_surface, NULL, color);
}

void Surface::set_alpha(Uint8 alpha)
{
    SDL_SetAlpha(m_surface, 0, alpha);
}

Surface *Surface::subsurface(int x, int y, int w, int h)
{
    Surface *sub = new Surface(m_surface);
    set_rect(sub->dims, x + dims[0], y + dims[1], w, h);
    return sub;
}

void Surface::invert()
{
    SDL_LockSurface(m_surface);
    SDL_PixelFormat *fmt = m_surface->format;
    int bpp = fmt->BytesPerPixel;

    for (int x = 0; x < get_width(); x++)
    {
	for (int y = 0; y < get_height(); y++)
	{
	    Uint32 *p = (Uint32 *)((Uint8 *)m_surface->pixels +
				   y * m_surface->pitch + x * bpp);
	    Uint32 RGBmask = fmt->Rmask | fmt->Gmask | fmt->Bmask;
	    *p = (*p & fmt->Amask) | (~(*p) & RGBmask);
	}
    }

    SDL_UnlockSurface(m_surface);
}

void Surface::flip()
{
    SDL_Flip(m_surface);
}



void draw_point(Surface *dst, Uint32 color, int x, int y)
{
    if (x < 0 || x >= dst->get_width() ||
        y < 0 || y >= dst->get_height())
	return;
    
    SDL_Surface *surf = dst->get_surface();
    SDL_LockSurface(surf);
    SDL_PixelFormat *fmt = surf->format;
    int bpp = fmt->BytesPerPixel;
    Uint8 *p = (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
    Uint32 pixel;

    /*if (c_a < 255)*/
    if (((color & fmt->Amask) >> fmt->Ashift) << fmt->Aloss < 255)
    {
	Uint8 c_r, c_g, c_b, c_a;
	SDL_GetRGBA(color, surf->format, &c_r, &c_g, &c_b, &c_a);
	Uint8 r, g, b;
	SDL_GetRGB(*(Uint32 *)p, surf->format, &r, &g, &b);

	Uint8 a = ~c_a; // 255 - c_a
	Uint8 a_r = (a * r + c_a * c_r) / 255;
	Uint8 a_g = (a * g + c_a * c_g) / 255;
	Uint8 a_b = (a * b + c_a * c_b) / 255;
	pixel = SDL_MapRGB(surf->format, a_r, a_g, a_b);
    }
    else
	pixel = color;
    *(Uint32 *)p = pixel;

    SDL_UnlockSurface(surf);
}

void draw_line(Surface *dst, Uint32 color, double *p1, double *p2)
{
    draw_line(dst, color, p1[0], p1[1], p2[0], p2[1]);
}

void draw_line(Surface *dst, Uint32 color, double x1, double y1,
	       double x2, double y2)
{
    int dx = fabs(x2 - x1);
    int dy = fabs(y2 - y1);
    if (dx == 0)
    {
	if (y2 > y1)
	    for (int i = 0; i <= dy; i++)
		draw_point(dst, color, x1, y1 + i);
	else
	    for (int i = 0; i <= dy; i++)
		draw_point(dst, color, x1, y1 - i);
	return;
    }
    else if (dy == 0)
    {
	if (x2 > x1)
	    for (int i = 0; i <= dx; i++)
		draw_point(dst, color, x1 + i, y1);
	else
	    for (int i = 0; i <= dx; i++)
		draw_point(dst, color, x1 - i, y1);
	return;
    }

    double temp;
    if (dx > dy)
    {
	if (x1 > x2)
	{
	    temp = x1;
	    x1 = x2;
	    x2 = temp;

	    temp = y1;
	    y1 = y2;
	    y2 = temp;
	}

	int ddy = 1;
	if (y1 >= y2) ddy = -1;
	int y = y1;
	int p = 0;

	for (int x = (int)x1; x <= x2; x++)
	{
	    draw_point(dst, color, x, y);
	    
	    p += dy;
	    
	    if (p * 2 >= dx)
	    {
		y += ddy;
		p -= dx;
	    }
	}
    }
    else
    {
	if (y1 > y2)
	{
	    temp = x1;
	    x1 = x2;
	    x2 = temp;

	    temp = y1;
	    y1 = y2;
	    y2 = temp;
	}

	int x = x1;
	int p = 0;
	int ddx = 1;
	if (x1 >= x2) ddx = -1;

	for (int y = (int)y1; y <= y2; y++)
	{
	    draw_point(dst, color, x, y);

	    p += dx;

	    if (p * 2 >= dy)
	    {
		x += ddx;
		p -= dy;
	    }
	}
    }
}

void draw_rect(Surface *dst, Uint32 color, const double *rect, int width)
{
    if (width == 0)
    {
	int max_x = rect[0] + rect[2] - 1;
	int max_y = rect[1] + rect[3] - 1;
	for (int y = rect[1]; y <= max_y; y++)
	    draw_line(dst, color, rect[0], y, max_x, y);
	return;
    }

    if (width * 2 > rect[2]) width = rect[2] / 2;
    if (width * 2 > rect[3]) width = rect[3] / 2;

    for (int i = 0; i < width; i++)
    {
	int x1 = rect[0] + i;
	int y1 = rect[1] + i;
	int x2 = rect[0] + rect[2] - i - 1;
	int y2 = rect[1] + rect[3] - i - 1;
	draw_line(dst, color, x1, y1, x2, y1);
	draw_line(dst, color, x1, y2, x2, y2);
	draw_line(dst, color, x1, y1, x1, y2);
	draw_line(dst, color, x2, y1, x2, y2);
    }
}

void draw_rect_3d(Surface *dst, Uint32 color, const double *rect)
{
    draw_rect(dst, color, rect);
    Uint8 c[3];
    SDL_GetRGB(color, dst->get_surface()->format,
	       c, c + 1, c + 2);
    Uint32 color1 = mapRGB((255 + c[0]) / 2, (255 + c[1]) / 2,
			   (255 + c[2]) / 2);
    Uint32 color2 = mapRGB(c[0] / 2, c[1] / 2, c[2] / 2);

    double x1 = rect[0];
    double y1 = rect[1];
    double x2 = rect[0] + rect[2];
    double y2 = rect[1] + rect[3];
    draw_line(dst, color1, x1, y1, x2 - 1, y1);
    draw_line(dst, color1, x1, y1, x1, y2 - 1);
    draw_line(dst, color1, x1 + 1, y1 + 1, x2 - 2, y1 + 1);
    draw_line(dst, color1, x1 + 1, y1 + 1, x1 + 1, y2 - 2);
    draw_line(dst, color2, x1, y2 - 1, x2 - 1, y2 - 1);
    draw_line(dst, color2, x2 - 1, y1, x2 - 1, y2 - 1);
    draw_line(dst, color2, x1 + 1, y2 - 2, x2 - 2, y2 - 2);
    draw_line(dst, color2, x2 - 2, y1 + 1, x2 - 2, y2 - 2);
}

void circle_points(Surface *dst, Uint32 color, int cx, int cy,
		   int x, int y)
{
    if (x == 0)
    {
	draw_point(dst, color, cx, cy + y);
	draw_point(dst, color, cx, cy - y);
	draw_point(dst, color, cx + y, cy);
	draw_point(dst, color, cx - y, cy);
    }
    else if (x == y)
    {
	draw_point(dst, color, cx + x, cy + y);
	draw_point(dst, color, cx - x, cy + y);
	draw_point(dst, color, cx + x, cy - y);
	draw_point(dst, color, cx - x, cy - y);
    }
    else if (x < y)
    {
	draw_point(dst, color, cx + x, cy + y);
	draw_point(dst, color, cx - x, cy + y);
	draw_point(dst, color, cx + x, cy - y);
	draw_point(dst, color, cx - x, cy - y);
	draw_point(dst, color, cx + y, cy + x);
	draw_point(dst, color, cx - y, cy + x);
	draw_point(dst, color, cx + y, cy - x);
	draw_point(dst, color, cx - y, cy - x);
    }
}

void draw_circle(Surface *dst, Uint32 color, double cx, double cy,
		 double radius, double width)
{
    if (radius == 0) return;
    if (width == 0)
    {
	int x0 = cx - radius;
	int y0 = cy - radius;
	int max_x = x0 + 2 * radius;
	int max_y = y0 + 2 * radius;
	int entered, dx, dy, dx_2;
	int r_2 = radius * radius;
	for (int x = x0; x <= max_x; x++)
	{
	    entered = 0;
	    dx = fabs(cx - x);
	    dx_2 = dx * dx;
	    for (int y = y0; y <= max_y; y++)
	    {
		dy = fabs(cy - y);
		if (dx_2 + dy * dy <= r_2)
		{
		    draw_point(dst, color, x, y);
		    entered = 1;
		}
		else if (entered)
		    y = max_y;
	    }
	}
	return;
    }
    
    int x = 0;
    int y = (int)radius;
    int p = (int)((5 - 4 * radius) / 4);
    circle_points(dst, color, (int)cx, (int)cy, x, y);
    while (x < y)
    {
	x++;
	if (p < 0) p += 2 * x + 1;
	else
	{
	    y--;
	    p += 2 * (x - y) + 1;
	}
	circle_points(dst, color, (int)cx, (int)cy, x, y);
    }
}



Uint32 map_hue(int hue, Uint8 alpha)
{
    int c = (hue / 60) % 6;
    double f = hue / 60.0 - (int)(hue / 60.0);
    double r = 0, g = 0, b = 0;
    switch (c)
    {
    case 0: r = 1; g = f; b = 0; break;
    case 1: r = 1 - f; g = 1; b = 0; break;
    case 2: r = 0; g = 1; b = f; break;
    case 3: r = 0; g = 1 - f; b = 1; break;
    case 4: r = f; g = 0; b = 1; break;
    case 5: r = 1; g = 0; b = 1 - f; break;
    }
    int c_r = (int)(r * 255);
    int c_g = (int)(g * 255);
    int c_b = (int)(b * 255);
    return mapRGBA(c_r, c_g, c_b, alpha);
}

Uint32 map_hueB(int hue, Uint8 brightness)
{
    int c = (hue / 60) % 6;
    double f = hue / 60.0 - (int)(hue / 60.0);
    double r = 0, g = 0, b = 0;
    switch (c)
    {
    case 0: r = 1; g = f; b = 0; break;
    case 1: r = 1 - f; g = 1; b = 0; break;
    case 2: r = 0; g = 1; b = f; break;
    case 3: r = 0; g = 1 - f; b = 1; break;
    case 4: r = f; g = 0; b = 1; break;
    case 5: r = 1; g = 0; b = 1 - f; break;
    }
    int c_r = (int)(r * brightness);
    int c_g = (int)(g * brightness);
    int c_b = (int)(b * brightness);
    return mapRGBA(c_r, c_g, c_b, 255);
}



void draw_string(Surface *dst, Uint32 color, const char *text,
		 double x, double y)
{
    TTF_Font *font = ResourceManager::get_instance()->get_font();
    SDL_Color c;
    SDL_PixelFormat *fmt = dst->get_surface()->format;
    c.r = (color & fmt->Rmask) >> fmt->Rshift;
    c.g = (color & fmt->Gmask) >> fmt->Gshift;
    c.b = (color & fmt->Bmask) >> fmt->Bshift;
    c.unused = (color & fmt->Amask) >> fmt->Ashift;
    SDL_Surface *message = TTF_RenderText_Solid(font, text, c);
    Surface *msg = new Surface(message);
    dst->blit(msg, x, y);
    delete msg;
    SDL_FreeSurface(message);
}



void draw_lightning(Surface *dst, double *p1, double *p2,
		    Uint32 color, int radius)
{
    int dist = 20;
    double dx = p2[0] - p1[0];
    double dy = p2[1] - p1[1];
    double d = sqrt(dx * dx + dy * dy);
    int count = (int)(d / dist);

    dx = (dist * (dx / d));
    dy = (dist * (dy / d));

    double px[count + 2];
    double py[count + 2];
    px[0] = p1[0];
    py[0] = p1[1];
    for (int i = 1; i < count + 1; i++)
    {
	px[i] = p1[0] + i * dx;
	py[i] = p1[1] + i * dy;
    }
    px[count + 1] = p2[0];
    py[count + 1] = p2[1];

    double x0 = px[0];
    double y0 = py[0];
    double x, y;
    int r = 0;
    double theta = 0;
    for (int i = 1; i < count + 2; i++)
    {
	x = px[i];
	y = py[i];
	if (i <= count)
	{
	    r = randint(0, radius);
	    theta = randint(1, 360) * M_PI / 180;
	    x += r * cos(theta);
	    y += r * sin(theta);
	}
	draw_line(dst, color, (double[]){x0, y0}, (double[]){x, y});
	x0 = x;
	y0 = y;
    }
}

void draw_lightning_circle(Surface *dst, double *center, Uint32 color,
			   int radius, int width)
{
    double cx = center[0];
    double cy = center[1];
    double D = 50;
    double r = radius * sqrt(2);
    int n = 3;
    while (r * sqrt(1.0 - cos(2.0 * M_PI / n)) > D && n < 180)
	n++;
    int dt = randint(1, 360);
    for (int i = 0; i < n; i++)
    {
	double theta1 = (360.0 / n) * i + dt;
	double theta2 = (360.0 / n) * ((i + 1) % n) + dt;
	theta1 *= M_PI / 180.0;
	theta2 *= M_PI / 180.0;
	double x1 = radius * cos(theta1) + cx;
	double y1 = radius * sin(theta1) + cy;
	double x2 = radius * cos(theta2) + cx;
	double y2 = radius * sin(theta2) + cy;
	draw_lightning(dst, (double[]){x1, y1}, (double[]){x2, y2},
		       color, width);
    }
}

void draw_laser(Surface *dst, Uint32 color, double *p1, double *p2, int w)
{
    w *= 2;
    if (w < 2)
	return;
    Uint8 c[4];
    SDL_GetRGBA(color, dst->get_surface()->format, c, c + 1, c + 2, c + 3);
    double x1 = p1[0], y1 = p1[1];
    double x2 = p2[0], y2 = p2[1];
    double dc[] = {0, 0, 0};
    for (int i = 0; i < 3; i++)
	dc[i] = c[i] / (w / 2);
    Uint8 tcolor[4] = {0, 0, 0, 0};
    Uint32 tc = 0;
    double slope = (x1 - x2) / (y1 - y2);
    if (fabs(slope) < 1)
    {
	for (int i = 0; i < 1 + w / 2; i++)
	{
	    for (int j = 0; j < 3; j++)
		tcolor[j] = c[j] - i * dc[j];
	    tcolor[3] = (255 - i * (255 / (w / 2))) * (c[3] / 255.0);
	    tc = mapRGBA(tcolor[0], tcolor[1], tcolor[2], tcolor[3]);
	    draw_line(dst, tc, (double[]){x1 + i, y1},
		      (double[]){x2 + i, y2});
	    draw_line(dst, tc, (double[]){x1 - i, y1},
		      (double[]){x2 - i, y2});
	}
    }
    else
    {
	for (int i = 0; i < 1 + w / 2; i++)
	{
	    for (int j = 0; j < 3; j++)
		tcolor[j] = c[j] - i * dc[j];
	    tcolor[3] = (255 - i * (255 / (w / 2))) * (c[3] / 255.0);
	    tc = mapRGBA(tcolor[0], tcolor[1], tcolor[2], tcolor[3]);
	    draw_line(dst, tc, (double[]){x1, y1 + i},
		      (double[]){x2, y2 + i});
	    draw_line(dst, tc, (double[]){x1, y1 - i},
		      (double[]){x2, y2 - i});
	}
    }
}
