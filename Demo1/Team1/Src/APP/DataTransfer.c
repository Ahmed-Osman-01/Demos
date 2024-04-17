/******************************************************************************
*
* Module: Data Transfer
*
* File Name: DataTransfer.c
*
* Description: Source file for the Data Transfer Module That sends and recieves the data to other MC
*
* Author: Momen Elsayed Shaban
*
*******************************************************************************/

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LIB/std_types.h"
#include "HAL/LIN/LIN_MASTER/LIN_MasterAppData.h"

uint8_t Switches_CTRL_Control;
uint8_t Switches_CTRL_Directions;
uint8_t Switches_Status_Control;
uint8_t Switches_Status_Directions;


void DataTransferTask()

{
    MasterGetControls(&Switches_Status_Control,&Switches_Status_Directions);
    MasterSendControls(Switches_CTRL_Control,Switches_CTRL_Directions);
}
