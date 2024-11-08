// event.h
// Alex Hergenhan 2024
// defines class event

#ifndef EVENT_H
#define EVENT_H


#include <iostream>
#include <cstdint> // for uint16_t

#include <TCanvas.h>

#include "hit.h"
#include "line.h"
#include "circle.h"

// a constant scale factor to make TDCs not overlap during minimsation
const int SCALE {2000};

class event {
protected:
    // an event contains 8 hits
    hit hits[8];
    double v_best;
    int best_tangent;
    double theta_best;

    // Event counter
    static int count;

public:
    // constructors
    event();
    event(char (&buffer)[16]);

    void print() const;

    // overload [] to allow access to hits
    hit& operator[](int i) { return hits[i]; } // do i need const on this?
    
    void get_two_largest_circles(int& i, int& j) const;

    friend double f(const event& ev, double v, int n_tangent, int n_c1, int n_c2);

    void geometry(int& n_evt_failed);

    void plot() const;

    // getters
    double get_v_best() const { return v_best; }
    int get_count() const { return count; }
    int get_best_tangent() const { return best_tangent; }
    double get_theta_best() const { return theta_best; }
};


#endif // EVENT_H
// end event.h
