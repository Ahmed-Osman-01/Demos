
#include "MCAL/GPIO/GPIO_DRIVER.h"
#include "HAL/LCD/LCD_DRIVER.h"

#if 0
LCD_cfg_t LCDs_PinCfg =
    {
        .LCD_data_pins[0].Pin = GPIO_PIN_0,
        .LCD_data_pins[0].Port = GPIO_PORT_B,
        .LCD_data_pins[1].Pin = GPIO_PIN_1,
        .LCD_data_pins[1].Port = GPIO_PORT_B,
        .LCD_data_pins[2].Pin = GPIO_PIN_2,
        .LCD_data_pins[2].Port = GPIO_PORT_B,
        .LCD_data_pins[3].Pin = GPIO_PIN_5,
        .LCD_data_pins[3].Port = GPIO_PORT_B,
        .LCD_data_pins[4].Pin = GPIO_PIN_8,
        .LCD_data_pins[4].Port = GPIO_PORT_B,
        .LCD_data_pins[5].Pin = GPIO_PIN_9,
        .LCD_data_pins[5].Port = GPIO_PORT_B,
        .LCD_data_pins[6].Pin = GPIO_PIN_10,
        .LCD_data_pins[6].Port = GPIO_PORT_B,
        .LCD_data_pins[7].Pin = GPIO_PIN_12,
        .LCD_data_pins[7].Port = GPIO_PORT_B,
        .R_S_pin.Pin = GPIO_PIN_0,
        .R_S_pin.Port = GPIO_PORT_A,
        .R_W_pin.Pin = GPIO_PIN_1,
        .R_W_pin.Port = GPIO_PORT_A,
        .E_pin.Pin = GPIO_PIN_2,
        .E_pin.Port = GPIO_PORT_A
    };
#elif 1
LCD_cfg_t LCDs_PinCfg =
{
    .LCD_data_pins[0].Pin = GPIO_PIN_1,
    .LCD_data_pins[0].Port = GPIO_PORT_B,
    .LCD_data_pins[1].Pin = GPIO_PIN_4,
    .LCD_data_pins[1].Port = GPIO_PORT_A,
    .LCD_data_pins[2].Pin = GPIO_PIN_5,
    .LCD_data_pins[2].Port = GPIO_PORT_A,
    .LCD_data_pins[3].Pin = GPIO_PIN_6,
    .LCD_data_pins[3].Port = GPIO_PORT_A,
    .LCD_data_pins[4].Pin = GPIO_PIN_7,
    .LCD_data_pins[4].Port = GPIO_PORT_A,
    .LCD_data_pins[5].Pin = GPIO_PIN_8,
    .LCD_data_pins[5].Port = GPIO_PORT_A,
    .LCD_data_pins[6].Pin = GPIO_PIN_9,
    .LCD_data_pins[6].Port = GPIO_PORT_A,
    .LCD_data_pins[7].Pin = GPIO_PIN_10,
    .LCD_data_pins[7].Port = GPIO_PORT_A,
    .R_S_pin.Pin = GPIO_PIN_0,
    .R_S_pin.Port = GPIO_PORT_A,
    .R_W_pin.Pin = GPIO_PIN_1,
    .R_W_pin.Port = GPIO_PORT_A,
    .E_pin.Pin = GPIO_PIN_0,
    .E_pin.Port = GPIO_PORT_B
};

#endif