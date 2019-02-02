/*
General-use functions for PAY.
*/

#include "general.h"


// Initializes everything in PAY
void init_pay(void) {
    // UART
    init_uart();

    // SPI
    init_spi();

    // PEX
    init_pex(&pex);

    // ADC
    init_adc(&adc);

    // DAC
    init_dac(&dac);

    // Environmental sensors
    temp_init();
    hum_init();
    pres_init();

    // PAY-Optical
    init_opt_spi();

    // Motors
    init_motors();

    // Queues
    init_queue(&rx_msg_queue);
    init_queue(&tx_msg_queue);

    // CAN and MOBs
    init_can();
    init_rx_mob(&cmd_rx_mob);
    init_tx_mob(&data_tx_mob);
}


// If there is an RX messsage in the queue, handle it
void process_next_rx_msg(void) {
    if (!queue_empty(&rx_msg_queue)) {
        handle_rx_msg();
    }
}


/*
If there is a TX message in the queue, sends it

When resume_mob(mob name) is called, it:
1) resumes the MOB
2) triggers an interrupt (callback function) to get the data to transmit
3) sends the data
4) pauses the mob
*/
void send_next_tx_msg(void) {
    if (!queue_empty(&tx_msg_queue)) {
        resume_mob(&data_tx_mob);
    }
}
