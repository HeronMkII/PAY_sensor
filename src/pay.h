#ifndef PAY_H
#define PAY_H

// AVR libraries
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <util/delay.h>

// Standard libraries
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// lib-common libraries
#include <can/can.h>
#include <can/ids.h>
#include <can/data_protocol.h>
#include <queue/queue.h>
#include <spi/spi.h>
#include <uart/uart.h>
#include <adc/adc.h>
#include <adc/pay.h>
#include <pex/pex.h>
#include <pex/pay.h>
#include <conversions/conversions.h>

// PAY libraries
#include "general.h"
#include "can_interface.h"
#include "can_commands.h"
#include "boost.h"
#include "motors.h"
#include "heaters.h"
#include "optical_spi.h"
#include "env_sensors.h"

extern adc_t adc1;
extern adc_t adc2;
extern pex_t pex1;
extern pex_t pex2;

void init_pay(void);
void process_next_rx_msg(void);
void send_next_tx_msg(void);


#endif
