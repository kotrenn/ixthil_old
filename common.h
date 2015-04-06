#ifndef COMMON_H
#define COMMON_H

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <list>
#include <map>
#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#include "geometry.h"

using namespace std;

int randint(int a, int b);
int rnd(double x);
int rand_value_dict(int *count, int n);
int avg_value(int *vals, int n);
int avg_theta(int *theta, int n);

#endif
