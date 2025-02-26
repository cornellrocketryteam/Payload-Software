/**
 * @file pins.hpp
 * @author anb78
 *
 * @brief GPIO pin definitions for the Pico
 */

#ifndef PINS_HPP
#define PINS_HPP

#define LED 25

#define VACCUM_PUMP 21
#define VALVE 22
#define VENTURI_PUMP 26
#define ANALYSIS_OUT_1 27
#define ANALYSIS_OUT_2 28

/**
 * Initializes all GPIO, I2C, SPI, UART, and PWM pins
 */
void init_pins();

#endif