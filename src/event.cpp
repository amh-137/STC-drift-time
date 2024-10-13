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

    std::cout<< "int: "<<sizeof(int) << " char: " << sizeof(char) << std::endl;


    hits.resize(8);
    // loop through the buffer and create hits
    hit new_hit;
    for (int i = 0; i < 14; i+=2){

        /*
        // use a uint16_t to store the 2 bytes of data (reccomended by chatgpt)
        std::cout<< static_cast<int>(buffer[i])<< " " << static_cast<int>(buffer[i+1]) << std::endl;
        std::cout << std::bitset<16>(buffer[i]) << " " << std::bitset<16>(buffer[i+1]) << std::endl;
        uint16_t data = (static_cast<uint16_t>(buffer[i]) << 8) | static_cast<uint16_t>(buffer[i+1]);
        std::cout<<data<<std::endl;
        std::cout<<std::bitset<16>(data)<<std::endl;
        uint16_t mask = 0b111; // mask for the lowest 3 bits
        new_hit.layer = data & mask;
        mask = 0b111000; // mask for the next 3 bits
        new_hit.wire = (data & mask) >> 3;
        mask = 0b111111000000; // mask for the final 10 bits
        new_hit.TDC = (data & mask) >> 6;
        */

        std::cout << std::bitset<8>(buffer[i]) << " " << std::bitset<8>(buffer[i+1]) << std::endl;
        new_hit.layer = buffer[i+1] & 0b111; // first three bits
        std::cout << (std::bitset<16>(buffer[i+1] & 0b111)) << std::endl;
        
        new_hit.wire = (buffer[i+1] & 0b111000) >> 3; // next three bits
        std::cout << std::bitset<16>((buffer[i+1] & 0b111000) >> 3) << std::endl;


        // this is a bit harder
        // ints are 16 bits. need the final 2 bits from buffer[i+1] and then the 8 bits from buffer[i] to make 1 10 bit value
        // store the 10 bit value in a 16 bit int!

        int rh = (buffer[i+1] & 0b11000000) >> 6;
        std::cout<<"rh "<<std::bitset<16>(rh)<<std::endl;
        int lh = (buffer[i] & 0b11111111) << 2;
        std::cout<<"lh "<<std::bitset<16>(lh)<<std::endl;
        int data = lh | rh;
        std::cout<<std::bitset<16>(data)<<std::endl;
        new_hit.TDC = data;


        //std::cout << "Layer: " << new_hit.layer << ", Wire: " << new_hit.wire << ", TDC: " << new_hit.TDC << std::endl;
        hits[i] = new_hit;
        std::cout<<"\n\n";
    }

    /*
    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire << ", TDC " << hits[i].TDC << std::endl;
    }*/
}

void event::print() const {
    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire << ", TDC " << hits[i].TDC << std::endl;
    }
}








// end event.cpp
