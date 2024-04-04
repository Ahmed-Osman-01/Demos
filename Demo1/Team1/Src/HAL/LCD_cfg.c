
#include "MCAL/GPIO_DRIVER.h"
#include "HAL/LCD_DRIVER.h"

LCD_cfg_t LCDs_PinCfg =
    {
        //   .LCD_data_pins[0]
        //       .Pin = GPIO_PIN_0,
        //   .LCD_data_pins[0].Port = GPIO_PORT_A,
        //   .LCD_data_pins[1].Pin = GPIO_PIN_1,
        //   .LCD_data_pins[1].Port = GPIO_PORT_A,
        //   .LCD_data_pins[2].Pin = GPIO_PIN_2,
        //   .LCD_data_pins[2].Port = GPIO_PORT_A,
        //   .LCD_data_pins[3].Pin = GPIO_PIN_3,
        //   .LCD_data_pins[3].Port = GPIO_PORT_A,
        .LCD_data_pins[0].Pin = GPIO_PIN_4,
        .LCD_data_pins[0].Port = GPIO_PORT_A,
        .LCD_data_pins[1].Pin = GPIO_PIN_5,
        .LCD_data_pins[1].Port = GPIO_PORT_A,
        .LCD_data_pins[2].Pin = GPIO_PIN_6,
        .LCD_data_pins[2].Port = GPIO_PORT_A,
        .LCD_data_pins[3].Pin = GPIO_PIN_7,
        .LCD_data_pins[3].Port = GPIO_PORT_A,
        .R_S_pin.Pin = GPIO_PIN_10,
        .R_S_pin.Port = GPIO_PORT_A,
        .R_W_pin.Pin = GPIO_PIN_9,
        .R_W_pin.Port = GPIO_PORT_A,
        .E_pin.Pin = GPIO_PIN_8,
        .E_pin.Port = GPIO_PORT_A};