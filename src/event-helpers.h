// event-helpers.cpp
// Alex Hergenhan 2024
// Here we define the helper functions for the event class

#include <iostream>
#include <cmath>

#include "circle.h"
#include "line.h"
#include "hit.h"

#ifndef EVENT_HELPERS_H
#define EVENT_HELPERS_H


// convert hit to (layer, wire) coordinates (x, y) [cm, cm]
void conv_hit_to_coords(hit h, double &x, double &y);

// get the nth tangent line between two circles n=0,1,2,3
void get_two_circle_tangent(circle c1, circle c2, line& l, int n);


// shortest distance between line and circle
double circle_line_distance(circle c, line l);

#endif // end event-helpers.cpp
