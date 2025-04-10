/**
 * @file service_implementation.c
 * @author anb78
 *
 * @brief Implementation of BLE GATT service for the air sampler
 */
#include "service_implementation.h"
#include "btstack_defines.h"
#include "ble/att_db.h"
#include "ble/att_server.h"
#include "btstack_util.h"
#include <stdio.h>
#include <string.h>

// Create service and characteristic UUIDs
static uint8_t service_uuid[16];
static uint8_t timestamp_uuid[16];

// Create service and characteristic handles
static uint16_t service_start_handle = 0;
static uint16_t service_end_handle = 0;
static uint16_t timestamp_value_handle = 0;
static uint16_t timestamp_desc_handle = 0;

// Create ATT DB storage
static uint8_t timestamp_value[8];
static const char timestamp_desc[] = "Timestamp";

// Create ATT service handler
static att_service_handler_t service_handler;

// Flags for command state
bool ble_start_command_received = false;
uint32_t received_timestamp = 0;

// Read callback for characteristic values
static uint16_t att_read_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t offset, uint8_t *buffer, uint16_t buffer_size) {
    UNUSED(connection_handle);
    
    // Handle read of timestamp value
    if (att_handle == timestamp_value_handle) {
        return att_read_callback_handle_blob(timestamp_value, sizeof(timestamp_value), offset, buffer, buffer_size);
    }
    
    // Handle read of timestamp description
    if (att_handle == timestamp_desc_handle) {
        return att_read_callback_handle_blob((uint8_t*)timestamp_desc, strlen(timestamp_desc), offset, buffer, buffer_size);
    }
    
    return 0;
}

// Write callback for characteristic values
static int att_write_callback(hci_con_handle_t connection_handle, uint16_t att_handle, uint16_t transaction_mode, uint16_t offset, uint8_t *buffer, uint16_t buffer_size) {
    UNUSED(transaction_mode);
    UNUSED(offset);
    UNUSED(connection_handle);
    
    // Handle write to timestamp value
    if (att_handle == timestamp_value_handle) {
        // Ensure we have enough data for a timestamp
        if (buffer_size >= 4) {
            // Copy data to our local buffer
            memcpy(timestamp_value, buffer, buffer_size < sizeof(timestamp_value) ? buffer_size : sizeof(timestamp_value));
            
            // Parse timestamp (32-bit little-endian integer)
            received_timestamp = buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24);
            printf("Received timestamp: %u - Starting sequence\n", received_timestamp);
            
            // Set flag to indicate start command
            ble_start_command_received = true;
        }
        return 0;
    }
    
    return 0;
}

// Initialize the service
void payload_service_init(void) {
    // Create UUIDs
    uuid_create_from_uuid16(service_uuid, PAYLOAD_SERVICE_UUID);
    uuid_create_from_uuid16(timestamp_uuid, TIMESTAMP_CHARACTERISTIC_UUID);
    
    // Allocate range in ATT DB for service
    uint16_t service_size = 6; // Service + characteristic + value + descriptor
    att_db_util_add_service_uuid128(service_uuid);
    
    // Add timestamp characteristic
    timestamp_value_handle = att_db_util_add_characteristic_uuid128(
        timestamp_uuid,
        ATT_PROPERTY_READ | ATT_PROPERTY_WRITE | ATT_PROPERTY_WRITE_WITHOUT_RESPONSE,
        ATT_SECURITY_NONE,
        ATT_SECURITY_NONE,
        timestamp_value,
        sizeof(timestamp_value)
    );
    
    // Add timestamp description
    timestamp_desc_handle = att_db_util_add_characteristic_user_description(
        (const char*)timestamp_desc,
        ATT_SECURITY_NONE
    );
    
    // Get service handle range
    service_start_handle = att_db_util_get_service_start_handle();
    service_end_handle = att_db_util_get_service_end_handle();
    
    // Register service handler
    service_handler.start_handle = service_start_handle;
    service_handler.end_handle = service_end_handle;
    service_handler.read_callback = &att_read_callback;
    service_handler.write_callback = &att_write_callback;
    att_server_register_service_handler(&service_handler);
    
    printf("Air Sampler GATT Service initialized\n");
    printf("  Service: 0x%04X - 0x%04X\n", service_start_handle, service_end_handle);
    printf("  Timestamp value handle: 0x%04X\n", timestamp_value_handle);
    printf("  Timestamp description handle: 0x%04X\n", timestamp_desc_handle);
}

// Process BLE packets
void payload_packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    UNUSED(channel);
    UNUSED(size);
    
    if (packet_type != HCI_EVENT_PACKET) return;
    
    uint8_t event_type = hci_event_packet_get_type(packet);
    
    switch (event_type) {
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            printf("BLE disconnected\n");
            break;
            
        case HCI_EVENT_LE_META:
            if (hci_event_le_meta_get_subevent_code(packet) == HCI_SUBEVENT_LE_CONNECTION_COMPLETE) {
                uint8_t status = hci_subevent_le_connection_complete_get_status(packet);
                if (status == ERROR_CODE_SUCCESS) {
                    printf("BLE connected\n");
                }
            }
            break;
            
        case ATT_EVENT_CAN_SEND_NOW:
            // We're not using notifications in this simplified implementation
            break;
            
        default:
            break;
    }
}