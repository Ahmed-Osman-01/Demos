/******************************************************************************
 *
 * Module: Switch
 *
 * File Name: SWITCH_Cfg.c
 *
 * Description: Source file for the Switch driver for STM32F401xC Configurations
 *
 * Author: Momen Elsayed Shaban
 *
 *******************************************************************************/
#include "GPIO.h"
#include "SWITCH.h"

const SWITCH_Cfg_t Switches[_SWITCH_NUM]=
{
    [SWITCH_Fire]={
        .port = GPIO_PORT_A,
        .pin = GPIO_PIN_7,
        .dir = SWITCH_PULL_UP,
    }
};
