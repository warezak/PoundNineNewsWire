/*
 * spi_ported.h
 *
 * Created: 5/21/2015 5:44:04 PM
 *  Author: CLOUD 7
 */ 
#ifndef _SPI_WRAPPER_
#define _SPI_WRAPPER_

#ifdef    __cplusplus
extern "C"
{
#endif

#include <simplelink.h>
#include <spi_master.h>

typedef short* Fd_t;

Fd_t piSpiOpen(char *ifName, unsigned long flags);
int iSpiClose(Fd_t fd);
int iSpiRead(Fd_t fd, unsigned char *pBuff, int len);
int iSpiWrite(Fd_t fd, unsigned char *pBuff, int len);
#ifdef    __cplusplus
}
#endif

#endif