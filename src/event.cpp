// event.cpp
// Alex Hergenhan 2024
// defines class event

# include "event.h"


event::event(char (&buffer)[16]){
    // loop through the buffer and create hits
    for (int i = 0; i < 8; i++){
        // create a new hit from the buffer
        hit new_hit;

        // first three bits of a byte is 0x07 (0000 0111)
        new_hit.layer = buffer[i] & 0x07;

        // now we want the next three bits, so we shift the byte 3 bits to the right, and take the lowest 3 bits with 0x07 again
        new_hit.wire = (buffer[i] >> 3) & 0x07;

        // now we want the final 10 bits, so we take the last 2 bits of the current byte, and the next 8 bits from the next byte
        new_hit.TDC = (buffer[i] >> 6) | (buffer[i+1] << 2) | (buffer[i+2] << 10);
        
        hits[i] = new_hit;
    }
}

void event::print() const {
    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire << ", TDC " << hits[i].TDC << std::endl;
    }
}








// end event.cpp
