/*
 * LIN_MASTER_config.h
 *
 *  Created on: Apr 5, 2024
 *      Author: Osman
 */

#ifndef COMM_LIN_MASTER_LIN_MASTER_CONFIG_H_
#define COMM_LIN_MASTER_LIN_MASTER_CONFIG_H_


#define SCHED_ENTRY_NUM     2


#define MASTER_TASK_PERIODICITY     2


#define LIN_MASTER_MESSAGES_NUM		2



/* Uart confifuration */

#define LIN_MASTER_SCI     UART2

#define LIN_MASTER_TX_PIN   GPIO_PIN_2
#define LIN_MASTER_TX_PORT  GPIO_PORT_A

#define LIN_MASTER_RX_PIN   GPIO_PIN_3
#define LIN_MASTER_RX_PORT  GPIO_PORT_A

#define LIN_MASTER_SPEED    9600U


#endif /* COMM_LIN_MASTER_LIN_MASTER_CONFIG_H_ */
