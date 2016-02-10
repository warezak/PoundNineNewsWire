/*
 * SRAM23k256.c
 *
 * Created: 28-09-2015 14:44:43
 *  Author: Ecloud6_2
 */ 
#include "SRAM23K256.h"

struct spi_device spi_device_conf1 = {.id = SRAM23Kxx_CS };
void SendAddress(uint16_t tuAddress);
boolean ProgramToSRAM(uint8_t tuData);
uint8_t uSramToProgram(void);

void InitSRAM(void)
{
	ioport_configure_pin(SRAM23Kxx_CS, IOPORT_DIR_OUTPUT |IOPORT_INIT_HIGH);
	//ioport_configure_pin(SRAM23Kxx_SCK, IOPORT_DIR_OUTPUT | IOPORT_INIT_HIGH);
	//ioport_configure_pin(SRAM23Kxx_MOSI, IOPORT_DIR_OUTPUT |IOPORT_INIT_HIGH);
	//ioport_configure_pin(SRAM23Kxx_MISO, IOPORT_DIR_INPUT);
	
	delay_ms(10);
	
//	spi_master_init(SRAM23Kxx_SPI);
//	spi_master_setup_device(SRAM23Kxx_SPI, &spi_device_conf1, SPI_MODE_0, 1000000, 0);
//	spi_enable(SRAM23Kxx_SPI);
}

void Sram_Write_Packet(uint16_t tuAddress,uint8_t *tuDataBuffer,uint8_t tuSize)
{
	SRAM_Select_Chip();
	ProgramToSRAM(SRAM_WRITE_COMMAND);
	SendAddress(tuAddress);
	spi_write_packet(SRAM23Kxx_SPI,tuDataBuffer,tuSize);
	SRAM_DeSelect_Chip();
}

void Sram_Read_Packet(uint16_t tuAddress,uint8_t *tuDataBuffer,uint8_t tuSize)
{
	SRAM_Select_Chip();
	ProgramToSRAM(SRAM_READ_COMMAND);
	SendAddress(tuAddress);
	spi_read_packet(SRAM23Kxx_SPI,tuDataBuffer,tuSize);
	SRAM_DeSelect_Chip();
}

void Sram_Write_Byte(uint16_t tuAddress,uint8_t tuData)
{
	SRAM_Select_Chip();
	ProgramToSRAM(SRAM_WRITE_COMMAND);
	SendAddress(tuAddress);
	ProgramToSRAM(tuData);
	SRAM_DeSelect_Chip();
}
char Sram_Read_Byte(uint16_t tuAddress)
{
	SRAM_Select_Chip();
	ProgramToSRAM(SRAM_READ_COMMAND);
	SendAddress(tuAddress);
	
	char tuData =  uSramToProgram();
	SRAM_DeSelect_Chip();
	return tuData;
}

void SRAM_Select_Chip(void)
{
	spi_select_device(SRAM23Kxx_SPI,&spi_device_conf1);
}
void SRAM_DeSelect_Chip(void)
{
	spi_deselect_device(SRAM23Kxx_SPI,&spi_device_conf1);
}
void SendAddress(uint16_t tuAddress)
{
	uint8_t tuAddressMSB = ((tuAddress >> 8) & 0xFF),tuAddressLSB = (tuAddress & 0xFF);

	ProgramToSRAM(tuAddressMSB);
	ProgramToSRAM(tuAddressLSB);
}

boolean ProgramToSRAM(uint8_t tuData)
{
	if(STATUS_OK == spi_write_packet(SRAM23Kxx_SPI,&tuData,1))
	{
		return true;
	}
	return false;
}

uint8_t uSramToProgram(void)
{
	uint8_t tuRcvData;
	
	if(STATUS_OK == spi_read_packet(SRAM23Kxx_SPI,&tuRcvData,1))
	{
		  return tuRcvData;
	}
    return false;
}

