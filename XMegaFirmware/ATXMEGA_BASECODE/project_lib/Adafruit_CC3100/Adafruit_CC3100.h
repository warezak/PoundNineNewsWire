
/**************************************************************************/
/*! 
Modified by gp for xmega-a1 xplained board
v1
 
  @file     Adafruit_CC3100.h
  @author   KTOWN (Kevin Townsend for Adafruit Industries)
  @license  BSD (see license.txt) 

  This is a library for the Adafruit CC3100 WiFi breakout board
  This library works with the Adafruit CC3100 breakout
  ----> https://www.adafruit.com/products/1469
  
  Check out the links above for our tutorials and wiring diagrams 
  These chips use SPI to communicate.
  
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit! 
*/
/**************************************************************************/

#ifndef ADAFRUIT_CC3100_H
#define ADAFRUIT_CC3100_H


#include "sl_common.h"
#include "simplelink.h"
//#include "SPI.h"
//#include "Client.h"
#include "cli_uart_wrapper.h"
#include "stdio.h"
//#include "WString.h"
//#include "Print.h"
typedef uint8_t byte ;
#include "protocol.h"
#if defined(__arm__) && defined(__SAM3X8E__) // Arduino Due
  #define SPI_CLOCK_DIVIDER 6 // used to set the speed for the SPI bus; 6 == 14 Mhz on the Arduino Due
#else
  #define SPI_CLOCK_DIVIDER 16 // Don't set this to a slower speed (i.e. larger div value)
                                           // or communication will be flakey on 16mhz chips!
#endif

#define ENABLE_CC3X_PRINTER

/*#ifdef ENABLE_CC3X_PRINTER
#define CHECK_PRINTER if(CC3XPrinter != 0)
#else
#define CHECK_PRINTER if(false)
#endif*/

//bmodif for xmega cc3k printing to default serial
#define CC3X_DEFAULT_PRINTER iUartWriteData
//#if defined(UDR0) || defined(UDR1) || defined(CORE_TEENSY) || ( defined (__arm__) && defined (__SAM3X8E__) )
//  #define CC3X_DEFAULT_PRINTER &Serial 
//#else
//  #define CC3X_DEFAULT_PRINTER 0
//#endif

#define SMART_CONFIG_TIMEOUT 60000  // how long to wait for smart config to complete
#define WLAN_CONNECT_TIMEOUT 10000  // how long to wait, in milliseconds
#define RXBUFFERSIZE  100 // how much to buffer on the incoming side
#define TXBUFFERSIZE  100 // how much to buffer on the outgoing side

#define WIFI_ENABLE 1
#define WIFI_DISABLE 0
#define WIFI_STATUS_CONNECTED 1

typedef struct Result_Struct
{
  uint32_t  num_networks;
  uint32_t  scan_status;
  uint8_t   rssiByte;
  uint8_t   Sec_ssidLen;
  uint16_t  time;
  uint8_t   ssid_name[32];
  uint8_t   bssid[6];
} ResultStruct_t;   /**!ResultStruct_t : data struct to store SSID scan results */

/* Enum for wlan_ioctl_statusget results */
typedef enum 
{
  STATUS_DISCONNECTED = 0,
  STATUS_SCANNING     = 1,
  STATUS_CONNECTING   = 2,
  STATUS_CONNECTED    = 3
} status_t;

class Adafruit_CC3100;

class Adafruit_CC3100_Client {
 public:
  Adafruit_CC3100_Client(int32_t s);
  Adafruit_CC3100_Client(void);
  Adafruit_CC3100_Client(const Adafruit_CC3100_Client& copy);
  void operator=(const Adafruit_CC3100_Client& other);
  
  // NOTE: If public functions below are added/modified/removed please make sure to update the
  // Adafruit_CC3100_ClientRef class to match!

  int connect(uint32_t ip, uint16_t port);
  int connect(const char *host, uint16_t port);

  uint8_t connected(void);
  size_t write(uint8_t c);

  size_t fastrprint(const char *str);
  size_t fastrprintln(const char *str);
  size_t fastrprint(char *str);
  size_t fastrprintln(char *str);
  size_t fastrprint_n(const char *ifsh);
  size_t fastrprintln_n(const char *ifsh);

  size_t write(const void *buf, uint16_t len, uint32_t flags = 0);
  int read(void *buf, uint16_t len, uint32_t flags = 0);
  int read(void);
  int32_t close(void);
  int available(void);

  int read(uint8_t *buf, size_t size);
  size_t write(const uint8_t *buf, size_t size);
  int peek();
  void flush();
  void stop();
  operator bool();

  uint8_t _rx_buf[RXBUFFERSIZE], _rx_buf_idx;
  int16_t bufsiz;

 private:
  int32_t _socket;

};


class Adafruit_CC3100 {
  public:
    Adafruit_CC3100(uint8_t csPin, uint8_t irqPin, uint8_t vbatPin, uint8_t spispeed = SPI_CLOCK_DIVIDER);
    bool     begin(uint8_t patchReq = 0, bool useSmartConfigData = false, const char *_deviceName = NULL);
    void     reboot(uint8_t patchReq = 0);
    void     stop(void);
    bool     disconnect(void);
    bool     deleteProfiles(void);
    void     printHex(const byte * data, const uint32_t numBytes);
    void     printHexChar(const byte * data, const uint32_t numBytes);
    void     printIPdots(uint32_t ip);
    void     printIPdotsRev(uint32_t ip);
    uint32_t IP2U32(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
    bool     getMacAddress(uint8_t address[6]);
    bool     setMacAddress(uint8_t address[6]);
    bool     setStaticIPAddress(uint32_t ip, uint32_t subnetMask, uint32_t defaultGateway, uint32_t dnsServer);
    bool     setDHCP();

    bool     connectToAP(const char *ssid, const char *key, uint8_t secmode, uint8_t attempts = 0);
    bool     connectSecure(const char *ssid, const char *key, int32_t secMode);
    bool     connectOpen(const char *ssid); 
    bool     checkConnected(void);
    bool     checkDHCP(void);
    bool     getIPAddress(uint32_t *retip, uint32_t *netmask, uint32_t *gateway, uint32_t *dhcpserv, uint32_t *dnsserv);

    bool     checkSmartConfigFinished(void);

    Adafruit_CC3100_Client connectTCP(uint32_t destIP, uint16_t destPort);
    Adafruit_CC3100_Client connectUDP(uint32_t destIP, uint16_t destPort);
     
    #ifndef CC3100_TINY_DRIVER
    bool     getFirmwareVersion(uint8_t *major, uint8_t *minor);
    status_t getStatus(void);
    bool     startSSIDscan(uint32_t *index);
    void     stopSSIDscan();
    uint8_t  getNextSSIDList(const uint8_t Index, const  uint8_t Count, Sl_WlanNetworkEntry_t *pEntries);

    bool     listSSIDResults(void);
    bool     startSmartConfig(const char *_deviceName = NULL, const char *smartConfigKey = NULL, uint32_t timeout = SMART_CONFIG_TIMEOUT);
	bool     startProvisioningAP();
    bool     getIPConfig(SlNetCfgIpV4Args_t ipConfig);


    uint16_t ping(uint32_t ip, uint8_t attempts=3,  uint16_t timeout=500, uint8_t size=32);
    uint16_t getHostByName(char *hostname, uint32_t *ip);
    #endif

    /* Functions that aren't available with the tiny driver */
    #ifndef CC3100_TINY_DRIVER
    bool     scanSSIDs(uint32_t time);
    #endif

    void setPrinter();

  private:
    bool _initialised;

};

//extern Print* CC3XPrinter;

#endif
