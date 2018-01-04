#ifndef __MAIN_H_
#define __MAIN_H_

#include "define.h"
#include <iostm8.h>
#include "lt_stm8s_macro.h"
#include "LT_STM8_Vectors.h"
#include "nRF24L01.h"
#include "UART.h"
#include <string.h>
#include "stdbool.h"

void CLK_Init(void);
void GPIO_Init(void);
void sendDataRF(u8 *array);
void clearArray(u8 *array);
void RF2UART(void);
void UART2RF(void);
void sendDataFeedback(void);
void TXX(void);
void RXX(void);
void nRF_fake_init_tx(void);
void nRF_fake_init_rx(void);

#endif