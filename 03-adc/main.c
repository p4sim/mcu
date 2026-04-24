#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "stdio-task/stdio-task.h"
#include "protocol-task/protocol-task.h"
#include "adc-task/adc-task.h"   // <-- ЭТА СТРОКА ДОЛЖНА БЫТЬ
#include "stdint.h" 
#include "stdio.h"

void version_callback(const char* args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void get_adc_callback(const char* args)
{
    float voltage_V = adc_task_measure_voltage();
    printf("%f\n", voltage_V);
}

void get_temp_callback(const char* args)
{
    float temp_C = adc_task_measure_temperature();
    printf("%f\n", temp_C);
}

int main()
{
    stdio_init_all();
 

    api_t device_api[] = {
        {"version", version_callback, "get device name and firmware version"},
        {"get_adc", get_adc_callback, "get ADC voltage value on pin 26"},
        {"get_temp", get_temp_callback, "get chip temperature in Celsius"},
        {NULL, NULL, NULL},
    };

    protocol_task_init(device_api);
    stdio_task_init();
    adc_task_init();

    while(1) {
        char* com = stdio_task_handle();
        if(com != NULL) {
            protocol_task_handle(com);
        }
    }
}