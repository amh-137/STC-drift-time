// main.cpp
// Alex Hergenhan 2024
// 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TH1F.h>


#include "event.h"
#include "hit.h"
#include "event-helpers.h"

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
    
    // from arguments later
    int plot_frequency = 1000000000;

    std::ifstream file;
    int stat = open_file("data/onetrack.raw", file);
    if (stat != 0){
        return 1;
    }

    // read the first event
    event ev;
    read_event(file, 0, ev);

    //ev.print();
    ev.geometry();
    ev.plot();

    // Do not remove!
    file.close(); std::cout<<"File closed"<<std::endl;


    
    int stat2 = open_file("data/manytracks.raw", file);
    if (stat2 != 0){
        return 1;
    }


    TFile *f = new TFile("data/v.root", "RECREATE");
    TH1F *h = new TH1F("v", "Distribution of v", 250, 40., 70.);

    double v_best_arr[10000]; // 100 events for now for small samples while its still slow.

    event ev2;
    for (int i = 0; i < 1000000; i++) {
        read_event(file, i, ev2);
        ev2.geometry();
        if (!(i % plot_frequency)) {
            ev2.plot();
        }

        // real velocity = v_best * SCALE / 2 [cm/ns]
        // real velocity [um/ns] = v_best * SCALE / 2 * 
        v_best_arr[i%10000] = std::abs(ev2.get_v_best()) / SCALE * 10000;

        if (i%10000==0){
            std::cout<<i<<" events processed - writing to histogram"<<std::endl;
            for (int j = 0; j < 10000; j++) {
                h->Fill(v_best_arr[j]);
            }
        }
    }

    h->Write();
    f->Close();

    //delete f;
    //delete h;
    
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
    return;
}




// end main.cpp
