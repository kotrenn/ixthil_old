#include "item.h"
#include "level.h"

Item::Item(Level *l, double x, double y, int type)
	:Actor(l, x, y, 1, 1)
{
	item_type = type;
	char imagename[80];
	if (item_type >= 0)
		snprintf(imagename, 80, "item%d.png", item_type);
	else if (item_type == IT_BLANK)
		snprintf(imagename, 80, "item-1.png");
	else if (item_type == IT_BOMB)
		snprintf(imagename, 80, "bomb.png");
	else if (item_type == IT_HEALTH)
		snprintf(imagename, 80, "life.png");
	else if (item_type == IT_SHIELD)
		snprintf(imagename, 80, "shield.png");
	else if (item_type == IT_HEALTH_PLUS)
		snprintf(imagename, 80, "health_up.png");
	else if (item_type == IT_SHIELD_PLUS)
		snprintf(imagename, 80, "shield_up.png");
	else if (item_type == IT_ENERGY)
		snprintf(imagename, 80, "item0.png");
	else if (item_type == IT_PORTAL)
		snprintf(imagename, 80, "item0.png");
	image = rm->get_image(imagename);
	dims[2] = image->get_width();
	dims[3] = image->get_height();
	pulled = false;
	speed = 4;
	set_pair(v, 0, speed);
	level->add_item(this);
}



EnergyItem::EnergyItem(Level *l, double x, double y)
	:Item(l, x, y, IT_ENERGY)
{
	dims[2] = dims[3] = 8;
	v[0] = 0.25 * randint(-20, 20);
	v[1] = 0.25 * randint(-20, 20);
	dims[0] += v[0];
	dims[1] += v[1];
	frame = randint(0, 5);
}

void EnergyItem::update()
{
	if (v[0] > 0)
		v[0] -= 0.5;
	if (v[0] < 0)
		v[0] += 0.5;
	if (v[1] < speed)
		v[1] += 1;
	Item::update();
}

void EnergyItem::draw(Surface *screen)
{
	Uint32 color = Bullet::get_color(frame);
	frame = (frame + 1) % 6;
	draw_circle(screen, color, dims[0] + 0.5 * dims[2],
	            dims[1] + 0.5 * dims[3], 0.5 * dims[2]);
}



PortalItem::PortalItem(Level *l, int c)
	:Item(l, 0, 0, IT_PORTAL)
{
	color = c;
	r = 25;
	dims[2] = r * 2;
	dims[3] = r * 2;
	dims[0] = level->dims[0] + 0.5 * (level->dims[2] - dims[2]);
	dims[1] = -1 * dims[3];
	set_pair(v, 0, 4);
	t0 = 0;
	t1 = 0;
}

void PortalItem::update()
{
	Item::update();

	t0 = (t0 + 10) % 360;
	t1 = (t1 - 20) % 360;
}

void PortalItem::draw(Surface *screen)
{
	Uint32 c = Bullet::get_color(color);
	for (int i = 0; i < 6; i++)
	{
		double theta = (t0 + i * 60) * M_PI / 180.0;
		double tx = dims[0];
		double ty = dims[1];
		tx += 0.5 * dims[2] + r * 2.0 * cos(theta);
		ty += 0.5 * dims[3] + r * 2.0 * sin(theta);
		for (int j = 0; j < 3; j++)
		{
			double theta1 = (t1 + 120 * j) * M_PI / 180.0;
			double theta2 = (t1 + 120 * (j + 1)) * M_PI / 180.0;
			double px1 = (r / 2.0) * cos(theta1);
			double py1 = (r / 2.0) * sin(theta1);
			double px2 = (r / 2.0) * cos(theta2);
			double py2 = (r / 2.0) * sin(theta2);
			draw_line(screen, c, (double[]){tx + px1, ty + py1},
			          (double[]){tx + px2, ty + py2});
		}
	}
}
