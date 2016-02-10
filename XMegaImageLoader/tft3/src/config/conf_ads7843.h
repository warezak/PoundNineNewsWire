/*
 * conf_ads7843.h
 *
 * Created: 5/31/2015 11:41:45 PM
 *  Author: ssenthil
 */ 

#include <board.h>
#ifndef CONF_ADS7843_H_
#define CONF_ADS7843_H_

#define BOARD_ADS7843_SPI_BASE     TOUCH_SPI
#define CONF_ADS7843_USART_SPI		TOUCH_SPI
#define CONF_ADS7843_CLOCK_SPEED   100000UL
//1000000UL


#define BOARD_ADS7843_SPI_NPCS        TOUCH_CS
#define CONF_ADS7843_CS_PIN        TOUCH_CS

#define BOARD_ADS7843_IRQ_GPIO        TOUCH_INT



#define SPI_TIMEOUT 5000000
#define SPI_ERROR_TIMEOUT 5


#endif /* CONF_ADS7843_H_ */

//
//#ifndef CONF_ADS7843_H_INCLUDED
//#define CONF_ADS7843_H_INCLUDED
//
//#include "board.h"
//
/*
#if !defined(BOARD_ADS7843_SPI_NPCS) || \
!defined(BOARD_ADS7843_IRQ_GPIO) || \
!defined(BOARD_ADS7843_IRQ_FLAGS) || \
!defined(BOARD_ADS7843_SPI_BASE)
*/

//#warning The ADS7843 SPI configuration does not exist in the board definition file. Using default settings.
//
//#define BOARD_ADS7843_SPI_BASE    &SPID /* SPI instance, which can be SPI, SPI0 or SPI1, depends on which SPI channel is used by ADS7843. */
////	#define BOARD_ADS7843_SPI_BASE    0 /* SPI instance, which can be SPI, SPI0 or SPI1, depends on which SPI channel is used by ADS7843. */
//#define BOARD_ADS7843_SPI_NPCS    0 /* SPI chip select number, depends on which SPI CS pin is used by ADS7843. */
////	#define BOARD_ADS7843_IRQ_GPIO    PIO_PA16_IDX /* PIN index, depends on which pin is connnected with nPENIRQ of ADS7843 */
////	#define BOARD_ADS7843_IRQ_FLAGS   (PIO_INPUT | PIO_PULLUP) /* PIO type and attribute of the pin connected with nPENIRQ, normally should be (PIO_INPUT | PIO_PULLUP) */
//
//#define BOARD_ADS7843_IRQ_GPIO    IOPORT_CREATE_PIN(PORTC, 0) /* PIN index, depends on which pin is connected with nPENIRQ of ADS7843 */
//#define BOARD_ADS7843_IRQ_FLAGS   (IOPORT_DIR_INPUT|IOPORT_PULL_UP) /* PIO type and attribute of the pin connected with nPENIRQ, normally should be (PIO_INPUT | PIO_PULLUP) */
//
//// interrupt for touch screen
/*
#define ADS7843_INTC_INIT()          ioport_configure_pin(PIN0_bp, IOPORT_DIR_INPUT); \
PORTC.PIN0CTRL = PORT_ISC0_bm; \
PORTC.INT0MASK = PIN0_bm; \
PORTC.INTFLAGS = PORT_INT0IF_bm;
*/
////IOPORT_SENSE_FALLING
//#define RTOUCH_ISR()                ISR(PORTC_INT0_vect)
//
//#define ENABLE_TRX_IRQ()                (PORTC.INTCTRL |= PORT_INT0LVL_gm)
///** Disables the transceiver main interrupt. */
//#define DISABLE_TRX_IRQ()               (PORTC.INTCTRL &= ~PORT_INT0LVL_gm)
///** Clears the transceiver main interrupt. */
//#define CLEAR_TRX_IRQ()                 (PORTC.INTFLAGS = PORT_INT0IF_bm)
//
//
//#endif
//#define SPI_TIMEOUT 5000000
//#define SPI_ERROR_TIMEOUT 5
//#endif /* CONF_ADS7843_H_INCLUDED */
//