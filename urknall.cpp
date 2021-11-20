#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "Habitat.hpp"


const uint LED_PIN = 25;            // Pico onboard LED
const uint  I2C_SDA_PIN = 2;        // I2C SDA (data)
const uint  I2C_SCL_PIN = 3;        // I2C SCL (clock)
const uint  START_STOP_PIN = 4;     // start/stop switch

const uint REFRESH_RATE = 250000;   // time to next generation in microseconds
const uint RANDOM_SEED = 4711;      // start value for random pattern generator
const uint PIXEL_DENSITY = 32;      // random value filter divisor

unsigned long time = to_ms_since_boot(get_absolute_time());
const int delayTime = 75;           // button debounce time in ms
bool running = false;

void debounce_interrupt(uint gpio, uint32_t events) {
    if ((to_ms_since_boot(get_absolute_time())-time)>delayTime) {
        time = to_ms_since_boot(get_absolute_time());
        running = !running;
        gpio_put(LED_PIN, running);
    }
}

int main() {
    stdio_init_all();
    i2c_init(i2c1, 400000);  //Initialize I2C on i2c1 port with 400kHz

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SDA_PIN);

    gpio_init(START_STOP_PIN);
    gpio_pull_up(START_STOP_PIN);   // enable start/stop switch

    // create instance of life generator
    Habitat habitat(0x3C, Size::W128xH64, i2c1);

    // show intro screen
    habitat.clear();
    habitat.drawString(40, 10,"<<Urknall>>", Colors::WHITE);
    habitat.drawString(40, 20, "hit button", Colors::WHITE);
    habitat.display();

    // enable button interrupts
    gpio_set_irq_enabled_with_callback(START_STOP_PIN, GPIO_IRQ_EDGE_FALL , true, &debounce_interrupt);
    while (!running) {
        sleep_ms(500);
    }

    // first generation: a r-pentomino grandfather
    habitat.clear();
    habitat.drawHorizontalLine(64, 32, 3, Colors::WHITE);  
    habitat.drawPixel(66, 31, Colors::WHITE);
    habitat.drawPixel(65, 34, Colors::WHITE);
    habitat.display();

    while(true) 
    {
        sleep_ms(200);
        if (running) {
            habitat.nextGen(); // build next generation
            habitat.display(); // and display it
        }
    }
    return 0;
}