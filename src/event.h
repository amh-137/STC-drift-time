// event.h
// Alex Hergenhan 2024
// defines class event

#ifndef EVENT_H
#define EVENT_H


#include <iostream>
#include <cstdint> // for uint16_t
#include <vector>

struct hit{
    int layer; // lowest order 3 bits of a 16 bit datum
    int wire; // wire number within the layer, next 3 bits
    int TDC; // final 10 bits
    /*
    uint16_t layer; // lowest order 3 bits of a 16 bit datum
    uint16_t wire; // wire number within the layer, next 3 bits
    uint16_t TDC; // final 10 bits
    */
};


class event {
private:
    // an event contains 8 hits
    std::vector<hit> hits;

public:
    // constructors
    event();
    event(char (&buffer)[16]);

    void print() const;
    
    // instead of having a print line function, I will overload the << operator to show off
    friend std::ostream& operator<<(std::ostream& os, const event& ev);

};

#endif // EVENT_H
// end event.h