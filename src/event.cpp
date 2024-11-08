// event.cpp
// Alex Hergenhan 2024
// defines class event

#include <iostream>
#include <bitset>
#include <cstdint> // for uint16_t
# include <cmath>

#include <TLine.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TEllipse.h>

# include "event.h"
# include "event-helpers.h"

/* CLASS DEPENDANT HELPER FUNCTIONS DEFINTIONS*/
// for other, non class-dependant helper functions, see event-helpers.cpp & event-helpers.h
double f(const event& ev, double v, int n_tangent, int n_c1, int n_c2);
double dfdv(double (*f)(const event&, double, int, int, int), const event& ev, double v, int n_tangent, int n_c1, int n_c2, double step);
double minimise(double (*f)(const event&, double, int, int, int), const event* ev, int n_tangent, int n_c1, int n_c2, double step, double lbound, double ubound, double v_init);
double secant_minimise(double (*f)(const event&, double, int, int, int), const event* ev, int n_tangent, int n_c1, int n_c2, double step, double tol, double v_init, int& n_evt_failed);


/* CLASS EVENT DEFINITIONS */

// static member variable
int event::count = 0;


event::event(){
    v_best = 0.;
    best_tangent = 5;
    theta_best = 361.;
    count++;
    for (int i = 0; i < 8; i++){
        hits[i].layer = 999;
        hits[i].wire = 999;
        hits[i].TDC = 999.;
    }
}


event::event(char (&buffer)[16]){
    v_best = 0.;
    best_tangent = 5;
    theta_best = 361.;
    count++;
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
        
        uint16_t rh = static_cast<uint16_t>(uch1);
        uint16_t lh = static_cast<uint16_t>(uch2) << 8;
        uint16_t line = rh | lh;
        //std::cout<<std::bitset<16>(line)<<std::endl;

        // first three bits is line
        new_hit.layer = line & 0b111;

        // next three bits is wire
        new_hit.wire = (line & 0b111000) >> 3;

        // final 10 bits is TDC
        uint16_t TDC = (line & 0b1111111111000000) >> 6;
        new_hit.TDC = (static_cast<double>(TDC) * 0.5) + 0.25;
        // This is important -> use the MIDDLE of the TDC bin

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


void event::geometry(int& n_evt_failed){
    // get the largest circles
    int i, j;
    get_two_largest_circles(i, j);

    // minimise f with respect to v for all 4 tangent lines!
    double v;
    double step = 0.01;
    double v_init = 9.;

    double d_best = 1e16;

    for (int n_tangent = 0; n_tangent < 4; n_tangent++){
        //v = minimise(f, this, n_tangent, i, j, step, lbound, ubound, v_init); // slower method.
        v = secant_minimise(f, this, n_tangent, i, j, step, 1e-3, v_init, n_evt_failed);
        double d_curr = f(*this, v, n_tangent, i, j);
        if (d_curr < d_best){
            v_best = v;
            best_tangent = n_tangent;
            d_best = d_curr;
            
            line l;
            get_two_circle_tangent(circle(hits[i].layer, hits[i].wire, hits[i].TDC / SCALE * v), circle(hits[j].layer, hits[j].wire, hits[j].TDC / SCALE * v), l, best_tangent);
            double m {-l.a/l.b};
            theta_best = std::atan(m);
        }
    }
}


void event::plot() const{
    // will use TGraph TEllips and TLine to plot the event + best fit of lines.
    std::string text = "Event ID" + std::to_string(count);
    // could use TLateX to make it prettier
    TCanvas c("canvas", "Event Display", 800, 800);
    c.cd();
    c.PaintText(0.1, 0.9, text.c_str()); // Desn't work for some reason
    TGraph g;
    g.SetTitle("Event;x (cm);y (cm)");

    // bottom left @ (0,0)
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            if (i%2 == 0){
                g.AddPoint(i, j);
            }
            else {
                g.AddPoint(i, j+.5);
            }
        }
    }
    g.Draw("AP*");

    // plot the circles with v_best
    for (int i=0; i<8; i++){
        double x, y;
        conv_hit_to_coords(hits[i], x, y);
        double r = hits[i].TDC / SCALE * v_best;
        TEllipse* e = new TEllipse(x, y, r, r);
        e->SetFillStyle(1001);
        e->SetFillColor(kBlue);
        e->Draw("same");
    }

    // plot the tangent line
    double x0, y0, x1, y1;
    int i, j;
    get_two_largest_circles(i, j);
    conv_hit_to_coords(hits[i], x0, y0);
    conv_hit_to_coords(hits[j], x1, y1);
    circle c1(x0, y0, hits[i].TDC / SCALE * v_best);
    circle c2(x1, y1, hits[j].TDC / SCALE * v_best);
    line l;
    get_two_circle_tangent(c1, c2, l, best_tangent);
    
    TLine* tl = new TLine(0, -l.c/l.b, 8, -(l.a*8 + l.c)/l.b);
    tl->SetLineColor(kRed);
    tl->Draw("same");
    
    std::string fname {"plots/ev-display/secant-" + std::to_string(count) + ".png"};
    c.SaveAs(fname.c_str());

    delete tl;
}




/* CLASS EVENT DEPENDANT HELPER FUNCTIONS */
// see event-helpers.cpp for the non class-dependent functions


/** @brief
 *  function to be minimised
 * @param v: velocity of particle
 * @param n_tangent: number of tangent line (0-3)
 * @param n_c1: number of largest circle (0-7)
 * @param n_c2: number of second largest circle (0-7)
**/
double f(const event& ev, double v, int n_tangent, int n_c1, int n_c2){
    // setup circles with TDC scaled by v
    double x0, y0, x1, y1, x2, y2;
    conv_hit_to_coords(ev.hits[n_c1], x0, y0);
    conv_hit_to_coords(ev.hits[n_c2], x1, y1);

    circle c1(x0, y0, ev.hits[n_c1].TDC / SCALE * v);
    circle c2(x1, y1, ev.hits[n_c2].TDC / SCALE * v);

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
            conv_hit_to_coords(ev.hits[i], x2, y2);
            circle c(x2, y2, ev.hits[i].TDC / SCALE * v);
            double cld = circle_line_distance(c, l);
            total_distance += cld*cld;
        }
    }
    return total_distance;
}


double dfdv(double (*f)(const event&, double, int, int, int), const event& ev, double v, int n_tangent, int n_c1, int n_c2, double step){
    // f(x + h) - f(x) / h
    return (f(ev, v + step, n_tangent, n_c1, n_c2) - f(ev, v, n_tangent, n_c1, n_c2)) / (step);
}


double minimise(double (*f)(const event&, double, int, int, int), const event* ev, int n_tangent, int n_c1, int n_c2, double step, double lbound, double ubound, double v_init){
    //int n_itters = 0;
    // for now, gradient descent
    double v = v_init;
    double grad = dfdv(f, *ev, v, n_tangent, n_c1, n_c2, step);
    while (std::abs(grad) > 1e-1){
        v -= grad * step;
        grad = dfdv(f, *ev, v, n_tangent, n_c1, n_c2, step);
        //std::cout<<"v: "<<v<<" grad: "<<grad<<std::endl;
        //n_itters++;
    }
    return v;
}


double secant_minimise(double (*f)(const event&, double, int, int, int), const event* ev, int n_tangent, int n_c1, int n_c2, double step, double tol, double v_init, int& n_evt_failed){
    // secant method for minimsation https://www.sciencedirect.com/science/article/pii/S0096300306002530
    double v = v_init;
    double grad = dfdv(f, *ev, v, n_tangent, n_c1, n_c2, step);
    double grad_prev = dfdv(f, *ev, v - step, n_tangent, n_c1, n_c2, step);
    int n_itters = 0;
    int max_itters = 100000;
    while (std::abs(grad) > 1e-3 && n_itters < max_itters){
        double grad_diff = grad - grad_prev;
        v -= grad * step / grad_diff;
        grad_prev = grad;
        grad = dfdv(f, *ev, v, n_tangent, n_c1, n_c2, step);
        n_itters++;
    }

    if (n_itters >= max_itters){
        //std::cout<<"!! Secant method did not converge !! ev = "<<ev->get_count()<<std::endl;
        n_evt_failed++;
        return 0.;
    }
    return v;

}


// end event.cpp
