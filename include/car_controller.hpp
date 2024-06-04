#ifndef CAR_CONTROLLER_HPP
#define CAR_CONTROLLER_HPP

#include <iostream>
// for delay function.
#include <chrono>
#include <thread>

// for signal handling
#include <signal.h>

// JetsonGPIO lib
#include <JetsonGPIO.h>

using namespace std;

static bool end_this_program = false;

inline void delay(int s) { this_thread::sleep_for(chrono::seconds(s)); }

class car
{
private:
    // Pinout (Jetson nano)
    // Motor 1 - 36, 38 / 40
    // Motor 2 - 19, 21 / 23
    // Motor 3 - 22, 24 / 26
    // Motor 4 - 11, 13 / 15 

    std::vector<int> channels = {36, 38, 40, 19, 21, 23, 22, 24, 26, 11, 13, 15}; // or std::vector<std::string>

public:
    car();
    ~car();

    void go_forward();
    void go_backward();

    void sideways_right();
    void sideways_left();
    
    void diagonally_top_left();
    void diagonally_top_right();
    void diagonally_bottom_right();
    void diagonally_bottom_left();

    void rotation();
    
    void refresh();

};

#endif  // CAR_CONTROLLER_HPP
