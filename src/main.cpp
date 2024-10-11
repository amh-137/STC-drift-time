// main.cpp
// Alex Hergenhan 2024
// 

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// include ROOT
#include <TTree.h>

int read_file(std::string fname, std::vector<double>& data); // returns 0 if success


// print a vector of any type
template <typename T>
void vprint(std::vector<T> v) {
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

int main(){

    std::vector<double> data;

    int stat {read_file("data/onetrack.raw", data)};
    
    if (stat != 0){
        return 1;
    }

    vprint(data);

    return 0;
}




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
