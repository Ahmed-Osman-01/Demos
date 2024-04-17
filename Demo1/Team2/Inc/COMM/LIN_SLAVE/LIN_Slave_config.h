/*
 * LIN_Slave_config.h
 *
 *  Created on: Apr 7, 2024
 *      Author: Osman
 */

#ifndef COMM_LIN_SLAVE_LIN_SLAVE_CONFIG_H_
#define COMM_LIN_SLAVE_LIN_SLAVE_CONFIG_H_


#define SLAVE1_MESSAGES_COUNT   2


#define LIN_SLAVE_SCI     UART1


#define LIN_SLAVE_TX_PIN   GPIO_PIN_9
#define LIN_SLAVE_TX_PORT  GPIO_PORT_A

#define LIN_SLAVE_RX_PIN   GPIO_PIN_10
#define LIN_SLAVE_RX_PORT  GPIO_PORT_A

#define LIN_SLAVE_SPEED    9600U


#endif /* COMM_LIN_SLAVE_LIN_SLAVE_CONFIG_H_ */
