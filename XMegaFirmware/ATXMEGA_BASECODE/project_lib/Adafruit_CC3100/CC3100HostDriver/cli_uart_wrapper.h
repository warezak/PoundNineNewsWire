/*
 * cli_uart_ported.h
 *
 * Created: 5/22/2015 10:56:42 AM
 *  Author: CLOUD 7
 */ 
#ifndef _CLI_UART_WRAPPER_
#define _CLI_UART_WRAPPER_

#ifdef    __cplusplus
extern "C"
{
#endif

#include <string.h>
#include <usart_serial.h>
#include <ioport.h>
#include <conf_board.h>


#define CLI_Read(a) iUartReadData(a)
#define CLI_Configure vUartInit
#define CLI_Write(a) iUartWriteData((unsigned char*)a)

int iUartReadData(unsigned char *pBuff);
void vUartInit(void);
int iUartWriteData(unsigned char *inBuff);

#ifdef    __cplusplus
}
#endif
#endif // _CLI_UART_WRAPPER