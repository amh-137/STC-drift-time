// event.cpp
// Alex Hergenhan 2024
// defines class event

#include <iostream>
#include <bitset>
#include <cstdint> // for uint16_t
# include <cmath>

#include <TLine.h>

# include "event.h"

// a constant scale factor to make TDCs not overlap during minimsation
const int SCALE {1000};

void conv_hit_to_coords(hit h, double &x, double &y){
    x = h.layer;
    y = h.wire + 0.5*(h.layer%2);
}

void get_two_circle_tangent(circle c1, circle c2, line l, int n){
    double i, j;
    // apparently switch statements are slightly faster than if else
    // get our 4 tangent lines with this:
    switch (n)
    {
    case 0:
        i = 1.; j = 1.; break;

    case 1:
        i = 1.; j = -1.; break;

    case 2:
        i = -1.; j = 1.; break;
    
    case 3:
        i = -1.; j = -1.; break;
    
    default:
        i = 1.; j = 1.; break;
    }

    double r1 {c1.r * i};
    double r2 {c2.r * j};

    double dr {r2 - r1};

    double x = c2.x0 - c1.x0;
    double y = c2.y0 - c1.y0;

    double z = x*x + y*y;
    double d = sqrt( std::abs(z - dr*dr) );

    // ax + by + c = 0
    l.a = (x*dr + y*d) / z;
    l.b = (y*dr - x*d) / z;
    l.c = r1 - c1.x0*l.a - c1.y0*l.b;
}

// shortest distance between line and circle
double circle_line_distance(circle c, line l){
    return std::abs(l.a*c.x0 + l.b*c.y0 + l.c) / sqrt(l.a*l.a + l.b*l.b);
}


double minimise(double (*f)(double, int, int, int), int n_tangent, int n_c1, int n_c2, double lbound, double ubound){
 
    return 0.;
}



/* CLASS EVENT DEFINITIONS */

event::event(){

    std::cout << "Default constructor" << std::endl;
    for (int i = 0; i < 8; i++){
        hits[i].layer = 999;
        hits[i].wire = 999;
        hits[i].TDC = 999;
    }
}


event::event(char (&buffer)[16]){
    // sizeof(char) = 1, sizeof(uint16_t) = 2
    
    // loop through the buffer and create hits
    hit new_hit;
    for (int i = 0, j=0; i < 16; i+=2, j++){
        // use a uint16_t to store the 2 bytes of data!

        // So this is a massive pain, but to ensure the conversion is correct, I need to cast the char to an unsigned char
        // to avoid the sign bit being set to 1
        unsigned char uch1 { static_cast<unsigned char>(buffer[i]) };
        unsigned char uch2 { static_cast<unsigned char>(buffer[i+1]) };
        // You can get around this with std::bit_cast in C++20 or memcpy
        // https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used

        //std::cout<<std::bitset<8>(uch1)<<" "<<std::bitset<8>(uch2)<<std::endl;
        
        uint16_t rh = static_cast<uint16_t>(uch1);
        uint16_t lh = static_cast<uint16_t>(uch2) << 8;
        uint16_t line = rh | lh;
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
}

void event::print() const {
    for (int i = 0; i < 8; i++){
        std::cout << "Hit " << i << ": Layer " << hits[i].layer << ", Wire " << hits[i].wire;
        std::cout << ", TDC " << hits[i].TDC << " (raw) -> " << hits[i].TDC / 2;
        std::cout << " to " << (static_cast<double>(hits[i].TDC) + 1.) / 2. << " bin (ns)."<< std::endl;
    }   
}

// get the index of the two highest tdcs in event.hits
void event::get_two_largest_circles(int& i, int& j) const {
    // this would be easiest with std::max_element
    // would need to convert hits to a vector or std::array
    i = 0;
    j = 1;
    for (int k=0; k<8; k++){
        if (hits[k].TDC > hits[i].TDC){
            j = i;
            i = k;
        } else if (hits[k].TDC > hits[j].TDC){
            j = k;
        }
    }
}



double event::f(double v, int n_tangent, int n_c1, int n_c2){
    // setup circles with TDC scaled by v
    double x0, y0, x1, y1, x2, y2;
    conv_hit_to_coords(hits[n_c1], x0, y0);
    conv_hit_to_coords(hits[n_c2], x1, y1);

    circle c1(x0, y0, hits[n_c1].TDC / SCALE * v);
    circle c2(x1, y1, hits[n_c2].TDC / SCALE * v);

    // get the tangent line
    line l;
    get_two_circle_tangent(c1, c2, l, n_tangent);

    // total_distance - what we want to minimise
    double total_distance = 0.;


    for (int i=0; i<8; i++){
        if (i == n_c1 || i == n_c2){
            // we know this is 0 as the tangent was drawn to these circles
            continue;
        } else {
            conv_hit_to_coords(hits[i], x2, y2);
            circle c(x2, y2, hits[i].TDC / SCALE * v);
            c.r = hits[i].TDC / SCALE * v;
            total_distance += circle_line_distance(c, l);
        }
    }
    return total_distance;
}


void event::geometry() const{
    // get the largest circles
    int i, j;
    get_two_largest_circles(i, j);

    // minimise f with respect to v for all 4 tangent lines!
    
}



// end event.cpp
