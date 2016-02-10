/*
 * SRAM23K256.h
 *
 * Created: 28-09-2015 14:45:02
 *  Author: Ecloud6_2
 */ 


#ifndef SRAM23K256_H_
#define SRAM23K256_H_

#ifdef    __cplusplus
extern "C"
{
#endif

#include "conf_board.h"
#include <spi_master.h>
#include "delay.h"

typedef bool boolean;
#define SRAM_WRITE_COMMAND 0x02
#define SRAM_READ_COMMAND 0x03

void InitSRAM(void);
void Sram_Write_Packet(uint16_t tuAddress,uint8_t *tuDataBuffer,uint8_t tuSize);
void Sram_Read_Packet(uint16_t tuAddress,uint8_t *tuDataBuffer,uint8_t tuSize);
void Sram_Write_Byte(uint16_t tuAddress,uint8_t tuData);
char Sram_Read_Byte(uint16_t tuAddress);
void SRAM_Select_Chip(void);
void SRAM_DeSelect_Chip(void);


#ifdef    __cplusplus
}
#endif

#endif /* SRAM23K256_H_ */