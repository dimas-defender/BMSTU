#ifndef ACTIONS_H
#define ACTIONS_H

#include <math.h>
#include "defines.h"
#include "object.h"

struct move
{
    float dx;
    float dy;
    float dz;
};

struct scale
{
    float kx;
    float ky;
    float kz;
};

struct rotate
{
    int dgx;
    int dgy;
    int dgz;
};

struct trig
{
    float cos_x;
    float cos_y;
    float cos_z;
    float sin_x;
    float sin_y;
    float sin_z;
};

int move_object(object_t &object, const struct move &params);
int scale_object(object_t &object, const struct scale &params);
int rotate_object(object_t &object, const struct rotate &params);

#endif // ACTIONS_H
