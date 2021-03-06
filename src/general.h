#ifndef GENERAL_H
#define GENERAL_H

// Standard libraries
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// lib-common libraries
#include <adc/adc.h>
#include <can/can.h>
#include <can/data_protocol.h>
#include <can/ids.h>
#include <conversions/conversions.h>
#include <heartbeat/heartbeat.h>
#include <pex/pex.h>
#include <queue/queue.h>
#include <spi/spi.h>
#include <uart/uart.h>
#include <uptime/uptime.h>
#include <watchdog/watchdog.h>

// PAY libraries
#include "can_commands.h"
#include "can_interface.h"
#include "devices.h"
#include "env_sensors.h"
#include "motors.h"
#include "optical_spi.h"
#include "boost.h"

void init_pay(void);

#endif
