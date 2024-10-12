// event.cpp
// Alex Hergenhan 2024
// defines class event

#include <iostream>
#include <bitset>
#include <cstdint> // for uint16_t

# include "event.h"

event::event(){
    hits.resize(8);

    std::cout << "Default constructor" << std::endl;
    for (int i = 0; i < 8; i++){
        hits[i].layer = 999;
        hits[i].wire = 999;
        hits[i].TDC = 999;
    }
}


event::event(char (&buffer)[16]){
    hits.resize(8);
    // loop through the buffer and create hits
    hit new_hit;
    for (int i = 0; i < 14; i+=2){     
        // use a uint16_t to store the 2 bytes of data (reccomended by chatgpt)
        uint16_t data = (static_cast<uint16_t>(buffer[i]) << 8) | static_cast<uint16_t>(buffer[i+1]);
        std::cout<<data<<std::endl;
        uint16_t mask = 0b111; // mask for the lowest 3 bits
        new_hit.layer = data & mask;
        mask = 0b111000; // mask for the next 3 bits
        new_hit.wire = (data & mask) >> 3;
        mask = 0b111111000000; // mask for the final 10 bits
        new_hit.TDC = (data & mask) >> 6;


        std::cout << "Layer: " << new_hit.layer << ", Wire: " << new_hit.wire << ", TDC: " << new_hit.TDC << std::endl;
        hits[i] = new_hit;
    }

    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire << ", TDC " << hits[i].TDC << std::endl;
    }

    std::cout << "Event created" << std::endl;
}

void event::print() const {
    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire << ", TDC " << hits[i].TDC << std::endl;
    }
}








// end event.cpp
