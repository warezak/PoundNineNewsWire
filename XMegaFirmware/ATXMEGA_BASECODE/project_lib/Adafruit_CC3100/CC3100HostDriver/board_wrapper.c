/*
 * board_wrapper.c
 *
 * Created: 5/22/2015 11:35:03 AM
 *  Author: CLOUD 7
 */ 

#include "board_wrapper.h"

P_EVENT_HANDLER     pIraEventHandler = 0;
unsigned char IntIsMasked;

void vInitClkBoard(void)
{
	sysclk_init();
	ioport_init();
	
	ioport_configure_pin(TFT_DISPLAY_SCK, IOPORT_DIR_OUTPUT	| IOPORT_INIT_HIGH);
	ioport_configure_pin(TFT_DISPLAY_MOSI, IOPORT_DIR_OUTPUT| IOPORT_INIT_HIGH);
	ioport_configure_pin(TFT_DISPLAY_MISO, IOPORT_DIR_INPUT);
	ioport_configure_pin(TFT_DISPLAY_CS, IOPORT_DIR_OUTPUT| IOPORT_INIT_HIGH);
	ioport_configure_pin(TFT_DISPLAY_CHG, IOPORT_DIR_INPUT);
	ioport_configure_pin(TFT_DISPLAY_DC, IOPORT_DIR_OUTPUT| IOPORT_INIT_LOW);
	ioport_configure_pin(TFT_DISPLAY_LCD_RESET, IOPORT_DIR_OUTPUT| IOPORT_INIT_LOW);
	//BRW 2015-11-07 update per gerald
	ioport_configure_pin(TFT_DISPLAY_BACKLIGHT, IOPORT_DIR_OUTPUT| IOPORT_INIT_LOW);
	
	ioport_configure_pin(AT45DBX_MASTER_SCK, IOPORT_DIR_OUTPUT| IOPORT_INIT_HIGH);
	ioport_configure_pin(AT45DBX_MASTER_MOSI, IOPORT_DIR_OUTPUT| IOPORT_INIT_HIGH);
	ioport_configure_pin(AT45DBX_MASTER_MISO, IOPORT_DIR_INPUT);
	ioport_configure_pin(AT45DBX_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	
	
	ioport_configure_pin(TOUCH_MASTER_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(TOUCH_MASTER_MOSI, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW);
	ioport_configure_pin(TOUCH_MASTER_MISO, IOPORT_DIR_INPUT);
	ioport_configure_pin(TOUCH_CS, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	ioport_configure_pin(TOUCH_INT, IOPORT_DIR_INPUT);
	
	ioport_configure_pin(CC3100_nHIB_PIN,IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW ); // Hib
	ioport_configure_pin(CC3100_RESET_PIN,IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH); // Reset
	
	delay_init();
	
	vWDTDisable();
}

void vWDTDisable(void)
{
	wdt_disable();
}

void vDisableCC3100(void)
{
	ioport_set_pin_level(CC3100_nHIB_PIN,0);
}

void vEnableCC3100(void)
{
	ioport_set_pin_level(CC3100_nHIB_PIN,1);
}

int iRegisterInterruptHandler(P_EVENT_HANDLER InterruptHdl , void* pValue)
{
	pIraEventHandler = InterruptHdl;

	return 0;
}

void vCC3100InterruptEnable(void)
{
	ioport_configure_pin(CC3100_IRQ_PIN,IOPORT_DIR_INPUT); // IRQ
 
	PORTD.INT0MASK = PIN0_bm;
	PORTD.INTCTRL = PORT_INT0LVL_LO_gc;

	ioport_set_pin_sense_mode(CC3100_IRQ_PIN,IOPORT_SENSE_RISING);
 
	PMIC.CTRL |= PMIC_LOLVLEN_bm;
	sei();
}

void vCC3100InterruptDisable(void)
{
	PORTD.INTCTRL = PORT_INT0LVL_OFF_gc;
	cli();
	
}

void vDelay(unsigned long interval)
{
	delay_ms(interval);
}


ISR(PORTD_INT0_vect)
{
	if (pIraEventHandler)
	{
		pIraEventHandler(0);
	}
}

void MaskIntHdlr(void)
{
    IntIsMasked = 1;
}

void UnMaskIntHdlr(void)
{
    IntIsMasked = 0;
}
