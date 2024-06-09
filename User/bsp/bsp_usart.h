#ifndef __USART_H
#define	__USART_H

#include "stm32f10x.h"
#include <stdio.h>


#define  DEBUG_USARTx                   USART1

/* Basic-Driver */
void usart1_init(unsigned int baud);
void usart2_init(unsigned int baud);
void usart3_init(unsigned int baud);
void usartx_send_byte(USART_TypeDef *USARTx, uint8_t byte);
void usartx_send_string(USART_TypeDef *USARTx, char *str);
void usartx_send_buffer(USART_TypeDef *USARTx, uint8_t *buffer, uint8_t len);
void usart_send_halfword( USART_TypeDef * USARTx, uint16_t ch);
void usartx_printf(USART_TypeDef *USARTx, char *fmt, ...);
// int fputc(int ch, FILE *f);
// int fgetc(FILE *f);

/* DJI-CRC CHECK API*/
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);

#endif /* __USART_H */
