// event.cpp
// Alex Hergenhan 2024
// defines class event

#include <iostream>
#include <bitset>
#include <cstdint> // for uint16_t
# include <cmath>

#include <TLine.h>

# include "event.h"



void conv_hit_to_coords(hit h, double &x, double &y){
    // bottom left @ (1,1)
    x = h.layer + 1;
    y = h.wire + 1;
    if (h.layer % 2 == 1){
        y += 0.5;
    }
}


double sqrt(double x){
    return std::pow(x, 0.5);
}



void get_two_circle_tangents(circle c1, circle c2, line l[4]){
    // Solutions to the tangents of two circles is derived here
    // https://cp-algorithms.com/geometry/tangents-to-two-circles.html

    // 4 tangent solutions in general hence l[4]
    for (int i=0; i<4; i++){
        double c1r = c1.r;
        double c2r = c2.r;
        if (i == 0){
            c1r = -c1.r;
            c2r = -c2.r;
        } else if (i == 1){
            c1r = -c1.r;
        } else if (i == 2){
            c2r = -c2.r;
        }
        // else the normal solution

        double dr {c2r - c1r};


        double den { c2.x0*c2.x0 + c2.y0*c2.y0 };
        double a = {( (dr*c2.x0) + c2.y0*sqrt(den - dr*dr) ) / den};
        double b = {( (dr*c2.y0) + c2.x0*sqrt(den - dr*dr) ) / den};
        double c = c1r - a*c1.x0 - b*c1.y0;

        l[i].m = -a/b;
        l[i].c = -c/b;
        std::cout<<"l["<<i<<"].m: "<<l[i].m<<", l["<<i<<"].c: "<<l[i].c<<std::endl;
    }
    //std::cout<<"end\n";
}







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

        // So this is a massive pain, but to ensure the conversion is correct, I need to cast the char to an unsigned char
        // to avoid the sign bit being set to 1
        unsigned char uch1 { static_cast<unsigned char>(buffer[i]) };
        unsigned char uch2 { static_cast<unsigned char>(buffer[i+1]) };
        // I think you can get around this with std::bit_cast in C++20
        // or memcpy
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


#include <TGraph.h>

// MIGHT BE UNUSED
void linspace(std::vector<double>& v, double min, double max, int n){
    v.resize(n); // more efficient than pushing it back a bunch
    double spacing = (max - min) / (double)n;
    v[0] = min;
    for (int i=1; i<n; i++){
        v[i] = v[i-1] + spacing;
    }
}

// y = mx + c
// MIGHT BE UNUSED
void f_linear(std::vector<double>& x, std::vector<double> y, double m, double c){
    y.resize(x.size());
    for (size_t i=0; i<x.size(); i++){
        y[i] = m*x[i] + c;
    }
}

void event::geometry(std::vector<line>& lvec) const{
    line l[4];

    for (int i=0; i<7; i++){
        std::cout<<"i: "<<i<<std::endl;
        double x0, y0, x1, y1;
        conv_hit_to_coords(hits[i], x0, y0);
        conv_hit_to_coords(hits[i+1], x1, y1);


        // TDC / 1025 to make sure the circles dont overlap
        //(im using 2cm or sqrt(5)cm as the seperation between circles)
        circle c1 = {x0, y0, hits[i].TDC / 2000.};
        circle c2 = {x1, y1, hits[i+1].TDC / 2000.};

        get_two_circle_tangents(c1, c2, l);

        for (int j=0; j<4; j++){
            lvec.push_back(l[j]);
        }
    }
}









// end event.cpp
