// event.cpp
// Alex Hergenhan 2024
// defines class event

#include <iostream>
#include <bitset>
#include <cstdint> // for uint16_t

# include "event.h"

event::event(){

    std::cout << "Default constructor" << std::endl;
    for (int i = 0; i < 8; i++){
        hits[i].layer = 999;
        hits[i].wire = 999;
        hits[i].TDC = 999;
    }
}


event::event(char (&buffer)[16]){
    // sizeof(char) = 1
    // sizeof(uint16_t) = 2
    
    // loop through the buffer and create hits
    hit new_hit;
    for (int i = 0, j=0; i < 16; i+=2, j++){
        // use a uint16_t to store the 2 bytes of data!
        uint16_t line { static_cast<uint16_t>( (static_cast<uint16_t>(buffer[i]) << 8) | (static_cast<uint16_t>(buffer[i+1])) ) };
        //std::cout<<std::bitset<8>(buffer[i])<<" "<<std::bitset<8>(buffer[i+1])<<std::endl;
        //std::cout<<std::bitset<16>(line)<<std::endl;

        // first three bits is line
        new_hit.layer = line & 0b111;

        // next three bits is wire
        new_hit.wire = (line & 0b111000) >> 3;

        // final 10 bits is TDC
        new_hit.TDC = (line & 0b1111111111000000) >> 6;

        //std::cout << "Layer: " << new_hit.layer << ", Wire: " << new_hit.wire << ", TDC: " << new_hit.TDC << std::endl;
        hits[j] = new_hit;
    }

    /*
    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire << ", TDC " << hits[i].TDC << std::endl;
    }*/
}

void event::print() const {
    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire;
        std::cout << ", TDC " << hits[i].TDC << " (raw) -> " << hits[i].TDC / 2;
        std::cout << " to " << (static_cast<double>(hits[i].TDC) + 1.) / 2. << " bin (ns)."<< std::endl;
    }   
}







// end event.cpp
