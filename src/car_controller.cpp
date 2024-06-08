#include "car_controller.hpp"

car::car()
{

    std::cout << "Car is initializing!" << std::endl;

    // Pin Setup.
    // Board pin-numbering scheme
    GPIO::setmode(GPIO::BOARD);
    // set pin as an output pin with optional initial state of HIGH
    GPIO::setup(channels, GPIO::OUT, {GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW});

    // Refresh pin
    refresh();
}
car::~car()
{
    refresh();
    GPIO::cleanup();
}

void car::sideways_right()
{
    // std::vector<int> channels = {36, 38, 40, 19, 21, 23, 22, 24, 26, 11, 13, 15};
    // 1(1)/2(-1)/3(-1)/4(1)
    std::cout << "Moving sideways right!" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH});
    
    refresh();
}

void car::sideways_left()
{
    // 1(-1)/2(1)/3(1)/4(-1)
    std::cout << "Moving sideways left!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
    
    refresh();
}

void car::go_forward()
{
    // 1(1)/2(1)/3(1)/4(1)
    std::cout << "Moving forward!" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH});
    
    refresh();
}

void car::go_backward()
{
    // 1(-1)/2(-1)/3(-1)/4(-1)
    std::cout << "Moving backward!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
    
    refresh();
}

void car::diagonally_top_left()
{
    // 1(0)/2(1)/3(1)/4(0)
    std::cout << "Moving diagonally top left!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW});
    
    refresh();
}

void car::diagonally_top_right()
{
    // 1(1)/2(0)/3(0)/4(1)
    std::cout << "Moving diagonally top right!" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH});
    
    refresh();
}
void car::diagonally_bottom_right()
{
    // 1(-11)/2(0)/3(0)/4(-1)
    std::cout << "Moving diagonally bottom right!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
    
    refresh();
}
void car::diagonally_bottom_left()
{
    // 1(0)/2(-1)/3(-1)/4(0)
    std::cout << "Moving diagonally bottom left!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW});
    
    refresh();
}

void car::rotation()
{
    // 1(1)/2(-1)/3(1)/4(-1)
    std::cout << "Rotating" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH,
                            GPIO::HIGH, GPIO::LOW, GPIO::HIGH,
                            GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
    
    refresh();
}

void car::refresh()
{
    GPIO::output(channels, {GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW,
                            GPIO::LOW, GPIO::LOW, GPIO::LOW});
}
