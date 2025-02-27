#ifndef GAP_CONFIG_H //INCLUDE STATEMENT OR NOT?
#define GAP_CONFIG_H

#include <stdio.h>
#include <cstdio>
#include "btstack.h"
//#include "ble_gap.h"

#define APP_AD_FLAGS 0x06 // 0x06 = General discoverable, BR/EDR not supported

extern bool ble_connected;

static uint8_t adv_data[] = {
    // Flags
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    // Name
    0x05, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'P', 'i', 'c', 'o', //Should I Change?
    // Custom 16-bit service UUID
    0x03, BLUETOOTH_DATA_TYPE_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS, 0x10, 0xFF, //check UUID, should I change?
};
static const uint8_t adv_data_len = sizeof(adv_data);

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    UNUSED(size);
    UNUSED(channel);
    bd_addr_t local_addr;
    if (packet_type != HCI_EVENT_PACKET) return;

    // Retrieve event type from HCI packet
    uint8_t event_type = hci_event_packet_get_type(packet);

    // Switch on event type . . .
    switch(event_type){
        // Setup GAP advertisement
        case BTSTACK_EVENT_STATE:{
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
            gap_local_bd_addr(local_addr);
            printf("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));
            // setup advertisements
            uint16_t adv_int_min = 320; //what interval do I need??
            uint16_t adv_int_max = 320;
            uint8_t adv_type = 0;
            bd_addr_t null_addr;
            memset(null_addr, 0, 6);
            gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
            //assert(adv_data_len <= 31); // ble limitation
            gap_advertisements_set_data(adv_data_len, (uint8_t*) adv_data);
            gap_advertisements_enable(1);

            break;
        }
        // Disconnected from a client
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            break;
        // Ready to send ATT
        case ATT_EVENT_CAN_SEND_NOW:
        printf("disconnected\n");
        case HCI_EVENT_LE_META:{ //for logging connection
            uint8_t subevent_code = hci_event_le_meta_get_subevent_code(packet);
            switch (subevent_code) {
                case HCI_SUBEVENT_LE_CONNECTION_COMPLETE: {
                    // use macros to parse fields
                    uint8_t status = hci_subevent_le_connection_complete_get_status(packet);
                    if (status == ERROR_CODE_SUCCESS){
                        ble_connected = true;
                        printf("Connection complete\n");
                        // uint16_t conn_handle = hci_subevent_le_connection_complete_get_connection_handle(packet);

                        // bd_addr_t peer_addr;
                        // reverse_bd_addr(hci_subevent_le_connection_complete_get_peer_address(packet), peer_addr); //check error

                        // uint8_t role = hci_subevent_le_connection_complete_get_role(packet);
                        // uint8_t peer_addr_type = hci_subevent_le_connection_complete_get_peer_address_type(packet);

                        // log to sd card

                    } else {
                        // connection failed, log sd card?
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

#endif