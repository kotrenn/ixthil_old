#include "common.h"

int randint(int a, int b)
{
    return rand()%(b - a + 1) + a;
}

int rnd(double x)
{
    if (x < 0)
	return -1 * rnd(-1.0 * x);
    double frac, ret;
    frac = modf(x, &ret);
    if (frac < 0.5)
	return (int)ret;
    return (int)(ret + 1.0);
}

int rand_value_dict(int *count, int n)
{
    int total = 0;
    for (int v = 0; v < n; v++)
	total += count[v];
    if (total <= 0)
	return -1;
    int k = randint(1, total);
    int val = -1;
    total = 0;
    for (int v = 0; v < n; v++)
    {
	if (total < k)
	    val = v;
	total += count[v];
    }
    return val;
}

int avg_value(int *vals, int n)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
	sum += vals[i];
    return sum / n;
}

int avg_theta(int *theta, int n)
{
    int top_avg = 0, top_n = 0;
    int bot_avg = 0, bot_n = 0;
    for (int i = 0; i < n; i++)
    {
	if (theta[i] < 180)
	{
	    top_n++;
	    top_avg += theta[i];
	}
	else
	{
	    bot_n++;
	    bot_avg += theta[i];
	}
    }
    if (top_n > 0)
	top_avg /= top_n;
    if (bot_n > 0)
	bot_avg /= bot_n;
    int diff = 0;
    if (bot_avg - top_avg >= 180)
    {
	diff = 360 - bot_avg;
	bot_avg = 0;
	top_avg = (top_avg + diff) % 360;
    }
    int avg = (bot_avg * bot_n + top_avg * top_n);
    avg /= top_n + bot_n;
    avg = (avg + 360 - diff) % 360;
    return avg;
    double x = 0;
    double y = 0;
    for (int i = 0; i < n; i++)
    {
	x += cos(theta[i] * M_PI / 180.0);
	y += sin(theta[i] * M_PI / 180.0);
    }
    x /= n;
    y /= n;
    if (fabs(x) + fabs(y) < 0.01)
	return randint(1, 360);
    return ((int)((atan2(x, y) * 180.0) / M_PI) + 360) % 360;
}
