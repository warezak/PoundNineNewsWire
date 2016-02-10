/*
 * board_wrapper.h
 *
 * Created: 5/22/2015 11:35:26 AM
 *  Author: CLOUD 7
 */ 
#ifndef _BOARD_WRAPPER_
#define _BOARD_WRAPPER_
#ifdef __cplusplus
extern "C"
{
#endif

#include <ioport.h>
#include <delay.h>
#include <conf_board.h>
//#include "cli_uart_wrapper.h"
#include <wdt.h>
typedef void (*P_EVENT_HANDLER)(void* pValue);


#define initClk vInitClkBoard
#define stopWDT vWDTDisable
#define registerInterruptHandler(a,b) iRegisterInterruptHandler(a,b)
#define CC3100_InterruptEnable vCC3100InterruptEnable
#define CC3100_InterruptDisable vCC3100InterruptDisable
#define Delay(a) vDelay(a)


void vInitClkBoard(void);
void vWDTDisable(void);
void vDisableCC3100(void);
void vEnableCC3100(void);
int iRegisterInterruptHandler(P_EVENT_HANDLER InterruptHdl , void* pValue);
void vCC3100InterruptEnable(void);
void vCC3100InterruptDisable(void);
void MaskIntHdlr(void);
void UnMaskIntHdlr(void);
void vDelay(unsigned long interval);


#ifdef __cplusplus
}
#endif
#endif