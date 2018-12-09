#include <adc/adc.h>
#include <adc/pay.h> //Includes constants
#include <conversions/conversions.h>
#include <spi/spi.h>
#include <uart/uart.h>

#include "../../src/devices.h"
#include "../../src/thermistors.h"


// Get the resistance of the thermistor given the voltage
// For equation, see: https://www.allaboutcircuits.com/projects/measuring-temperature-with-an-ntc-thermistor/
double thermis_voltage_to_resistance(double voltage) {
    return THERMIS_R_REF * (THERMIS_V_REF / voltage - 1);
}

int main(void){
    init_uart();
    print("\n\nUART initialized\n");

    init_spi();
    print("SPI Initialized\n");

    init_adc(&adc);
    print("ADC Initialized\n");

    print("\nStarting test\n\n");

    //Number of thermistor channels to measure temperature (max 10)
    uint8_t thermister_channel_num = 3;

    //Find the temperature given the voltage output from the adc
    while (1) {
        //Read all of the thermistors' voltage from adc
        fetch_all(&adc);

        //temperature for each thermistor-channel
        double thermis_temperature[ADC_CHANNELS];

        //Find resistance for each channel
        //only calculate it for thermistors (channel 10 and 11 are something else)
        for (uint8_t i = 0; i < thermister_channel_num; i++) {
            print("Channel %d\n", i);
            uint16_t raw_data = read_channel(&adc, i);
            double voltage = adc_raw_data_to_raw_voltage(raw_data);
            print("voltage (Volts): %f\n", voltage);

            //Convert adc voltage to resistance of thermistor
            double resistance = thermis_voltage_to_resistance(voltage);
            print("resistance (kilo Ohmns): %f\n", resistance);

            //Convert resistance to temperature of thermistor
            thermis_temperature[i] = thermis_resistance_to_temp(resistance);
            print("thermistor temperature (C): %f\n\n", thermis_temperature[i]);
        }

        _delay_ms(10000);
    }

    return 0;
}
