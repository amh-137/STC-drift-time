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
private:
    // an event contains 8 hits
    hit hits[8];

public:
    // constructors
    event();
    event(char (&buffer)[16]);

    void print() const;

    // overload [] to allow access to hits
    hit& operator[](int i) { return hits[i]; } // do i need const on this?
    
    // instead of having a print line function, I will overload the << operator to show off
    friend std::ostream& operator<<(std::ostream& os, const event& ev);


    void geometry(std::vector<line>& lvec) const;

};


void conv_hit_to_coords(hit h, double &x, double &y);

#endif // EVENT_H
// end event.h
