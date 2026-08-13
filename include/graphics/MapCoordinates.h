#ifndef MAP_COORDINATES_H
#define MAP_COORDINATES_H

#include "raylib.h"

struct SpaceGraphic
{
    Vector2 center;
    float radius;
};

extern const SpaceGraphic SPACE_GRAPHICS[32];

#endif