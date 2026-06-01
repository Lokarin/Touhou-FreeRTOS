#include "collision.h"

int circles_overlap(Circle a, Circle b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dist_sq = dx*dx + dy*dy;
    float radii_sq = (a.r + b.r) * (a.r + b.r);
    return dist_sq < radii_sq;
}
