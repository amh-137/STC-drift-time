// main.cpp
// Alex Hergenhan 2024
// 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
//#include <TTree.h> // include ROOT

#include "event.h"

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
    ev.print();
    read_event(file, 0, ev);
    file.close();
    std::cout<<"Hi"<<std::endl;
    ev.print();
    



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


    // Get the size of the file
    inp.seekg(0, std::ios::end);
    std::streamsize size = inp.tellg();
    inp.seekg(0, std::ios::beg);

    // Create a buffer to hold the data
    std::vector<char> buffer(size);

    // Read the file
    inp.read(buffer.data(), size);

    // print it for now
    for (size_t i = 0; i < buffer.size(); i++) {
            //std::cout << static_cast<double>(buffer[i]) << " ";
            data.push_back(static_cast<double>(buffer[i]));
        }
        //std::cout << std::endl;

    inp.close();

    return 0;
}



// end main.cpp
