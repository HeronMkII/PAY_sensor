/*

	FILENAME: 			analog_temp.c
	DEPENDENCIES:		spi, pgmspace, delay, stdint

	DESCRIPTION:		Functions for measuring thermistor temperature
	AUTHORS:			Dylan Vogel, James Sheridan
	DATE MODIFIED:	    2017-12-29

	NOTE:               Implement more efficient channel reading algorithm,
                        instead of having to waste SPI frames from waiting for
                        next message
                        Test edge cases

	REVISION HISTORY:

        2017-12-29      DV: Implemented lookup table with flash memory
        2017-12-28      DV: Created file, implemented lookup table, resistance
                        calculation.

*/

#include "analog_temp.h"

void init_thermistor(void){
    // Initialize COM_RST pin on 32M1 as ADC CS
    init_spi();

    init_cs(COM_RST, &COM_RST_DDR);
    set_cs_high(COM_RST, &COM_RST_PORT);
}

uint16_t read_thermistor_adc(int channel){
    // Returns the raw ADC value for the specified channel
    // Assumes channel is one of 0-6
    uint16_t data;
    uint8_t ADC_cmd, channel_bits;

    if (channel % 2 == 0){
        channel_bits = (uint8_t)(channel / 2);
    } else {
        channel_bits = 0b100  | (uint8_t)(channel / 2);
    }

    ADC_cmd = THERMISTOR_BASE | (channel_bits << 4);

    set_cs_low(COM_RST, &COM_RST_PORT);
    send_spi(ADC_cmd);
    set_cs_high(COM_RST, &COM_RST_PORT);

    // Wait for the ADC to finish conversion (pg 4 on datasheet)
    _delay_ms(1);

    set_cs_low(COM_RST, &COM_RST_PORT);
    // Resend command. Could consider enabling nap mode to save power.
    data = send_spi(ADC_cmd);
    data <<= 8;
    data |= send_spi(0x00);
    data >>= 4;
    set_cs_high(COM_RST, &COM_RST_PORT);

    return data;
}

double convert_thermistor_reading(uint16_t adc_reading, int channel){
    // Converts the ADC reading into a thermistor resistance
    // Assumes the thermistor is wired in series with a resistor of resistance
    // R_REF, using the 2.5V ADC reference voltage
    // See: https://www.allaboutcircuits.com/projects/measuring-temperature-with-an-ntc-thermistor/
    //
    // Equation is as follows, after rearringing the voltage divider equation:
    // R = R0 [(ADC_max_value / ADC_read_value) - 1]

    double res;

    res = (double)(1<<12) / adc_reading;
    res = (double)(R_REF[channel]) / 1000.0 * (res - 1.0);

    return res;
}


double resistance_to_temp(double res){
    // Convert the measured thermistor (NXRT15XH103FA1B040) resistance to temperature
    // Lookup table from manufacturer datasheet (pg 31)

    int i;
    double diff, slope, temp = 0;

    // Resistances are stored in kilo-ohms
    // PROGMEM instructs the compiler to store these values in flash memory
    static const float resistances[34] PROGMEM = {
        195.652,    148.171,    113.347,    87.559, 68.237,
        53.650,     42.506,     33.892,     27.219, 22.021,
        17.926,     14.674,     12.081,     10.000, 8.315,
        6.948,      5.834,      4.917,      4.161,  3.535,
        3.014,      2.586,      2.228,      1.925,  1.669,
        1.452,      1.268,      1.110,      0.974,  0.858,
        0.758,      0.672,      0.596,      0.531
    };
    static const int temps[34] PROGMEM = {
        -40,    -35,    -30,    -25,    -20,
        -15,    -10,    -5,     0,      05,
        10,     15,     20,     25,     30,
        35,     40,     45,     50,     55,
        60,     65,     70,     75,     80,
        85,     90,     95,     100,    105,
        110,    115,    120,    125
    };

    for (i = 0; i < 34; i++){
        if ((res - pgm_read_float(&resistances[i])) >= 0){
            diff = res - pgm_read_float(&resistances[i-1]);  //should be negative
            slope = (pgm_read_word(&temps[i]) - pgm_read_word(&temps[i-1]));
            slope /= (pgm_read_float(&resistances[i]) - pgm_read_float(&resistances[i-1]));
            temp = pgm_read_word(&temps[i-1]) + (diff * slope);
            break;
        }
    }
    return temp;
}
