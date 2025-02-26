/**
 * @file mode.hpp
 * @author Arya Bhandari
 
 * @brief Paylod Software modes and transitions
 * @date 2022-01-24
 */

#ifndef MODE_HPP
#define MODE_HPP

#include <cstdint>
#include <string>

/**
 * Abstract mode class, this does things we want to do every single cycle, e.g. write to sd card
 */

class Mode {
public:

    /**
     * Updates the current state by polling all sensors. Writes that state to
     * the SD card and transmits it over radio.
     */
    virtual void execute();

    /**
     * Checks for valid transition criteria and updates the current flight mode
     * in state, if necessary.
     */
    virtual void transition() = 0;

    /**
     * Switches to the specified flight mode and updates
     * the boot file, if able.
     * @param mode The flight mode to switch to
     */
    void to_mode(FlightMode *mode);

    /**
     * A short integer ID for storage and transmission purposes.
     */
    virtual uint8_t id() = 0;

    /**
     * A verbose name for debugging purposes.
     */
    virtual std::string name() = 0;
};

/**
 * Standby mode: Default mode, all output channels low. 
 * Waiting for bluetooth signal.
 */
class Standby : public Mode {
public:
    void execute();

    void transition();

    uint8_t id() { return 1; }

    std::string name() { return "Standby"; };

};

/**
 * Tminus5 mode: Start Pico timer (to control future transitions). 
 * Turn nozzle pump output channel high
 */
class Tminus5 : public Mode {
public:
    void execute();

    void transition();

    uint8_t id() { return 2; }

    std::string name() { return "Tminus5"; };
    
};

/**
 * Tminus2 mode: Turn valve output channel high
 */
class Tminus2 : public Mode {
public:
    void execute();

    void transition();

    uint8_t id() { return 3; }

    std::string name() { return "Tminus2"; };

};

/**
 * Apogee mode
 */
class Apogee : public Mode {
public:
    void execute();

    void transition();

    uint8_t id() { return 4; }

    std::string name() { return "Apogee"; };

};
 
/**
 * Touchdown mode
 */
class Touchdown : public Mode {
public:
    void execute();

    void transition();

    uint8_t id() { return 5; }

    std::string name() { return "Touchdown"; };
}


#endif