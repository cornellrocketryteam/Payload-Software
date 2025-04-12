/**
 * @file mode.hpp
 * @author Arya Bhandari
 * 
 * @brief Paylod Software modes and transitions
 * @date 2022-01-24
 */

#include "mode.hpp"
#include "pins.hpp"
#include "hardware/gpio.h"
// #include "hardware/clocks.h"
#include "pico/stdlib.h"
#include "constants.hpp"
#include "sd.hpp"

#include "gap_config.h"
#include "service_implementation.h"

// Define the current mode pointer
Mode* current_mode = nullptr;

// Global sequence time - measured in milliseconds since sequence start
volatile uint32_t run_time_ms = 0;
bool sequence_running = false;

// Hardware timer alarm ID
#define SEQUENCE_TIMER_ALARM 0

// 1ms timer callback for global sequence timing
int64_t sequence_timer_callback(alarm_id_t id, void* user_data) {

    run_time_ms += 10;
    
    if (run_time_ms % 1000 == 0) {
        printf("Timer: %d ms\n", run_time_ms);
    }

    // Repeat the timer (return ms delay before next firing)
    return 10000; // 1ms period
}


// Start the global hardware timer
void start_sequence_timer() {
    // Reset the sequence time
    run_time_ms = 0;
    sequence_running = true;
    
    // Start the hardware timer with 1ms period
    add_alarm_in_ms(10, sequence_timer_callback, NULL, true);
    
    printf("Sequence timer started.\n");
}


void Mode::reset_outputs() {
    gpio_put(VACCUM_PUMP, 0);
    gpio_put(VALVE, 0);
    gpio_put(VENTURI_PUMP, 0);
    gpio_put(ANALYSIS_OUT_1, 0);
}

//void Mode::execute() {}

//void Mode::transition() {}


void Mode::to_mode(Mode *mode) {
    // Log the transition
    printf("Transitioning from %s to %s at %d ms\n", 
           current_mode ? current_mode->name().c_str() : "None", 
           mode->name().c_str(),
           run_time_ms);

    // Log to SD card if available
    if (current_mode) {
        sd_card.log_transition(current_mode->name(), mode->name(), 
                                run_time_ms);
    } else {
        sd_card.log_transition("None", mode->name(), 
                                run_time_ms);
    }
    
    // Update the current mode
    if (current_mode != nullptr && current_mode != mode) {
        delete current_mode;  // Clean up previous mode
    }
    
    current_mode = mode;
    
    // Execute the new mode immediately
    if (current_mode) {
        current_mode->execute();
    }
}


void Standby::execute() {
    printf("Entered Standby\n");
    // Reset all outputs to ensure known state
    reset_outputs();
    
    // add_alarm_in_ms(1, sequence_timer_callback, NULL, true);
    
}

void Standby::transition() {
    // printf("transition from standby called\n");
    // Check if it's time to transition to Tminus5
    if (ble_start_command_received) {
        ble_start_command_received = false;
        to_mode(new Tminus5());
    }
}

void Tminus5::execute() {
    printf("Entered Tminus5\n");
    reset_outputs();

    gpio_put(VENTURI_PUMP, 1);
    start_sequence_timer();
    printf("ended execute of tminus5\n");
    
}

void Tminus5::transition() {
    //printf("entered transition of tminus5\n");
    //printf("Tminus5 transition check: run_time_ms=%d, threshold=%d\n", 
        // run_time_ms, TMINUS2_START_TIME);
    // Check if it's time to transition to Tminus2
    if (sequence_running && run_time_ms >= TMINUS2_START_TIME) {
        to_mode(new Tminus2());
    }
}

void Tminus2::execute() {
    printf("Entered Tminus2 at T+%d ms\n", run_time_ms);
    
    // Keep venturi pump on and turn valve on
    gpio_put(VENTURI_PUMP, 1);
    gpio_put(VALVE, 1);
}

void Tminus2::transition() {
    // Check if it's time to transition to PreApogee
    if (sequence_running && run_time_ms >= PREAPOGEE_START_TIME) {
        to_mode(new PreApogee());
    }
}

void PreApogee::execute() {
    printf("Entered PreApogee at T+%d ms\n", run_time_ms);
    
    // Reset all outputs to low
    reset_outputs();
}

void PreApogee::transition() {
    // Check if it's time to transition to Touchdown
    if (sequence_running && run_time_ms >= TOUCHDOWN_START_TIME) {
        to_mode(new Touchdown());
    }
}

void Touchdown::execute() {
    printf("Entered Touchdown at T+%d ms\n", run_time_ms);
    
    // Reset all outputs first
    reset_outputs();
    
    // Then set vacuum pump and analysis out high
    gpio_put(VACCUM_PUMP, 1);
    gpio_put(ANALYSIS_OUT_1, 1);
    
    printf("Final state reached - all sampling complete\n");
}


void Touchdown::transition() {}