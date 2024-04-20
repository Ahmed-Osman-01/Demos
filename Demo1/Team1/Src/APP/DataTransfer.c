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

volatile uint16_t Switches_Status;
volatile uint16_t Switches_Status_send;


void DataTransferTask()

{
    MasterGetControls(&Switches_Status);
    MasterSendControls(Switches_Status_send);
}
