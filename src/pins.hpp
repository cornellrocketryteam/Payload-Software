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

#define SD_SCK 2
#define SD_CMD 3
#define SD_D0 4
#define SD_D1 5
#define SD_D2 6
#define SD_D3 7

/**
 * Initializes all GPIO, I2C, SPI, UART, and PWM pins
 */
void init_pins();

#endif