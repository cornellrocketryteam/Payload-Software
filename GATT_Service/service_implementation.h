#ifndef SERVICE_IMPLEMENTATION_H
#define SERVICE_IMPLEMENTATION_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// BTstack includes for att_server_init, etc.
#include "btstack.h"
#include "btstack_defines.h"
#include "btstack_event.h"
#include "btstack_util.h"

// The auto-generated GATT database from your .gatt file:
#include "PAYLOAD.h"  // e.g. created by pico_btstack_make_gatt_header(...)

#ifdef __cplusplus
extern "C" {
#endif

// Global flags – declared extern here, define them once in main.cpp
extern bool     ble_start_command_received;
extern uint32_t received_timestamp;

/**
 * First, define the read/write callbacks as static inline *outside* of init_gatt_service.
 * They cannot be nested inside the function body.
 */

// Read callback
static inline uint16_t gatt_read_callback(hci_con_handle_t con_handle,
    uint16_t att_handle,
    uint16_t offset,
    uint8_t * buffer,
    uint16_t buffer_size)
    
{   
    printf("GATT read callback triggered\n");
    UNUSED(con_handle);
    UNUSED(att_handle);
    UNUSED(offset);
    UNUSED(buffer);
    UNUSED(buffer_size);
    
    // For a minimal approach, we can do nothing here if we let the
    // auto-generated GATT table handle default reads.
    // If you wanted custom read logic, you could implement it here.
    return 0;
}

// Write callback
static inline int gatt_write_callback(hci_con_handle_t con_handle,
                        uint16_t att_handle,
                        uint16_t transaction_mode,
                        uint16_t offset,
                        uint8_t * buffer,
                        uint16_t buffer_size)
{
    printf("GATT write callback triggered\n");
    UNUSED(con_handle);
    UNUSED(transaction_mode);
    UNUSED(offset);

    if (att_handle == ATT_CHARACTERISTIC_0xFF11_01_VALUE_HANDLE) {
        if (buffer_size >= 4) {
            // Parse a 32-bit timestamp
            received_timestamp = (buffer[0]) |
                                 (buffer[1] << 8) |
                                 (buffer[2] << 16) |
                                 (buffer[3] << 24);
            ble_start_command_received = true;
            printf("Received timestamp: %u -> Sequence triggered\n", received_timestamp);
        }
    }
    return 0; 
}

static inline void init_gatt_service(void) {
    // Make sure PAYLOAD_gatt_database and PAYLOAD_gatt_database_len exist 
    // in the auto-generated file from your .gatt. 
    att_server_init(profile_data,
    &gatt_read_callback,
    &gatt_write_callback);

    printf("GATT service initialized from .gatt file\n");
}

#ifdef __cplusplus
}
#endif

#endif // SERVICE_IMPLEMENTATION_H
