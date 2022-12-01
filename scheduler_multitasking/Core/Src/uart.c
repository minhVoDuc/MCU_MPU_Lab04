/*
 * uart.c
 *
 *  Created on: Nov 30, 2022
 *      Author: Guest_demo
 */

#include "uart.h"

UART_HandleTypeDef huart2;

void uart_init() {
	sprintf(tx_data, "HELLO!\r\n");
	HAL_UART_Transmit(&huart2, &tx_data, sizeof(tx_data), 1000);
	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

void uart_send_str(char str[]) {
	sprintf(tx_data, "");
	rx_data = "";
	sprintf(tx_data, "%s\r\n", str);
	HAL_UART_Transmit(&huart2, tx_data, sizeof(tx_data), 1000);
	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}

void uart_send_num(char str[], uint32_t data) {
	sprintf(tx_data, "");
	rx_data = "";
	sprintf(tx_data, "%s%d\r\n", str, data);
	HAL_UART_Transmit(&huart2, tx_data, sizeof(tx_data), 1000);
	HAL_UART_Receive_IT(&huart2, &rx_data, 1);
}
