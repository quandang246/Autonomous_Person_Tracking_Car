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

void signalHandler(int s) { end_this_program = true; }

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

    // Initialize PWM on the output pin with a frequency of 50 Hz
    GPIO::PWM a(enA, 50);
    GPIO::PWM b(enB, 50);

public:
    car();
    ~car();

    void turn_right();
    void turn_left();
    void go_forward();
    void go_backward();
    void refresh();
};
car::car()
{

    std::cout << "Car is initializing!" << std::endl;

    // Pin Setup.
    // Board pin-numbering scheme
    GPIO.setmode(GPIO.BOARD);
    // set pin as an output pin with optional initial state of HIGH
    GPIO::setup(channels, GPIO::OUT, {GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW});
}
car::~car()
{
    GPIO::cleanup();
}

void car::turn_right()
{
    std::cout << "1. Turn right!" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::LOW, GPIO::HIGH, GPIO::HIGH, GPIO::HIGH});
    delay(1);
    refresh();
}

void car::turn_left()
{
    std::cout << "2. Turn left!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::HIGH, GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
    delay(1);
    refresh();
}

void car::go_forward()
{
    std::cout << "3. Go forward!" << std::endl;
    GPIO::output(channels, {GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::HIGH});
    delay(1);
    refresh();
}

void car::go_backward()
{
    std::cout << "4. Go backward!" << std::endl;
    GPIO::output(channels, {GPIO::LOW, GPIO::HIGH, GPIO::LOW, GPIO::HIGH, GPIO::HIGH, GPIO::HIGH});
    delay(1);
    refresh();
}

void car::refresh()
{
    GPIO::output(channels, {GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW, GPIO::LOW});
}

int main()
{

    // When CTRL+C pressed, signalHandler will be called
    signal(SIGINT, signalHandler);

    // Initalize car
    car my_car;

    cout << "Strating demo now! Press CTRL+C to exit" << endl;
    std::cout << "1. Turn right!" << std::endl;
    std::cout << "2. Turn left!" << std::endl;
    std::cout << "3. Go forward!" << std::endl;
    std::cout << "4. Go backward!" << std::endl;

    while (!end_this_program)
    {
        int choice;
        std::cout << "Enter your choice to control the car: " << std::endl;
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            my_car.turn_right();
            break;
        case 2:
            my_car.turn_left();
            break;
        case 3:
            my_car.go_forward();
            break;
        case 4:
            my_car.go_backward();
            break;

        default:
            std::cout << "Invalid choice, please try again!" << std::endl;
            break;
        }

        delay(1);
    }

    return 0;
}
