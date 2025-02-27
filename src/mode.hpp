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
#include "pico/stdlib.h"
// #include "hardware/timer.h"

// Forward declaration for current_mode variable
class Mode;
extern Mode* current_mode;

//flag for ble connected 
//extern bool connected;

extern volatile uint32_t run_time_ms;
extern bool sequence_running;

// Hardware timer alarm ID
// #define SEQUENCE_TIMER_ALARM 0

// Timer callback declaration
int64_t sequence_timer_callback(alarm_id_t id, void* user_data);

// Start the global hardware timer
void start_sequence_timer();

/**
 * Abstract mode class, this does things we want to do every single cycle, e.g. write to sd card
 */

class Mode {
public:

    /**
     * Updates the current state by polling all sensors. Writes that state to
     * the SD card and transmits it over radio.
     */
    virtual void execute() = 0;

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
    void to_mode(Mode *mode);

    /**
     * A short integer ID for storage and transmission purposes.
     */
    virtual uint8_t id() = 0;

    /**
     * A verbose name for debugging purposes.
     */
    virtual std::string name() = 0;

    /**
     * Resets all output channels to low state
     */
    void reset_outputs();

    /**
     * mode destructor
     */
    virtual ~Mode() {}
};

/**
 * Standby mode: Default mode, all output channels low. 
 * Waiting for bluetooth signal.
 */
class Standby : public Mode {
public:
    void execute() override;

    void transition() override;

    uint8_t id() override{ return 1; }

    std::string name() override { return "Standby"; };

};

/**
 * Tminus5 mode: Start Pico timer (to control future transitions). 
 * Turn nozzle pump output channel high
 */
class Tminus5 : public Mode {
public:
    void execute() override;

    void transition() override;

    uint8_t id() override { return 2; }

    std::string name() override { return "Tminus5"; };
    
};

/**
 * Tminus2 mode: Turn valve output channel high
 */
class Tminus2 : public Mode {
public:
    void execute() override ;

    void transition() override;

    uint8_t id() override { return 3; }

    std::string name() override { return "Tminus2"; };

};

/**
 * PreApogee mode
 */
class PreApogee : public Mode {
public:
    void execute() override;

    void transition() override;

    uint8_t id() override { return 4; }

    std::string name() override { return "PreApogee"; };

};
 
/**
 * Touchdown mode
 */
class Touchdown : public Mode {
public:
    void execute() override;

    void transition() override;

    uint8_t id() override { return 5; }

    std::string name() override { return "Touchdown"; };
};


#endif