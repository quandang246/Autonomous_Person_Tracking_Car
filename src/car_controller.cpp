#include "car_controller.hpp"

car::car()
{

    std::cout << "Car is initializing!" << std::endl;

    // Pin Setup.
    // Board pin-numbering scheme
    GPIO::setmode(GPIO::BOARD);
    // set pin as an output pin with optional initial state of HIGH
    GPIO::setup(channels, GPIO::OUT, {GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW});
    
    // Refresh pin
    refresh();
}
car::~car()
{
    refresh();
    GPIO::cleanup();
}

void car::turn_right()
{
    std::cout << "1. Turn right!" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::LOW, GPIO::HIGH, GPIO::HIGH, GPIO::HIGH});
}

void car::turn_left()
{
    std::cout << "2. Turn left!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::HIGH, GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
}

void car::go_forward()
{
    std::cout << "3. Go forward!" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
}

void car::go_backward()
{
    std::cout << "4. Go backward!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::HIGH, GPIO::HIGH});
}

void car::refresh()
{
    GPIO::output(channels, {GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW});
}
