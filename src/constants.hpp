/**
 * @file constants.hpp
 * @author anb78
 *
 * @brief Constants and other definitions
 */

#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "pico/stdlib.h"

// Fixed transition times (milliseconds from sequence start)
//const uint32_t TMINUS5_START_TIME = 10000;     // t-5, will remove this when ble incorporated
const uint32_t TMINUS2_START_TIME = 3000;     // t-2 (t-5 + 3)
const uint32_t PREAPOGEE_START_TIME = 10000;   // t+x (t-2 + 2+x). X = 5 FOR NOW
const uint32_t TOUCHDOWN_START_TIME = 15000;   // t+x+y . Y = 5 FOR NOW



#endif