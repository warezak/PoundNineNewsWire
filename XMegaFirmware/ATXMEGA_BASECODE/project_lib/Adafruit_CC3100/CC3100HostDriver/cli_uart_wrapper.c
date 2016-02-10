/*
 * cli_uart_ported.c
 *
 * Created: 5/22/2015 10:56:21 AM
 *  Author: CLOUD 7
 */ 

#include "cli_uart_wrapper.h"

#define ASCII_ENTER     0x0D

int iUartReadData(unsigned char *pBuff)
{
	int tuReadDataLen = 0;
		
	if(pBuff == NULL)
		return -1;
		
	while(*pBuff != ASCII_ENTER)
	{
		usart_serial_getchar(USART_SERIAL_INTERFACE, pBuff);
		pBuff++;
		tuReadDataLen++;
	}
	
	return tuReadDataLen;
}

void vUartInit(void)
{
	static usart_rs232_options_t USART_SERIAL_OPTIONS =
		{
		.baudrate = USART_SERIAL_SET_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
		};
		
	ioport_configure_pin(CC3100_UART_TXD_PIN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH); // txd
	
	ioport_configure_pin(CC3100_UART_RXD_PIN, IOPORT_DIR_INPUT); //Rxd
		
	usart_init_rs232(USART_SERIAL_INTERFACE,&USART_SERIAL_OPTIONS);
}

int iUartWriteData(unsigned char *inBuff)
{
	int i;
	if(inBuff == NULL)
		return -1;
		
	unsigned short ret,usLength = strlen((const char *)inBuff);
	ret = usLength;
	
	for (i = 0; i < usLength; i++)
	{
		usart_putchar(USART_SERIAL_INTERFACE, *inBuff);
		inBuff++;
	}
	
	return (int)ret;
	
}
