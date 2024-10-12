#include <iostream>

struct SimpleData {
    int x;
    float y;
    char z[16];
};

int main() {
    SimpleData d1 = {42, 3.14f, "Hello"};
    SimpleData d2 = d1; // Uses default copy constructor
    SimpleData d3;
    d3 = d1; // Uses default copy assignment operator

    std::cout << "d1: " << d1.x << ", " << d1.y << ", " << d1.z << std::endl;
    std::cout << "d2: " << d2.x << ", " << d2.y << ", " << d2.z << std::endl;
    std::cout << "d3: " << d3.x << ", " << d3.y << ", " << d3.z << std::endl;

    d1.x = 100;
    d1.y = 2.718f;
    d1.z[0] = 'W';

    std::cout << "d1: " << d1.x << ", " << d1.y << ", " << d1.z << std::endl;
    std::cout << "d2: " << d2.x << ", " << d2.y << ", " << d2.z << std::endl;
    std::cout << "d3: " << d3.x << ", " << d3.y << ", " << d3.z << std::endl;


    return 0;
}