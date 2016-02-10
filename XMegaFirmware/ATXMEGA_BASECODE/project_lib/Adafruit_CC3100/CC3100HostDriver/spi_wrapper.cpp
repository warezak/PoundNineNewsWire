/*
 * spi_ported.c
 *
 * Created: 5/21/2015 5:43:22 PM
 *  Author: CLOUD 7
 */ 

#include "spi_wrapper.h"

struct spi_device spi_device_conf = {.id = CC3100_SPI_SS_PIN };

Fd_t piSpiOpen(char *ifName, unsigned long flags)
{
	ioport_configure_pin(CC3100_SPI_SS_PIN, IOPORT_DIR_OUTPUT |  IOPORT_INIT_HIGH); // cs
	ioport_configure_pin(CC3100_SPI_MOSI_PIN, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH); // mosi
	ioport_configure_pin(CC3100_SPI_MISO_PIN,  IOPORT_DIR_INPUT); //miso
	ioport_configure_pin(CC3100_SPI_SCK_PIN, IOPORT_DIR_OUTPUT | IOPORT_INIT_LOW); //sck
	
	spi_master_init(CC3100_SPI);
	spi_master_setup_device(CC3100_SPI, &spi_device_conf, SPI_MODE_0, 500000, 0);
	spi_enable(CC3100_SPI);
	
    Delay(50);
	
	CC3100_InterruptEnable();
	
	return NONOS_RET_OK;
}

int iSpiClose(Fd_t fd)
{
	 CC3100_InterruptDisable();
	
	 spi_disable(CC3100_SPI);
	
	 return NONOS_RET_OK;
}

int iSpiRead(Fd_t fd, unsigned char *pBuff, int len)
{
	int tiLengthToReturn = 0;
	char SpiData[15];
	
	spi_select_device(CC3100_SPI, &spi_device_conf);
	
	if(STATUS_OK == spi_read_packet(CC3100_SPI,pBuff,len))
	{
		tiLengthToReturn = len;
	}
	
	spi_deselect_device(CC3100_SPI, &spi_device_conf);
	
	return tiLengthToReturn;
}

int iSpiWrite(Fd_t fd, unsigned char *pBuff, int len)
{
	int tiLengthToReturn = 0;
	
	spi_select_device(CC3100_SPI, &spi_device_conf);
	
	if(STATUS_OK == spi_write_packet(CC3100_SPI,pBuff,len))
	{
		tiLengthToReturn = len;
	}
	
	spi_deselect_device(CC3100_SPI, &spi_device_conf);
	
	return tiLengthToReturn;
}
