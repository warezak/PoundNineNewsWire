/*
 * CFile2.c
 *
 * Created: 15/02/2015 21:04:54
 *  Author: gerald
 */ 
#include <asf.h>

/**
 * \brief main function
 */
int main (void)
{
#if SAMD21 || SAMR21
	/* Initialize basic board support features.
	 * - Initialize system clock sources according to device-specific
	 *   configuration parameters supplied in a conf_clocks.h file.
	 * - Set up GPIO and board-specific features using additional configuration
	 *   parameters, if any, specified in a conf_board.h file.
	 */
	system_init();
#else
	/* Initialize basic board support features.
	 * - Initialize system clock sources according to device-specific
	 *   configuration parameters supplied in a conf_clock.h file.
	 * - Set up GPIO and board-specific features using additional configuration
	 *   parameters, if any, specified in a conf_board.h file.
	 */
	sysclk_init();
	board_init();
#endif

	// Initialize interrupt vector table support.
	irq_initialize_vectors();

	// Enable interrupts
	cpu_irq_enable();

	/* Call a local utility routine to initialize C-Library Standard I/O over
	 * a USB CDC protocol. Tunable parameters in a conf_usb.h file must be
	 * supplied to configure the USB device correctly.
	 */
	stdio_usb_init();

	// Get and echo characters forever.

	uint8_t ch;

	while (true) {

		scanf("%c",&ch); // get one input character

		if (ch) {
			//printf("%c",ch); // echo to output
			printf("testing CFILE2 "); // echo to output
		}
	}
}

