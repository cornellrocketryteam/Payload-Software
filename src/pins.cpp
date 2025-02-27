/**
 * @file pins.cpp
 * @author anb78
 *
 * @brief Pin initialization functionality
 */

#include "pins.hpp"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

void init_pins() {

// cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
// sleep_ms(250);
// cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

// Init GPIO pins
gpio_init(VACCUM_PUMP);
gpio_init(VALVE);
gpio_init(VENTURI_PUMP);
gpio_init(ANALYSIS_OUT_1);
//gpio_init(ANALYSIS_OUT_2);

//GPIO directions
gpio_set_dir(VACCUM_PUMP, GPIO_OUT);
gpio_set_dir(VALVE, GPIO_OUT);
gpio_set_dir(VENTURI_PUMP, GPIO_OUT);
gpio_set_dir(ANALYSIS_OUT_1, GPIO_OUT);
//gpio_set_dir(ANALYSIS_OUT_2, GPIO_OUT);

gpio_put(VENTURI_PUMP, 0);
gpio_put(VALVE, 0);
gpio_put(VACCUM_PUMP, 0);
gpio_put(ANALYSIS_OUT_1, 0);
//gpio_put(ANALYSIS_OUT_2, 0);

}