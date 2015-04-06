#ifndef GEOMETRY_H
#define GEOMETRY_H

#define set_rect(rect, x, y, w, h) \
    (rect)[0] = (x); \
    (rect)[1] = (y); \
    (rect)[2] = (w); \
    (rect)[3] = (h);

#define cpy_rect(r1, r2) \
    (r1)[0] = (r2)[0]; \
    (r1)[1] = (r2)[1]; \
    (r1)[2] = (r2)[2]; \
    (r1)[3] = (r2)[3];

#define set_pair(pair, x, y) \
    (pair)[0] = (x); \
    (pair)[1] = (y);

#define cpy_pair(p1, p2) \
    (p1)[0] = (p2)[0]; \
    (p1)[1] = (p2)[1];

bool rect_collides(double *a, double *b);

#endif
