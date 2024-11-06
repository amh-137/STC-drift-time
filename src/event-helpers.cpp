// event-helpers.cpp
// Alex Hergenhan 2024
// Here we expand the helper functions for the event class

#include <iostream>
#include <cmath>

#include "event-helpers.h"


void conv_hit_to_coords(hit h, double &x, double &y){
    x = h.layer;
    y = h.wire + 0.5*(h.layer%2);
}

void get_two_circle_tangent(circle c1, circle c2, line& l, int n){
    double i, j;
    // stackoverflow: switch statements are slightly faster than if else or using math to do it
    // get our 4 tangent lines with this:
    switch (n) {
    case 0:
        i = 1.; j = 1.; break;
    case 1:
        i = 1.; j = -1.; break;
    case 2:
        i = -1.; j = 1.; break;
    case 3:
        i = -1.; j = -1.; break;
    default:
        i = 1.; j = 1.; break;
    }

    double r1 {c1.r * i};
    double r2 {c2.r * j};

    double dr {r2 - r1};

    double x = c2.x0 - c1.x0;
    double y = c2.y0 - c1.y0;

    double z = x*x + y*y;
    double d = std::pow( std::abs(z - dr*dr), 0.5 );

    // ax + by + c = 0
    l.a = (x*dr + y*d) / z;
    l.b = (y*dr - x*d) / z;
    l.c = r1 - c1.x0*l.a - c1.y0*l.b;
}


double circle_line_distance(circle c, line l){
    return std::abs(l.a*c.x0 + l.b*c.y0 + l.c) / std::pow(l.a*l.a + l.b*l.b, 0.5);
}


// end event-helpers.cpp
