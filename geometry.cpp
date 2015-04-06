#include "geometry.h"

bool rect_collides(double *a, double *b)
{
    return !(
	a[0] >= b[0] + b[2] ||
	a[1] >= b[1] + b[3] ||
	b[0] >= a[0] + a[2] ||
        b[1] >= a[1] + a[3] );
}
