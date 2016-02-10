/**
 * \file
 *
 * \brief API driver for component ADS7843.
 *
 * Copyright (c) 2011-2015 Atmel Corporation. All rights reserved.
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
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "ads7843.h"
#include "conf_ads7843.h"
#include "board.h"
#include "ioport.h"
//#include "pio_handler.h"
//#include "pio.h"
//#include "spi_master.h"
//#include "conf_spi_master.h"
#include "asf.h"
#include "util/delay.h"
/** PD0 */
#define ADS_CTRL_PD0              (1 << 0)
/** PD1 */
#define ADS_CTRL_PD1              (1 << 1)
/** SER/DFR */
#define ADS_CTRL_DFR              (1 << 2)
/** Mode */
#define ADS_CTRL_EIGHT_BITS_MOD   (1 << 3)
/** Start Bit */
#define ADS_CTRL_START            (1 << 7)
/** Address setting */
#define ADS_CTRL_SWITCH_SHIFT     4

/** Get X position command */
#define CMD_Y_POSITION ((1 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START |\
		ADS_CTRL_PD0 | ADS_CTRL_PD1)
//1000 1011 -> 8B

/** Get Y position command */
#define CMD_X_POSITION ((5 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START |\
		ADS_CTRL_PD0 | ADS_CTRL_PD1)
//1000 0011 -> 83


/** Enable penIRQ */
#define CMD_ENABLE_PENIRQ  ((1 << ADS_CTRL_SWITCH_SHIFT) | ADS_CTRL_START)
//0000 1001 -> 09


#define ADS7843_TIMEOUT        5000000
#define ADS7843_BUFSIZE        1

/** Frequence rate for sending one bit */
#define ADS7843_SPI_BAUDRATE   100000

/** 2us min (tCSS) <=> 200/100 000 000 = 2us */
#define DELAY_BEFORE_SPCK          200
/** 5us min (tCSH) <=> (32 * 15) / (100 000 000) = 5us */
#define DELAY_BETWEEN_CONS_COM     0xf

/** @cond 0*/
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/** @endcond*/



__always_inline static void ads7843_send_byte(uint8_t data)
{
	#if defined(CONF_ADS7843_USART_SPI)
		#  if XMEGA
		while (!usart_data_register_is_empty(CONF_ADS7843_USART_SPI)) {
		/* Do nothing */
		}

		irqflags_t flags = cpu_irq_save();
		usart_clear_tx_complete(CONF_ADS7843_USART_SPI);
		usart_put(CONF_ADS7843_USART_SPI, data);
		cpu_irq_restore(flags);
		#  else
		usart_spi_write_single(CONF_ADS7843_USART_SPI, data);
		#  endif
	#elif defined(CONF_ADS7843_SPI)
	/* Wait for any previously running send data */
	//ili9341_wait_for_send_done();

	//spi_write_single(CONF_ADS7843_SPI, data);
	#endif
}

/*
__always_inline static uint8_t ads7843_read_byte(void)
{
	uint8_t data;

	#if defined(CONF_ADS7843_USART_SPI)
	#  if XMEGA
	/ * Workaround for clearing the RXCIF for XMEGA * /
	usart_rx_enable(CONF_ADS7843_USART_SPI);

	usart_put(CONF_ADS7843_USART_SPI, 0xFF);
	while (!usart_rx_is_complete(CONF_ADS7843_USART_SPI)) {
		/ * Do nothing * /
	}
	data = usart_get(CONF_ADS7843_USART_SPI);

	/ * Workaround for clearing the RXCIF for XMEGA * /
	usart_rx_disable(CONF_ADS7843_USART_SPI);
	#  else
	usart_spi_read_single(CONF_ADS7843_USART_SPI, &data);
	#  endif
	#elif defined(CONF_ADS7843_SPI)
	spi_write_single(CONF_ADS7843_SPI, 0xFF);

	ads7843_wait_for_send_done();

	/ * Wait for RX to complete * /
	while (!spi_is_rx_full(CONF_ADS7843_SPI)) {
		/ * Do nothing * /
	}

	spi_read_single(CONF_ADS7843_SPI, &data);
	#endif

	return data;
}*/
__always_inline static void ads7843_select_chip(void)
{
	ioport_set_pin_level(CONF_ADS7843_CS_PIN, false);
}

__always_inline static void ads7843_deselect_chip(void)
{
	ioport_set_pin_level(CONF_ADS7843_CS_PIN, true);
}

__always_inline static void ads7843_wait_for_send_done(void)
{
	#if defined(CONF_ADS7843_USART_SPI)
	#  if XMEGA
	while (!usart_tx_is_complete(CONF_ADS7843_USART_SPI)) {
		/* Do nothing */
	}
	usart_clear_tx_complete(CONF_ADS7843_USART_SPI);
	#  else
	/* Wait for TX to complete */
	while (!usart_spi_is_tx_empty(CONF_ADS7843_USART_SPI)) {
		/* Do nothing */
	}
	#  endif
	#elif defined(CONF_ADS7843_SPI)
	/* Wait for TX to complete */
	while (!spi_is_tx_empty(CONF_ADS7843_SPI)) {
		/* Do nothing */
	}
	#endif
}
__always_inline static uint8_t ads7843_read_byte(void)
{
	uint8_t data;

	#if defined(CONF_ADS7843_USART_SPI)
	#  if XMEGA
	/* Workaround for clearing the RXCIF for XMEGA */
	usart_rx_enable(CONF_ADS7843_USART_SPI);

	//senthil temp usart_put(CONF_ADS7843_USART_SPI, 0xFF);
	while (!usart_rx_is_complete(CONF_ADS7843_USART_SPI)) {
		/* Do nothing */
	}
	data = usart_get(CONF_ADS7843_USART_SPI);

	/* Workaround for clearing the RXCIF for XMEGA */
	usart_rx_disable(CONF_ADS7843_USART_SPI);
	#  else
	usart_spi_read_single(CONF_ADS7843_USART_SPI, &data);
	#  endif
	#elif defined(CONF_ADS7843_SPI)
	spi_write_single(CONF_ADS7843_SPI, 0xFF);

	ads7843_wait_for_send_done();

	/* Wait for RX to complete */
	while (!spi_is_rx_full(CONF_ADS7843_SPI)) {
		/* Do nothing */
	}

	spi_read_single(CONF_ADS7843_SPI, &data);
	#endif

	return data;
}
/**
 * \brief Send a command to the ADS7843 touch controller.
 *
 * \param uc_cmd command to send.
 *
 * \return Command result.
 */
static uint32_t ads7843_send_cmd(uint8_t uc_cmd)
{
	uint32_t uResult = 0;
//	volatile uint32_t i;
	uint8_t data;
//	uint32_t timeout = SPI_TIMEOUT;

 char buffer [33];
 
	/** (volatile declaration needed for code optimisation by compiler) */
	volatile uint8_t bufferRX[ADS7843_BUFSIZE];
//	volatile uint8_t bufferTX[ADS7843_BUFSIZE];

	bufferRX[0] = 0;
	bufferRX[1] = 0;
	bufferRX[2] = 0;

/*
	bufferTX[0] = uc_cmd;
	bufferTX[1] = 0;
	bufferTX[2] = 0;
*/

	ads7843_select_chip();
	ads7843_send_byte(uc_cmd);
	ads7843_wait_for_send_done();
	data = ads7843_read_byte();
		ads7843_deselect_chip();
	bufferRX[0] = data;
	//data = ads7843_read_byte();
if (data > 0)
{
	itoa(data, buffer,10);
	
	gfx_draw_string("data :",20, 80, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
	gfx_draw_string(buffer,50, 80, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
	
	if (data <= 15)
	{
		gfx_draw_string("data <=15",20, 90, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);		
	}
	if (data == 16)
	{
		gfx_draw_string("data 16",20, 90, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
	}
	if (data ==17)
	{
		gfx_draw_string("data 17",20, 90, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
	}
	if (data == 18)
	{
		gfx_draw_string("data 18",20, 90, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
	}
	if (data > 18)
	{
		gfx_draw_string("data > 18",20, 90, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
	}

}
else
{
		gfx_draw_string("data====0",20, 80, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	//	_delay_ms(5000);
}
_delay_ms(2000);


/*

	for(i = 0; i < ADS7843_BUFSIZE; i++){
		timeout = SPI_TIMEOUT;
		while (!spi_is_tx_ready(BOARD_ADS7843_SPI_BASE)) {
			if (!timeout--) {
					gfx_draw_string("SPI tx is not ready",60, 80, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
				return SPI_ERROR_TIMEOUT;
			}
		}
		spi_write_single(BOARD_ADS7843_SPI_BASE, bufferTX[i]);
				gfx_draw_string("write complete",60, 90, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	}
	for(i = 0; i < ADS7843_BUFSIZE; i++){
		timeout = SPI_TIMEOUT;
		while (!spi_is_rx_full(BOARD_ADS7843_SPI_BASE)) {
			if (!timeout--) {
				return SPI_ERROR_TIMEOUT;
				gfx_draw_string("SPI Rx full",60, 100, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
			}
		}
		spi_read_single(BOARD_ADS7843_SPI_BASE, &data);
		gfx_draw_string("SPI Rx done:",60, 100, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
		gfx_draw_string(&data,150, 100, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
		bufferRX[i] = data;
	}*/

	uResult = (uint32_t)bufferRX[1] << 8;
	uResult |= (uint32_t)bufferRX[2];
	uResult = uResult >> 4;

	return uResult;
}

uint32_t ads7843_is_pressed(void)
{
	return (ioport_get_pin_level(BOARD_ADS7843_IRQ_GPIO) ==	IOPORT_PIN_LEVEL_LOW); 
}

typedef void (*int_callback_t)();


void ads7843_set_handler(void (*p_handler) (uint32_t, uint32_t))
{
	/** Initialize interrupts */
	//tbdpio_handler_set_pin(BOARD_ADS7843_IRQ_GPIO,
//tbd			BOARD_ADS7843_IRQ_FLAGS,
//tbd			(void (*)(uint32_t, uint32_t)) p_handler
			
		//tbd	);
//	int_callback_t = p_handler;
	/** Enable the interrupt */
	ENABLE_TRX_IRQ();
	//tbdpio_enable_pin_interrupt(BOARD_ADS7843_IRQ_GPIO);
}

void ads7843_enable_interrupt(void)
{
	ENABLE_TRX_IRQ();
	/** Enable the interrupt */
	//tbdpio_enable_pin_interrupt(BOARD_ADS7843_IRQ_GPIO);
}

void ads7843_disable_interrupt(void)
{
	DISABLE_TRX_IRQ();
	/** Disable the interrupt */
	//tbdpio_disable_pin_interrupt(BOARD_ADS7843_IRQ_GPIO);
}

void ads7843_get_raw_point(uint32_t *p_x, uint32_t *p_y)
{
	/** Disable interrupt to quickly evaluate the coordinates */
	ads7843_disable_interrupt();
	//tbdpio_disable_pin_interrupt(BOARD_ADS7843_IRQ_GPIO);

	/** Get X position */
	//gfx_draw_string(p_x,10, 130, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	//*p_x = ads7843_send_cmd(CMD_X_POSITION);
	*p_x = ads7843_send_cmd(0xD0);
	gfx_draw_string(p_x,150, 130, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	/** Get Y position */
	//gfx_draw_string(p_y,10, 140, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	//*p_y = ads7843_send_cmd(CMD_Y_POSITION);
	*p_y = ads7843_send_cmd(0x90);
	gfx_draw_string(p_y,150, 140, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	/** Switch to full power mode */
		ads7843_select_chip();
	ads7843_send_byte(CMD_ENABLE_PENIRQ);
	ads7843_deselect_chip();
	/** Re-enable interrupt */
	
	//tbdpio_enable_pin_interrupt(BOARD_ADS7843_IRQ_GPIO);
	ads7843_enable_interrupt();
}

uint32_t ads7843_init(void)
{
		spi_flags_t spi_flags = SPI_MODE_0;
		board_spi_select_id_t spi_select_id = 0;
		
		struct usart_spi_device device = {
			.id = 0,
		};
/*
		gfx_draw_string("1. Usart SPi init",10, 10, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
*/
		usart_spi_init(CONF_ADS7843_USART_SPI);
		
//			_delay_ms(2000);
		usart_spi_setup_device(CONF_ADS7843_USART_SPI, &device, spi_flags,
		CONF_ADS7843_CLOCK_SPEED, spi_select_id);	
//		_delay_ms(2000);
		ads7843_select_chip();
//		gfx_draw_string("2. Usart SPi setup done",10, 30, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	//ads7843_send_byte(CMD_ENABLE_PENIRQ);
		return 0;
}

/*
uint32_t ads7843_init(void)
{
	volatile uint32_t uDummy;
	struct spi_device ADS7843_SPI_DEVICE_CFG = {
		/ ** Board specific chip select configuration* /
#ifdef BOARD_ADS7843_SPI_NPCS
		.id = BOARD_ADS7843_SPI_NPCS
#else
#warning The board TouchScreen chip select definition is missing. Default configuration is used.
		.id = 0
#endif
	};

	spi_master_init(BOARD_ADS7843_SPI_BASE);
	spi_master_setup_device(BOARD_ADS7843_SPI_BASE, &ADS7843_SPI_DEVICE_CFG,
			SPI_MODE_0, ADS7843_SPI_BAUDRATE, 0);
	spi_select_device(BOARD_ADS7843_SPI_BASE, &ADS7843_SPI_DEVICE_CFG);
	gfx_draw_string("SPI D CS deasserted",60, 35, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);
	//_delay_ms(2000);
	spi_enable(BOARD_ADS7843_SPI_BASE);
	gfx_draw_string("SPI enabled",60, 50, &sysfont,GFX_COLOR_YELLOW, GFX_COLOR_RED);

	for (uDummy = 0; uDummy < 100000; uDummy++) {
	}

	ads7843_send_cmd(CMD_ENABLE_PENIRQ);

	return 0;
}*/

/** @cond 0*/
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/** @endcond*/

/**
 * \}
 */





void ADS7843_write(uint8_t dat)
{
//	uint8_t nop;
	ioport_set_pin_level(TOUCH_MASTER_SCK, false);

	for(int i=0; i<8; i++) {
		if(dat&0x80)
			ioport_set_pin_level(TOUCH_MASTER_MOSI, true);
		else
			ioport_set_pin_level(TOUCH_MASTER_MOSI, false);
		dat=dat<<1;
		ioport_set_pin_level(TOUCH_MASTER_SCK, false);
		ioport_set_pin_level(TOUCH_MASTER_SCK, true);
	}
	
		ioport_set_pin_level(TOUCH_MASTER_SCK, false);
}

uint16_t ADS7843_read(void)
{
//	uint8_t nop;
	uint16_t Num=0;
	for(int i=0; i<12; i++) {
		Num<<=1;
		ioport_set_pin_level(TOUCH_MASTER_SCK, true);
		ioport_set_pin_level(TOUCH_MASTER_SCK, false);
		if(ioport_get_pin_level(TOUCH_MASTER_MISO))
		Num++;
	}
		ioport_set_pin_level(TOUCH_MASTER_SCK, true);
		ioport_set_pin_level(TOUCH_MASTER_SCK, false);
		ioport_set_pin_level(TOUCH_MASTER_SCK, true);
		ioport_set_pin_level(TOUCH_MASTER_SCK, false);
		ioport_set_pin_level(TOUCH_MASTER_SCK, true);
		ioport_set_pin_level(TOUCH_MASTER_SCK, false);
		ioport_set_pin_level(TOUCH_MASTER_SCK, true);
		ioport_set_pin_level(TOUCH_MASTER_SCK, false);
	return(Num);
}
#define ADS7843_READ_TIMES		20
#define ADS7843_LOST_VAL		3



Point_t ADS7843_getpos(uint8_t *flag)
{
	Point_t p[ADS7843_READ_TIMES];
	Point_t P;
	int i;
	int k,j,x_tmp, y_tmp;
	uint32_t sum_x, sum_y;
//	uint16_t tmp;
//	 char buffer1[33], buffer2[33];
	i=0; sum_x = 0; sum_y = 0;
	
	while(ads7843_is_pressed()){
			//gfx_draw_string("within",20, 180, &sysfont,GFX_COLOR_BLUE, GFX_COLOR_RED);
			ADS7843_getPosOnce( (uint16_t *)(&(p[i].x)), (uint16_t *)(&(p[i].y)) );

			
//		gfx_draw_string("after",20, 190, &sysfont,GFX_COLOR_BLUE, GFX_COLOR_RED);
		i++;
		if(i==ADS7843_READ_TIMES){
			/** sort here */
			
			for(k=0; k<ADS7843_READ_TIMES-1; k++){
				for(j=k+1; j<=ADS7843_READ_TIMES; j++){
					if(p[k].x > p[j].x){
						x_tmp = p[j].x;
						p[j].x=p[k].x;
						p[k].x=x_tmp;
					}
					if(p[k].y > p[j].y){
						y_tmp = p[j].y;
						p[j].y=p[k].y;
						p[k].y=y_tmp;
					}
				}
			}

			#if ADS7843_DEBUG
			for(k=0; k<ADS7843_READ_TIMES; k++){
				Serial.print(p[k].x);
				Serial.print(" ");
				Serial.print(p[k].y);
				Serial.print(" ");
			}
			Serial.println(" ");
			#endif

			for(k=ADS7843_LOST_VAL; k<(ADS7843_READ_TIMES-ADS7843_LOST_VAL); k++){
				sum_x += (uint32_t)p[k].x;
				sum_y += (uint32_t)p[k].y;
			}
			
			sum_x /= (ADS7843_READ_TIMES - 2*ADS7843_LOST_VAL);
			sum_y /= (ADS7843_READ_TIMES - 2*ADS7843_LOST_VAL);
			//	gfx_draw_string("SUM:",20, 210, &sysfont,GFX_COLOR_BLUE, GFX_COLOR_RED);
			#if ADS7843_DEBUG
			Serial.print("SUM:");
			Serial.print(sum_x, DEC);
			Serial.print("   ");
			Serial.print(sum_y, DEC);
			Serial.println();
			#endif
/*
			P.x = (uint16_t)sum_x;
			P.y = (uint16_t)sum_y;
*/
			P.y = (uint16_t) 240-((sum_x-100)*0.133);
	// working with slight right shift 		P.x = (uint16_t) ((sum_y-100)*0.177);
		P.x = (uint16_t) ((sum_y-100)*0.19);
/*
			itoa(P.y, buffer1,10);
			gfx_draw_string("Y:",20, 210, &sysfont,GFX_COLOR_BLACK, GFX_COLOR_WHITE);
			gfx_draw_string("     ",50, 210, &sysfont,GFX_COLOR_BLACK, GFX_COLOR_WHITE);
			gfx_draw_string(buffer1,50, 210, &sysfont,GFX_COLOR_BLACK, GFX_COLOR_WHITE);
			itoa(P.x, buffer2,10);
			gfx_draw_string("X:",20, 190, &sysfont,GFX_COLOR_BLACK, GFX_COLOR_WHITE);
			gfx_draw_string("     ",50, 190, &sysfont,GFX_COLOR_BLACK, GFX_COLOR_WHITE);
			gfx_draw_string(buffer2,50, 190, &sysfont,GFX_COLOR_BLACK, GFX_COLOR_WHITE);
*/


			#if ADS7843_DEBUG
			Serial.print("I:");
			Serial.print(P.x, DEC);
			Serial.print("   ");
			Serial.print(P.y, DEC);
			Serial.println();
			Serial.print("C:");
			#endif
			*flag = 1;
			return P;
		}
	}
	*flag = 0;
	return P;
}

void ADS7843_getPosOnce(uint16_t *x, uint16_t *y)
{

	ioport_set_pin_level(TOUCH_MASTER_SCK, false);
	ads7843_select_chip();
	ADS7843_write(0x90); // 1001 0000
	*y=ADS7843_read();
	ADS7843_write(0xD0); // 1101 0000
	*x=ADS7843_read();
	ads7843_deselect_chip();
}

