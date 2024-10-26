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

class event {
protected:
    // an event contains 8 hits
    hit hits[8];
    double v_best;
    int best_tangent;

    // Event counter
    static int count;

public:
    // constructors
    event();
    event(char (&buffer)[16]);

    void print() const;

    // overload [] to allow access to hits
    hit& operator[](int i) { return hits[i]; } // do i need const on this?
    
    // instead of having a print line function, I will overload the << operator to show off
    friend std::ostream& operator<<(std::ostream& os, const event& ev);

    void get_two_largest_circles(int& i, int& j) const;

    /** @brief
     *  function to be minimised
     * @param v: velocity of particle
     * @param n_tangent: number of tangent line (0-3)
     * @param n_c1: number of largest circle (0-7)
     * @param n_c2: number of second largest circle (0-7)
    */
    friend double f(const event& ev, double v, int n_tangent, int n_c1, int n_c2);

    void geometry();

    void plot() const;

    double get_v_best() const { return v_best; }
};


void conv_hit_to_coords(hit h, double &x, double &y);

#endif // EVENT_H
// end event.h
