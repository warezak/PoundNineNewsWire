
/**
 * \file
 *
 * \brief Board configuration
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#ifndef CONF_BOARD_H_INCLUDED
#define CONF_BOARD_H_INCLUDED

// Wifi
#define CC3100_nHIB_PIN			IOPORT_CREATE_PIN(PORTB, 2)
#define CC3100_RESET_PIN		IOPORT_CREATE_PIN(PORTB, 3)
#define CC3100_IRQ_PIN			IOPORT_CREATE_PIN(PORTD, 0)

#define CC3100_UART_TXD_PIN		IOPORT_CREATE_PIN(PORTF, 3)
#define CC3100_UART_RXD_PIN		IOPORT_CREATE_PIN(PORTF, 2)

#define CC3100_SPI				&SPIC
#define CC3100_SPI_SS_PIN		IOPORT_CREATE_PIN(PORTC, 4)
#define CC3100_SPI_MOSI_PIN		IOPORT_CREATE_PIN(PORTC, 5)
#define CC3100_SPI_MISO_PIN		IOPORT_CREATE_PIN(PORTC, 6)
#define CC3100_SPI_SCK_PIN		IOPORT_CREATE_PIN(PORTC, 7)

// Display
#define TFT_DISPLAY_USART_SPI      &USARTC0 //usartc1
#define TFT_DISPLAY_CS             IOPORT_CREATE_PIN(PORTB, 0) //PC 4
#define TFT_DISPLAY_SCK            IOPORT_CREATE_PIN(PORTC, 1) //7
#define TFT_DISPLAY_MOSI           IOPORT_CREATE_PIN(PORTC, 3) //5
#define TFT_DISPLAY_MISO           IOPORT_CREATE_PIN(PORTC, 2) //6
#define TFT_DISPLAY_CHG            IOPORT_CREATE_PIN(PORTC, 2)
#define TFT_DISPLAY_DC             IOPORT_CREATE_PIN(PORTB, 1) //PC 3
#define TFT_DISPLAY_BACKLIGHT      IOPORT_CREATE_PIN(PORTA, 4)
#define TFT_DISPLAY_LCD_RESET      IOPORT_CREATE_PIN(PORTA, 6)

// Touch
#define TOUCH_SPI                     &USARTE0
#define TOUCH_CS                      IOPORT_CREATE_PIN(PORTE, 0)
#define TOUCH_MASTER_SCK              IOPORT_CREATE_PIN(PORTE, 1)
#define TOUCH_MASTER_MOSI             IOPORT_CREATE_PIN(PORTE, 3)
#define TOUCH_MASTER_MISO             IOPORT_CREATE_PIN(PORTE, 2)
#define TOUCH_INT                     IOPORT_CREATE_PIN(PORTC, 0)
#define TOUCH_INTC_INIT()           ioport_configure_pin(TOUCH_INT, IOPORT_DIR_INPUT); \
									PORTC.PIN0CTRL = PORT_ISC0_bm; \
									PORTC.INT0MASK = PIN0_bm; \
									PORTC.INTFLAGS = PORT_INT0IF_bm;
									
//! \name DataFlash memory (AT45DBX)

#define AT45DBX_SPI                     &USARTD0
#define AT45DBX_CS                      IOPORT_CREATE_PIN(PORTF, 4)
#define AT45DBX_MASTER_SCK              IOPORT_CREATE_PIN(PORTD, 1)
#define AT45DBX_MASTER_MOSI             IOPORT_CREATE_PIN(PORTD, 3)
#define AT45DBX_MASTER_MISO             IOPORT_CREATE_PIN(PORTD, 2)

// Necessary After adding tft
/** Enables the transceiver main interrupt. */
#define ENABLE_TRX_IRQ()                (PORTC.INTCTRL |= PORT_INT0LVL_gm)

/** Disables the transceiver main interrupt. */
#define DISABLE_TRX_IRQ()               (PORTC.INTCTRL &= ~PORT_INT0LVL_gm)

/** Clears the transceiver main interrupt. */
#define CLEAR_TRX_IRQ()                 (PORTC.INTFLAGS = PORT_INT0IF_bm)

/* Enable the TFT Display */
#define CONF_BOARD_ENABLE_TFT_DISPLAY

// Enable AT45DBX Component.
#define CONF_BOARD_AT45DBX

// SRAM Pin Conf
#define SRAM23Kxx_SPI           CC3100_SPI
#define SRAM23Kxx_CS			IOPORT_CREATE_PIN(PORTA, 5)
#define SRAM23Kxx_SCK			CC3100_SPI_SCK_PIN
#define SRAM23Kxx_MOSI			CC3100_SPI_MOSI_PIN
#define SRAM23Kxx_MISO			CC3100_SPI_MISO_PIN
#define CONF_SRAM23Kxx_CLOCK_SPEED 8000000

//#define BOARD_XOSC_HZ          8000000
//#define BOARD_XOSC_TYPE        XOSC_TYPE_XTAL
//#define BOARD_XOSC_STARTUP_US  500000


#endif /* CONF_BOARD_H_INCLUDED */
