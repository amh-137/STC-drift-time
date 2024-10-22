// circle.h
// Alex Hergenhan 2024

#ifndef CIRCLE_H
#define CIRCLE_H

// (x-x0)^2 + (y-y0)^2 = r^2
struct circle{
    double x0;
    double y0;
    double r;

    // constructors
    circle() : x0{0.}, y0{0.}, r{0.} {}
    circle(double x0, double y0, double r) : x0{x0}, y0{y0}, r{r} {}
};

#endif // CIRCLE_H
