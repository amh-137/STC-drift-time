// main.cpp
// Alex Hergenhan 2024
// 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include <TTree.h> // include ROOT
#include <TGraph.h>
#include <TCanvas.h>

#include "event.h"
#include "hit.h"

int read_file(std::string fname, std::vector<double>& data); // returns 0 if success

int open_file(std::string fname, std::ifstream& inp); // returns 0 if success
void read_event(std::ifstream &inp, int line, event &ev); // returns 0 if success



// print a vector of any type
template <typename T>
void vprint(std::vector<T> v) {
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}


#include <TEllipse.h>
void event_img(event ev){

    TCanvas c("canvas", "Event Display", 800, 800);
    

    c.cd();
    TGraph g;
    g.SetTitle("Event;x (cm);y (cm)");

    // bottom left @ (1,1)
    for (int i=0; i<8; i++){
        for (int j=0; j<8; j++){
            // if its an even row, plot normally
            if (i%2 == 0){
                g.AddPoint(i+1, j+1);
            }
            else {
                g.AddPoint(i+1, j+1.5);
            }
        }
    }

    TGraph g2;

    for (int i=0; i<8; i++){
        double x, y;
        conv_hit_to_coords(ev[i], x, y);
        g2.AddPoint(x, y);
    }

    g2.SetMarkerColor(kRed); // Marker color red

    g.Draw("AP*");
    g2.Draw("P* same");


    // get the lines
    std::vector<line> lvec;
    ev.geometry(lvec);

    for (int i=0; i<7; i++){

        double x0, y0, x1, y1;
        conv_hit_to_coords(ev[i], x0, y0);
        conv_hit_to_coords(ev[i+1], x1, y1);
        double f1, f2;

        for (int j=0; j<4; j++){
            line l = lvec[i*4 + j];
            // plot between x0-1 to x1+1
            // two points define a straight line!
            f1 = l.m * (x0-1.) + l.c;
            f2 = l.m * (x1+1.) + l.c;
            std::cout<<"x0 "<<x0<<" f0 "<<f1<<" x1 "<<x1<<" f2 "<<f1<<std::endl;
            double x[2] = {x0-1., x1+1.};
            double y[2] = {f1, f2};
            TGraph* g3 = new TGraph(2, x, y);
            g3->Draw("L same");
        }

        // draw the circles
        double r1 = ev[i].TDC / 2000.;
        TEllipse* e1 = new TEllipse(x0, y0, r1, r1);
        e1->SetFillStyle(0);
        e1->Draw("same");
    }

    c.SaveAs("plots/event.png");
}



int main(){
    
    std::ifstream file;
    int stat = open_file("data/onetrack.raw", file);
    if (stat != 0){
        return 1;
    }

    // read the first event
    event ev;
    read_event(file, 0, ev);

    ev.print();

    event_img(ev);
    

    file.close();

    std::cout<<"File closed"<<std::endl;


    return 0;
}







int open_file(std::string fname, std::ifstream& inp) {
    // open file
    inp.open(fname, std::ios::binary);

    // if file doesnt exit / wrong path : return 1
    if (!inp) {
        std::cerr << "Failed to open file: " << fname << std::endl;
        return 1;
    }
    return 0;
}


void read_event(std::ifstream &inp, int line, event &ev) {
    // unfortunately cannot read in directly to 16 bit types like int16_t
    char buffer[16];
    inp.seekg(line * 16); // go to the line
    inp.read(buffer, 16);
    ev = event(buffer);
    std::cout << "Event read" << std::endl;
    return;
}









// Old

int read_file(std::string fname, std::vector<double>& data) {
    // open file
    std::ifstream inp(fname, std::ios::binary);

    // if file doesnt exit / wrong path : return 1
    if (!inp) {
        std::cerr << "Failed to open file: " << fname << std::endl;
        return 1;
    }

    std::cout<<"\n!!";
    // Get the size of the file
    inp.seekg(0, std::ios::end);
    std::streamsize size = inp.tellg();
    inp.seekg(0, std::ios::beg);

    // Create a buffer to hold the data
    std::vector<char> buffer(size);

    std::cout<<"\n!!";

    // Read the file
    inp.read(buffer.data(), size);

    std::cout<<"\n!!";


    // print it for now
    for (size_t i = 0; i < buffer.size(); i++) {
            //std::cout << static_cast<double>(buffer[i]) << " ";
            data.push_back(static_cast<double>(buffer[i]));
        }
        //std::cout << std::endl;
    
    std::cout<<"\n!!";

    inp.close();


    return 0;
}



// end main.cpp
