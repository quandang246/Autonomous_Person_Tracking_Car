/*
Copyright (c) 2019-2023, NVIDIA CORPORATION.
Copyright (c) 2019-2023, Jueon Park(pjueon) <bluegbgb@gmail.com>.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <JetsonGPIO.h>

using namespace std;

static bool end_this_program = false;

inline void delay(int s) { this_thread::sleep_for(chrono::seconds(s)); }

void signalHandler(int s) { end_this_program = true; }

void forward(int output_pins[], int num_pins)
{
    cout << "Forward! " << endl;
    GPIO::output(output_pins[0], GPIO::HIGH);
    GPIO::output(output_pins[1], GPIO::LOW);
    GPIO::output(output_pins[2], GPIO::HIGH);
    GPIO::output(output_pins[3], GPIO::LOW);

    // Code to set motor speed

}   

void backward(int output_pins[], int num_pins)
{
    cout << "Forward! " << endl;
    GPIO::output(output_pins[0], GPIO::LOW);
    GPIO::output(output_pins[1], GPIO::HIGH);
    GPIO::output(output_pins[2], GPIO::LOW);
    GPIO::output(output_pins[3], GPIO::HIGH);
    delay(1);

    // Code to set motor speed
}

void stop()
{
    // set motor speed to 0
}

void left
{
    
}

int main()
{
    // When CTRL+C pressed, signalHandler will be called
    signal(SIGINT, signalHandler);

    // Pin Definitions
    int output_pins[] = {18, 17, 20, 21}; // Example GPIO pins
    int num_pins = sizeof(output_pins) / sizeof(output_pins[0]);

    // Pin Setup
    GPIO::setmode(GPIO::BCM);
    for (int i = 0; i < num_pins; ++i)
    {
        // Set pins as output pins with an initial state of HIGH
        GPIO::setup(output_pins[i], GPIO::OUT, GPIO::LOW);
    }

    cout << "Starting demo now! Press CTRL+C to exit" << endl;

    // Array to hold current values of each pin
    int curr_values[num_pins];
    for (int i = 0; i < num_pins; ++i)
    {
        curr_values[i] = GPIO::LOW;
    }

    while (!end_this_program)
    {
        delay(1);
        // Toggle the output of each pin every second
        for (int i = 0; i < num_pins; ++i)
        {
            cout << "Outputting " << curr_values[i] << " to pin " << output_pins[i] << endl;
            GPIO::output(output_pins[i], curr_values[i]);
            curr_values[i] ^= GPIO::HIGH;
        }
    }

    GPIO::cleanup();

    return 0;
}
