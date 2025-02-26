/**
 * @file main.cpp
 * @author anb78
 * 
 * @brief The entry point for the program
 */

#include <cstdio>
#include "pico/stdlib.h"
#include "pins.hpp"

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"

#include "btstack_config.h"
#include "btstack.h"

#include "gap_config.h"

// BTstack objects
static btstack_packet_callback_registration_t hci_event_callback_registration;

int main() {
    stdio_init_all();

    init_pins();
    printf("pins initialised\n");

    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }

    // Initialize L2CAP and security manager
    //sm_init();

    // inform about BTstack state
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // // turn on bluetooth!
    hci_power_control(HCI_POWER_ON);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(5000);
    }

    return 0;
}