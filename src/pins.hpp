/**
 * @file pins.hpp
 * @author anb78
 *
 * @brief GPIO pin definitions for the Pico
 */

#ifndef PINS_HPP
#define PINS_HPP

#define LED 25

#define VACCUM_PUMP 27
#define VALVE 29
#define VENTURI_PUMP 31
#define ANALYSIS_OUT_1 32
#define ANALYSIS_OUT_2 34

/**
 * Initializes all GPIO, I2C, SPI, UART, and PWM pins
 */
void init_pins();

#endif