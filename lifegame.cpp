#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "Habitat.hpp"

const uint REFRESH_RATE = 1000000;  // time to next generation in microseconds
const uint RANDOM_SEED = 4711;      // start value for random pattern generator
const uint PIXEL_DENSITY = 32;      // random value filter divisor

int main() {

    //setup
    stdio_init_all();
    i2c_init(i2c1, 400000);                 //Initialize I2C on i2c1 port with 400kHz
    gpio_set_function(2, GPIO_FUNC_I2C);    //Use GPIO2 as I2C
    gpio_set_function(3, GPIO_FUNC_I2C);    //Use GPIO3 as I2C
    gpio_pull_up(2);                        //Pull up GPIO2
    gpio_pull_up(3);                        //Pull up GPIO3

    // create instance of Conway's Game of Life generator
    Habitat habitat(0x3C, Size::W128xH64, i2c1);
    
    // habitat.randomFill(RANDOM_SEED, PIXEL_DENSITY); // start with random habitat
    habitat.drawHorizontalLine(2,2,10, Colors::WHITE);
    // habitat.display(); // display start habitat

    while(true) 
    {
        // set timer for next generation build
        auto nextPeriod = make_timeout_time_us(REFRESH_RATE);
        auto started = get_absolute_time();
        habitat.nextGen(); // build next generation
        habitat.display(); // and display it

        // sleep until next generation build 
        printf("%lld ms ", absolute_time_diff_us(get_absolute_time(), started)/1000);
        sleep_until(nextPeriod);
    }
    return 0;
}