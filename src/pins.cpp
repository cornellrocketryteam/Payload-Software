/**
 * @file pins.cpp
 * @author anb78
 *
 * @brief Pin initialization functionality
 */

#include "pins.hpp"
#include "constants.hpp"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

void init_pins() {

gpio_init(LED);
gpio_set_dir(LED, GPIO_OUT);
gpio_put(LED, 1);

// Init GPIO pins
gpio_init(VACCUM_PUMP);
gpio_init(VALVE);
gpio_init(VENTURI_PUMP);
gpio_init(ANALYSIS_OUT_1);
gpio_init(ANALYSIS_OUT_2);

//GPIO directions
gpio_set_dir(VACCUM_PUMP, GPIO_OUT);
gpio_set_dir(VALVE, GPIO_OUT);
gpio_set_dir(VENTURI_PUMP, GPIO_OUT);
gpio_set_dir(ANALYSIS_OUT_1, GPIO_OUT);
gpio_set_dir(ANALYSIS_OUT_2, GPIO_OUT);


}