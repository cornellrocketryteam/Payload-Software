/**
 * @file service_implementation.h
 * @author anb78
 *
 * @brief BLE GATT service implementation for the air sampler
 */
#ifndef SERVICE_IMPLEMENTATION_H
#define SERVICE_IMPLEMENTATION_H

#include <stdint.h>
#include "btstack.h"

#ifdef __cplusplus
extern "C" {
#endif

// Constants for service and characteristic UUIDs (16-bit format)
#define PAYLOAD_SERVICE_UUID      0xFF10  // Custom service UUID
#define TIMESTAMP_CHARACTERISTIC_UUID 0xFF11  // Timestamp characteristic UUID

// External flags that can be checked from other files
extern bool ble_start_command_received;
extern uint32_t received_timestamp;

/**
 * @brief Initialize the GATT service
 */
void payload_service_init(void);

/**
 * @brief Process packet events for the GATT service
 */
void payload_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif // SERVICE_IMPLEMENTATION_H