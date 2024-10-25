// hit.h
// Alex Hergenhan 2024

#ifndef HIT_H
#define HIT_H
#include <cstdint> // for uint16_t


struct hit{
    /*
    int layer; // lowest order 3 bits of a 16 bit datum
    int wire; // wire number within the layer, next 3 bits
    int TDC; // final 10 bits
    */
    
    uint16_t layer; // lowest order 3 bits of a 16 bit datum
    uint16_t wire; // wire number within the layer, next 3 bits
    double TDC; // final 10 bits
    
};

#endif // HIT_H
