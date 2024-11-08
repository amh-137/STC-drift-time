// main.cpp
// Alex Hergenhan 2024
// 

#include <iostream>
#include <string>
#include <fstream>
#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>


#include "event.h"
#include "hit.h"
#include "event-helpers.h"

int open_file(std::string fname, std::ifstream& inp); // returns 0 if success
void read_event(std::ifstream &inp, int line, event &ev); // returns 0 if success


int main(int argc, char *argv[]){
    // not plotting event displays by default
    int plot_frequency = 1000000000;
    
     if (argc == 2){
        // ploting the event displays
        plot_frequency = std::stoi(argv[1]);
    } else if (argc > 2){ 
        std::cerr<<"Too many arguments"<<std::endl;
        plot_frequency = 1000000000;
        return 1;
    }
    

    // Look at file onetrack
    std::ifstream file;
    int stat = open_file("data/onetrack.raw", file);
    if (stat != 0){
        return 1;
    }

    // read the first event
    event ev;
    read_event(file, 0, ev);

    //ev.print();
    int tmp = 0;
    ev.geometry(tmp);
    ev.plot();

    std::cout<<"For the single track file:"<<std::endl;
    std::cout<<"> Velocity: "<<std::abs(ev.get_v_best()) / SCALE * 10000<<std::endl;
    std::cout<<"> Theta: "<<ev.get_theta_best()<<std::endl;
    // Do not remove!
    file.close(); std::cout<<"File closed"<<std::endl;


    /* Look at manytracks */    
    int stat2 = open_file("data/manytracks.raw", file);
    if (stat2 != 0){
        return 1;
    }


    TFile *f = new TFile("data/distributions.root", "RECREATE");
    TH1F *h = new TH1F("v", "Distribution of v", 100, 30., 80.);
    TH1F *h2 = new TH1F("theta", "Distribution of angle", 30, -3.2, 3.2);

    double v_best_arr[10000];
    double theta_best_arr[10000];

    event ev2;
    int n_evt_failed = 0;
    for (int i = 0; i < 1000000; i++) {
        read_event(file, i, ev2);
        ev2.geometry(n_evt_failed);
        if (!(i % plot_frequency)) {
            ev2.plot();
        }

        v_best_arr[i%10000] = std::abs(ev2.get_v_best()) / SCALE * 10000;
        theta_best_arr[i%10000] = ev2.get_theta_best();

        if (i%10000==0){
            std::cout<<i<<" events processed - writing to histogram"<<std::endl;
            for (int j = 0; j < 10000; j++) {
                h->Fill(v_best_arr[j]);
                h2->Fill(theta_best_arr[j]);
            }
        }
    }
    file.close(); std::cout<<"File maytracks.raw closed"<<std::endl;

    h->Write();
    h2->Write();
    
    /* Fitting */
    // create fit functions
    TF1 *fit_v = new TF1("gaus_v", "gaus", 40., 70.);
    TF1 *fit_theta = new TF1("gaus_theta", "gaus", -3.2, 3.2);

    h->Fit("gaus_v");
    h2->Fit("gaus_theta");

    // print the results
    std::cout << "Fit Velocity mu: " << fit_v->GetParameter(1) << " +/- " << fit_v->GetParError(1) << std::endl;
    std::cout << "Fit Velocity sigma" << fit_v->GetParameter(2) << " +/- " << fit_v->GetParError(2) << std::endl;
    std::cout << "Fit Angle mu: " << fit_theta->GetParameter(1) << " +/- " << fit_theta->GetParError(1) << std::endl;
    std::cout << "Fit Angle sigma: " << fit_theta->GetParameter(2) << " +/- " << fit_theta->GetParError(2) << std::endl;

    // draw these fits on the histogram
    TCanvas *c = new TCanvas("c", "c", 800, 600);
    h->Draw();
    fit_v->SetLineColor(kRed);
    fit_v->Draw("same");
    c->SaveAs("plots/velocity_fit.png");

    TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
    h2->Draw();
    fit_theta->SetLineColor(kRed);
    fit_theta->Draw("same");
    c2->SaveAs("plots/angle_fit.png");

    std::cout<<"Number of events that did not minimise: "<<n_evt_failed<<std::endl;

    f->Close();

    return 0;
}




int open_file(std::string fname, std::ifstream& inp) {
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
