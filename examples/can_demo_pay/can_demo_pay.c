#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <uart/uart.h>
#include <uart/log.h>
#include <can/can.h>

void rx_callback(uint8_t*, uint8_t);

mob_t rx_mob = {
    .mob_num = 0,
    .mob_type = RX_MOB,
    .dlc = 8,
    .id_tag = { 0x0000 },
    .id_mask = { 0x0000 },
    .ctrl = default_rx_ctrl,
    .rx_cb = rx_callback
};

void print_bytes(uint8_t *data, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        print("0x%02x ", data[i]);
    }
    print("\n");
}

void rx_callback(uint8_t* data, uint8_t len) {
    print("RX received!\n");
    print("%s\n", (char *) data);

    print("Received Data:\n");
    print_bytes(data, len);
}

int main(void) {
    init_uart();
    print("UART initialized\n");

    init_can();
    init_rx_mob(&rx_mob);

    print("Waiting for TX\n");
    while (1) {}
}
