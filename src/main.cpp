/**
 * @file main.cpp
 * @author anb78
 * 
 * @brief The entry point for the program
 */

#include <cstdio>
#include "pico/stdlib.h"
#include "pins.hpp"
#include "mode.hpp"
#include "constants.hpp"
#include "hardware/watchdog.h"
#include "sd.hpp"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"

#include "btstack_config.h"
#include "btstack.h"

#include "gap_config.h"

// BTstack objects
static btstack_packet_callback_registration_t hci_event_callback_registration;

bool ble_connected = false;

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    init_pins();
    printf("pins initialised\n");

    // Initialize SD card
    printf("Initializing SD card...\n");
    if (!sd_card.begin()) {
        printf("Warning: SD card initialization failed\n");
    }

    // inform about BTstack state
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // // turn on bluetooth!
    hci_power_control(HCI_POWER_ON);

    //watchdog_enable(constants::watchdog_delay, true);

    // Start in standby mode
    current_mode = new Standby();
    current_mode->execute();  
    // printf("System initialized in Standby mode\n");

    while (true) {
        // printf("Hello, world!\n");

        if (current_mode) {
            current_mode->transition();
        }

        // current_mode->transition();

        
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(200);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(200);
        
        // Small delay to avoid consuming too much CPU
        // This delay is not critical for timing accuracy 
        // since we're using hardware timer for sequence timing
        //sleep_ms(10);

        //watchdog_update();
    }

    return 0;
}