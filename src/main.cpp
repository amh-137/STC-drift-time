// main.cpp
// Alex Hergenhan 2024
// 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include <TTree.h> // include ROOT


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
    ev.geometry();
    ev.plot();

    //event_img(ev);
    

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




// end main.cpp
