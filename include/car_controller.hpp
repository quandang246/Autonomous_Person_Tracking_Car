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
    // IN4 - 12 - 79
    // IN3 - 13 - 14
    // IN2 - 16 - 232
    // IN1 - 18 - 15
    // ENA - 33 - 38
    // ENB - 32 - 168

    std::vector<int> channels = {12, 13, 16, 18, 33, 32}; // or std::vector<std::string>

public:
    car();
    ~car();

    void turn_right();
    void turn_left();
    void go_forward();
    void go_backward();
    void refresh();
};

#endif  // CAR_CONTROLLER_HPP
