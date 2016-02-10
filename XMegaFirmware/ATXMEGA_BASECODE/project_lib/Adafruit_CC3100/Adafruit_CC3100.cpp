 /**************************************************************************/
/*!
Modified by gp for xmega-a1 xplained board
v1 

  @file     Adafruit_CC3100.cpp
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

  @section  HISTORY

  v1.0    - Initial release
*/
/**************************************************************************/
//Atmel ASF DOES NOT compile under C++ (as of Atmel Studio 6.2) Must use C compiler
extern "C" {
	#include "asf.h"
};
#include <util/delay.h>
#include "Adafruit_CC3100.h"
#include "spi_wrapper.h"
#include "simplelink.h"

SlSecParams_t g_secParams;
_u8 g_ssid_list[SCAN_TABLE_SIZE][MAXIMAL_SSID_LENGTH + 1];
_i8 g_Wlan_Security_Key[MAX_KEY_LENGTH];
_i8 g_Wlan_SSID[MAXIMAL_SSID_LENGTH + 1];
_u8 g_Status;
_u32   g_ConnectTimeoutCnt = 0;
Sl_WlanNetworkEntry_t g_netEntries[SCAN_TABLE_SIZE];

uint8_t g_csPin, g_irqPin, g_vbatPin, g_IRQnum, g_SPIspeed;
uint32_t count;
bool bConnectionError = false;
//bmodif dreqinttable[]  is not being used for xmega implementation. Keep it for reference but IRQ set in begin method for xmega pins 
static const uint8_t dreqinttable[] = {
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || defined(__AVR_ATmega8__) 
  2, 0,
  3, 1,
#elif defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__) 
  2, 0,
  3, 1,
  21, 2, 
  20, 3,
  19, 4,
  18, 5,
#elif  defined(__AVR_ATmega32U4__) && defined(CORE_TEENSY)
  5, 0,
  6, 1,
  7, 2,
  8, 3,
#elif  defined(__AVR_AT90USB1286__) && defined(CORE_TEENSY)
  0, 0,
  1, 1,
  2, 2,
  3, 3,
  36, 4,
  37, 5,
  18, 6,
  19, 7,
#elif  defined(__arm__) && defined(CORE_TEENSY)
  0, 0, 1, 1, 2, 2, 3, 3, 4, 4,
  5, 5, 6, 6, 7, 7, 8, 8, 9, 9,
  10, 10, 11, 11, 12, 12, 13, 13, 14, 14,
  15, 15, 16, 16, 17, 17, 18, 18, 19, 19,
  20, 20, 21, 21, 22, 22, 23, 23, 24, 24,
  25, 25, 26, 26, 27, 27, 28, 28, 29, 29,
  30, 30, 31, 31, 32, 32, 33, 33,
#elif  defined(__AVR_ATmega32U4__) 
  7, 4,
  3, 0,
  2, 1,
  0, 2,
  1, 3,
#elif defined(__arm__) && defined(__SAM3X8E__) // Arduino Due  
  0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 
  5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 
  10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 
  15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 
  20, 20, 21, 21, 22, 22, 23, 23, 24, 24, 
  25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 
  30, 30, 31, 31, 32, 32, 33, 33, 34, 34, 
  35, 35, 36, 36, 37, 37, 38, 38, 39, 39, 
  40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 
  45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 
  50, 50, 51, 51, 52, 52, 53, 53, 54, 54, 
  55, 55, 56, 56, 57, 57, 58, 58, 59, 59, 
  60, 60, 61, 61, 62, 62, 63, 63, 64, 64, 
  65, 65, 66, 66, 67, 67, 68, 68, 69, 69,
  70, 70, 71, 71,
#endif
};

/***********************/

uint8_t pingReportnum;
SlPingReport_t pingReport;

#define CC3100_SUCCESS                        (0)

#define CHECK_SUCCESS(func,Notify,errorCode)  {if ((func) != CC3100_SUCCESS) {(unsigned char*)CLI_Write(Notify); return errorCode;}} 
//#define CHECK_SUCCESS(func,Notify,errorCode)  {if ((func) != CC3100_SUCCESS) { printf("*E*");return errorCode;}} 
//debuggerald
//#define CHECK_SUCCESS(func,Notify,errorCode)  {if ((func) != CC3100_SUCCESS) { printf("Notify:");printf(Notify);printf("   ErrorCode:");printf("%d",errorCode);return errorCode;}}
//#define CHECK_SUCCESS2(func,Notify,errorCode)  {if ((func) != CC3100_SUCCESS) { printf("Notify:");printf(Notify);printf("   ErrorCode:");printf("%d",errorCode);return errorCode;}}

#define MAXSSID           (32)
#define MAXLENGTHKEY      (32)  /* Cleared for 32 bytes by TI engineering 29/08/13 */

#define MAX_SOCKETS 32  // can change this
uint8_t closed_sockets[MAX_SOCKETS] = {false, false, false, false};


/* *********************************************************************** */
/*                                                                         */
/* PRIVATE FIELDS (SmartConfig)                                            */
/*                                                                         */
/* *********************************************************************** */

class CC3100BitSet {
public:
  static const byte IsSmartConfigFinished = 0x01;
  static const byte IsConnected = 0x02;
  static const byte IsStationConnected = 0x04;
  static const byte IsIPAcquired = 0x08;
  static const byte SmartConfigStoped =  0x10;
  static const byte IsApProvisioningDone = 0x20;
  static const byte IsProfileAdded = 0x40;
  static const byte IsConnectedToConfAp = 0x80;

  void clear() {
    flags = 0;
  }

  bool test(const byte flag) {
    return (flags & flag) != 0;
  }

  void set(const byte flag) {
    flags |= flag;
  }

  void reset(const byte flag) {
    flags &= ~flag;
  }
private:
  volatile byte flags;
}cc3100Bitset;

volatile long ulSocket;
char _cc3100_prefix[] = { 'T', 'T', 'T' };
//Print* CC3XPrinter; // user specified output stream for general messages and debug

/* *********************************************************************** */
/*                                                                         */
/* PRIVATE FUNCTIONS                                                       */
/*                                                                         */
/* *********************************************************************** */


/**************************************************************************/
/*!
    @brief    Scans for SSID/APs in the CC3100's range

    @note     This command isn't available when the CC3100 is configured
              in 'CC3100_TINY_DRIVER' mode

    @returns  False if an error occured!
*/
/**************************************************************************/
#ifndef CC3100_TINY_DRIVER
bool Adafruit_CC3100::scanSSIDs(uint32_t time)
{
  if (!_initialised)
  {
    return false;
  }
  // We can abort a scan with a time of 0
  if (time)
  {
    //CHECK_PRINTER {
     CLI_Write("Started AP/SSID scan\n\r");
   // }
    // Set  SSID Scan params to includes channels above 11 
    CHECK_SUCCESS(
      sl_WlanPolicySet(SL_POLICY_SCAN, 1,
      (uint8_t*)&time, sizeof(time)),
      "Failed setting params for SSID scan", false);
  }
  else
  {
    // Set  SSID Scan params to includes channels above 11
    CHECK_SUCCESS(
      sl_WlanPolicySet(SL_POLICY_SCAN, 0, 0, 0),
          "Failed setting params for SSID scan", false);
  }
  return true;
}
#endif

/* *********************************************************************** */
/*                                                                         */
/* CONSTRUCTORS                                                            */
/*                                                                         */
/* *********************************************************************** */

/**************************************************************************/
/*!
    @brief  Instantiates a new CC3100 class.
            Note that by default this class will assume the first hardware 
            serial should be used for debug output.  This behavior can be
            changed by explicitly specifying a cc3xPrinter parameter.
*/
/**************************************************************************/
Adafruit_CC3100::Adafruit_CC3100(uint8_t csPin, uint8_t irqPin, uint8_t vbatPin, uint8_t SPIspeed)
{
  _initialised = false;

  g_csPin = csPin;
  g_irqPin = irqPin;
  g_vbatPin = vbatPin;
  // g_IRQnum is set to 0xFF to verify validity further away,
  g_IRQnum = 0xFF;
  g_SPIspeed = SPIspeed;

  cc3100Bitset.clear();

  //comment out,CC3XPrinter = cc3xPrinter;
}

/* *********************************************************************** */
/*                                                                         */
/* PUBLIC FUNCTIONS                                                        */
/*                                                                         */
/* *********************************************************************** */
bool establishConnectionWithAP()
{
	sl_WlanProfileAdd(g_Wlan_SSID, strlen((char*)g_Wlan_SSID), 0, &g_secParams,0, 7, 0);
	
	while (g_ConnectTimeoutCnt < 2000 && ((!cc3100Bitset.test(CC3100BitSet ::IsConnected)) || (!cc3100Bitset.test(CC3100BitSet ::IsIPAcquired))))
	{
		_SlNonOsMainLoopTask();
		Delay(1);
		g_ConnectTimeoutCnt++;
	}
	g_ConnectTimeoutCnt = 0;
	
	return SUCCESS;
}
// It Filter's a list of duplicate ssid's
void FilterList()
{
	_u8 idx1 = 0;
	_u8 idx2 = 0;

	pal_Memset(g_ssid_list, '\0', (SCAN_TABLE_SIZE * (MAXIMAL_SSID_LENGTH + 1)));

	for(idx1=0; idx1 < SCAN_TABLE_SIZE; idx1++)
	{
		if(g_netEntries[idx1].ssid[0] != '\0')
		{
			for(idx2=0; idx2 < SCAN_TABLE_SIZE; idx2++)
			{
				if(pal_Strcmp(g_netEntries[idx1].ssid,
				g_ssid_list[idx2]) == 0)
				break;
				if(g_ssid_list[idx2][0] == '\0')
				{
					pal_Strcpy(g_ssid_list[idx2],
					g_netEntries[idx1].ssid);
					break;
				}
			}
		}
	}
}

//  Enable/Disable the authentication check for http server
void set_authentication_check (_u8 enable)
{
	_NetAppHttpServerGetSet_auth_enable_t auth_enable;
	auth_enable.auth_enable = enable;
	sl_NetAppSet(SL_NET_APP_HTTP_SERVER_ID, NETAPP_SET_GET_HTTP_OPT_AUTH_CHECK,sizeof(_NetAppHttpServerGetSet_auth_enable_t), (_u8 *)&auth_enable);
}

// initializes the provisioning ap application variables
bool initializeAppVariables()
{
	g_Status = 0;
	g_ConnectTimeoutCnt = 0;
	pal_Memset(g_Wlan_SSID, 0, (MAXIMAL_SSID_LENGTH + 1));
	pal_Memset(g_Wlan_Security_Key, 0, MAX_KEY_LENGTH);
	pal_Memset(g_netEntries, 0, sizeof(g_netEntries));
	pal_Memset(g_ssid_list, 0, sizeof(g_ssid_list));
	pal_Memset(&g_secParams, 0, sizeof(SlSecParams_t));

	return SUCCESS;
}
void configureSimpleLinkToDefaultState()
{
	_WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};

	_u8           val = 1;
	_u8           configOpt = 0;
	_u8           configLen = 0;
	_u8           power = 0;

	_i32          retVal = -1;
	_i32          mode = -1;
	
	sl_Stop(SL_STOP_TIMEOUT);
	delay_ms(SL_STOP_TIMEOUT);
	mode = sl_Start(0, 0, 0);
	if (ROLE_STA != mode)
	{
		if (ROLE_AP == mode)
		{
			while(!cc3100Bitset.test(CC3100BitSet ::IsIPAcquired)) { _SlNonOsMainLoopTask(); }
		}
		sl_WlanSetMode(ROLE_STA);
		sl_Stop(SL_STOP_TIMEOUT);
		sl_Start(0, 0, 0);
	}
	// Set connection policy to Auto + SmartConfig (Device's default connection policy)
	sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);

	sl_WlanProfileDel(0xFF);
	retVal = sl_WlanDisconnect();
	if(0 == retVal)
	{
		while(cc3100Bitset.test(CC3100BitSet ::IsConnected)) { _SlNonOsMainLoopTask(); }
	}
	
	sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&val);

	configOpt = SL_SCAN_POLICY(0);
	sl_WlanPolicySet(SL_POLICY_SCAN , configOpt, NULL, 0);

	// Set Tx power level for station mode
	power = 0;
	sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (_u8 *)&power);

	// Set PM policy to normal
	sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);

	// Unregister mDNS services
	sl_NetAppMDNSUnRegisterService(0, 0);

	// Remove  all 64 filters (8*8)
	pal_Memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
	sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask, sizeof(_WlanRxFilterOperationCommandBuff_t));
	sl_Stop(SL_STOP_TIMEOUT);
	
	initializeAppVariables();
}

// implements the ProvisioningAP functionality

bool Adafruit_CC3100 :: startProvisioningAP()
{
	_u32 intervalInSeconds = SCAN_INTERVAL;
	_i32 retVal = -1;
	_u8 SecType = 0;
	
	if (!_initialised)      return false;
	configureSimpleLinkToDefaultState();
	cc3100Bitset.clear();
	cc3100Bitset.reset(CC3100BitSet ::IsProfileAdded);
	sl_Start(0, 0, 0);
	
	CLI_Write("Started provisioningAp in adafruitcc3100\n\r");
	
	sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SSID,pal_Strlen(SSID_AP_MODE), (_u8 *)SSID_AP_MODE);

	SecType = SEC_TYPE_AP_MODE;
	sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_SECURITY_TYPE, 1,(_u8 *)&SecType);

	sl_WlanSet(SL_WLAN_CFG_AP_ID, WLAN_AP_OPT_PASSWORD, pal_Strlen(PASSWORD_AP_MODE),(_u8 *)PASSWORD_AP_MODE);

	set_authentication_check(FALSE);

	sl_WlanProfileDel(0xFF);
	
	sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 1), NULL, 0);

	sl_Stop(SL_STOP_TIMEOUT);

	cc3100Bitset.reset(CC3100BitSet ::IsIPAcquired);
	cc3100Bitset.reset(CC3100BitSet ::IsConnected);

	// Initializing the CC3100 device
	retVal = sl_Start(0, 0, 0);
	if ((retVal < 0) || (ROLE_STA != retVal) )
	{
		CLI_Write("failed to start a device n\r");
	}

	CLI_Write(" Configuring device in AP mode - SSID: ");
	CLI_Write(SSID_AP_MODE);
	CLI_Write("\r\n");

	while(!(cc3100Bitset.test(CC3100BitSet ::IsApProvisioningDone)))
	{

		// Set the scan policy
		sl_WlanPolicySet(SL_POLICY_SCAN,SL_SCAN_POLICY_EN(1),(_u8 *)&intervalInSeconds,sizeof(intervalInSeconds));

		Delay(250);

		sl_WlanGetNetworkList(0,20,&g_netEntries[0]);
		FilterList();

		sl_WlanSetMode(ROLE_AP);
		sl_Stop(SL_STOP_TIMEOUT);

		cc3100Bitset.reset(CC3100BitSet ::IsIPAcquired);
		cc3100Bitset.reset(CC3100BitSet ::IsConnected);

		// Initializing the CC3100 device
		retVal = sl_Start(0, 0, 0);
		if ((retVal < 0) ||(ROLE_AP != retVal) )
		{
			CLI_Write("failed to start the device\n\r");
		}

		// Wait for CC3100 to Acquired IP in AP Mode
		while (!cc3100Bitset.test(CC3100BitSet ::IsIPAcquired))
		{
			_SlNonOsMainLoopTask();
		}
		while((!cc3100Bitset.test(CC3100BitSet ::IsProfileAdded)) && (!cc3100Bitset.test(CC3100BitSet ::IsApProvisioningDone)))
		{
			_SlNonOsMainLoopTask();
		}
		cc3100Bitset.reset(CC3100BitSet ::IsProfileAdded);

		sl_WlanSetMode(ROLE_STA);
		sl_Stop(SL_STOP_TIMEOUT);

		cc3100Bitset.reset(CC3100BitSet ::IsIPAcquired);
		cc3100Bitset.reset(CC3100BitSet ::IsConnected);
		
		retVal = sl_Start(0, 0, 0);
		if ((retVal < 0) || (ROLE_STA != retVal) )
		{
			CLI_Write(" Failed to start the device in STA mode \n\r");
		}

		if (!cc3100Bitset.test(CC3100BitSet ::IsApProvisioningDone))
		{

			// Delete all stored profiles
			sl_WlanProfileDel(0xFF);

			// Enables the auto-connect policy
			sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
			
			if(establishConnectionWithAP() != SUCCESS)
			{
				CLI_Write(" Failed to establish connection w/ AP \n\r");
			}

			if(cc3100Bitset.test(CC3100BitSet ::IsConnected))
			cc3100Bitset.set(CC3100BitSet ::IsConnectedToConfAp);
			else
			cc3100Bitset.reset(CC3100BitSet ::IsConnectedToConfAp);

			// Disable the auto-connect policy
			sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 1), NULL, 0);
			retVal = sl_WlanDisconnect();
			if(0 == retVal)
			{
				while(cc3100Bitset.test(CC3100BitSet ::IsConnected)) { _SlNonOsMainLoopTask(); }
			}
		}
	}

	// Enables the auto-connect policy
	sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);

	while ((!cc3100Bitset.test(CC3100BitSet ::IsConnected)) || (!cc3100Bitset.test(CC3100BitSet ::IsIPAcquired)))
	{
		_SlNonOsMainLoopTask();
	}

	return true;
}

/**************************************************************************/
/*!
    @brief  Setups the HW
    
    @args[in] patchReq  
              Set this to true if we are starting a firmware patch,
              otherwise false for normal operation
    @args[in] useSmartConfig
              Set this to true if you want to use the connection details
              that were stored on the device from the SmartConfig process,
              otherwise false to erase existing profiles and start a
              clean connection
*/
/**************************************************************************/
bool Adafruit_CC3100::begin(uint8_t patchReq, bool useSmartConfigData, const char *_deviceName)
{
  if (_initialised) return true;
  #if defined(digitalPinToInterrupt)
  // digitalPinToInterrupt macro is supported on Arduino 1.0.6+ and 1.5.6+
  // returns NOT_AN_INTERRUPT (-1 = 0xFF) if g_irqPin is not mapped to an INT
  #ifndef NOT_AN_INTERRUPT
    #define NOT_AN_INTERRUPT (-1)
  #endif
  //g_IRQnum = digitalPinToInterrupt(g_irqPin);
  #elif !defined(CORE_ADAX)
  #endif
  // determine irq #
  //bmodif set g_IRQnum = g_irqPin unconditionally since this library will only be used for xmega.
  //  g_IRQnum = g_irqPin;
  // stated that (almost) every single pin on Xmega supports interrupt.
  // bmodif need to verify that in using another Xmega chip than a serie - this tested on A1
  //bmodif commenting below - setup IRQ for atmega
  //for (uint8_t i=0; i<sizeof(dreqinttable); i+=2) {
	//if (g_irqPin == dreqinttable[i]) {
   //   g_IRQnum = dreqinttable[i+1];
   // }
  //}
 /* if (g_IRQnum == 0xFF) {
    CHECK_PRINTER {
      CC3XPrinter->println(F("IRQ pin is not an INT pin!"));
    }
	//bmodif, add notification something wrong with irq
	//CC3XPrinter->println(F("Problem setting IRQ pin on Xmega")); 
    return false;
  }
  #else
  //bmodif make direct correspondance for IRQ pin  on  xmega
  g_IRQnum = g_irqPin;
  // (almost) every single pin on Xmega supports interrupt
  #endif*/

  //bmodif
  //Serial.println(F("\n kdebug k4 about to init_spi")); 
  piSpiOpen(0, 0);

  sl_Start((void*)patchReq, NULL, NULL);

  // Check if we should erase previous stored connection details
  // (most likely written with data from the SmartConfig app)
  if (!useSmartConfigData)
  {
    // Manual connection only (no auto, profiles, etc.)
    sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0);
    // Delete previous profiles from memory
    sl_WlanProfileDel(255);
  }
  else
  {
    // Auto Connect - the C3000 device tries to connect to any AP it detects during scanning:
    // wlan_ioctl_set_connection_policy(1, 0, 0)
    
    // Fast Connect - the CC3100 device tries to reconnect to the last AP connected to:
    // wlan_ioctl_set_connection_policy(0, 1, 0)

    // Use Profiles - the CC3100 device tries to connect to an AP from profiles:
    sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 0), NULL, 0);
  }

  _initialised = true;

  // Wait for re-connection if we're using SmartConfig data
  if (useSmartConfigData)
  {
    // Wait for a connection
    uint32_t timeout = 0;
    while(!cc3100Bitset.test(CC3100BitSet::IsConnected))
    {
      _SlNonOsMainLoopTask();
      if(timeout > WLAN_CONNECT_TIMEOUT)
      {
       // CHECK_PRINTER {
         CLI_Write("Timed out using SmartConfig data");
       // }
        return false;
      }
      timeout += 10;
      delay_ms(10);
    }
    
    delay_ms(1000);  
    if (cc3100Bitset.test(CC3100BitSet::IsIPAcquired))
    {
      //mdnsAdvertiser(1, (char *) _deviceName, strlen(_deviceName));
    }
  }
    
  return true;
}

/**************************************************************************/
/*!
    @brief  Prints a hexadecimal value in plain characters

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void Adafruit_CC3100::printHex(const byte * data, const uint32_t numBytes)
{
  //if (CC3XPrinter == 0) return;

 uint32_t szPos,tuNumOfByte = numBytes;
 char ptcHexNum;
 for (szPos=0; szPos < tuNumOfByte; szPos++)
 {
	 CLI_Write("0x");
	 sprintf(&ptcHexNum,"%02x",*data);
	 data++;
	 CLI_Write(&ptcHexNum);
	 
	 if(szPos >= (tuNumOfByte - 1))
	 {
		 break;
	 }
	 CLI_Write(":");
 }
 
 CLI_Write("\r\n");
  //CC3XPrinter->println();
}

/**************************************************************************/
/*!
    @brief  Prints a hexadecimal value in plain characters, along with
            the char equivalents in the following format

            00 00 00 00 00 00  ......

    @param  data      Pointer to the byte data
    @param  numBytes  Data length in bytes
*/
/**************************************************************************/
void Adafruit_CC3100::printHexChar(const byte * data, const uint32_t numBytes)
{
  //if (CC3XPrinter == 0) return;

  uint32_t szPos;
  for (szPos=0; szPos < numBytes; szPos++)
  {
    // Append leading 0 for small values
    if (data[szPos] <= 0xF)
      CLI_Write('0');
    CLI_Write(&data[szPos]);
    if ((numBytes > 1) && (szPos != numBytes - 1))
    {
      CLI_Write(' ');
    }
  }
 CLI_Write("  ");
  for (szPos=0; szPos < numBytes; szPos++)
  {
    if (data[szPos] <= 0x1F)
     CLI_Write('.');
    else
      CLI_Write(&data[szPos]);
  }
  //CC3XPrinter->println();
}

/**************************************************************************/
/*!
    @brief  Helper function to display an IP address with dots
*/
/**************************************************************************/

void Adafruit_CC3100::printIPdots(uint32_t ip) {
 // if (CC3XPrinter == 0) {
 // return;
 // }
  
  char tcIPBuff[15];
  sprintf(tcIPBuff,"%d.%d.%d.%d",uint8_t(ip),uint8_t(ip >> 8),uint8_t(ip >> 16),uint8_t(ip >> 24));
  CLI_Write(&tcIPBuff);
  CLI_Write("\r\n");
  
}

/**************************************************************************/
/*!
    @brief  Helper function to display an IP address with dots, printing
            the bytes in reverse order
*/
/**************************************************************************/
void Adafruit_CC3100::printIPdotsRev(uint32_t ip) {
  //  if (CC3XPrinter == 0) {
  //return;
  //}
  
 char tcIPBuff[15];
 sprintf(tcIPBuff,"%d.%d.%d.%d",uint8_t(ip>> 24),uint8_t(ip >> 16),uint8_t(ip >> 8),uint8_t(ip));
 CLI_Write(&tcIPBuff);
 //CLI_Write("\r\n");
  
}

/**************************************************************************/
/*!
    @brief  Helper function to convert four bytes to a U32 IP value
*/
/**************************************************************************/
uint32_t Adafruit_CC3100::IP2U32(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
  uint32_t ip = a;
  ip <<= 8;
  ip |= b;
  ip <<= 8;
  ip |= c;
  ip <<= 8;
  ip |= d;

  return ip;
}

/**************************************************************************/
/*!
    @brief   Reboot CC3100 (stop then start)
*/
/**************************************************************************/
void Adafruit_CC3100::reboot(uint8_t patch)
{
  if (!_initialised)
  {
    return;
  }

  sl_Stop(0);
  delay_ms(4000);
  sl_Start((const void*)patch, NULL, NULL);
}

/**************************************************************************/
/*!
    @brief   Stop CC3100
*/
/**************************************************************************/
void Adafruit_CC3100::stop(void)
{
  if (!_initialised)
  {
    return;
  }

  sl_Stop(0);
}

/**************************************************************************/
/*!
    @brief  Disconnects from the network

    @returns  False if an error occured!
*/
/**************************************************************************/
bool Adafruit_CC3100::disconnect(void)
{
  if (!_initialised)
  {
    return false;
  }

  long retVal = sl_WlanDisconnect();

  return true;
}

/**************************************************************************/
/*!
    @brief   Deletes all profiles stored in the CC3100

    @returns  False if an error occured!
*/
/**************************************************************************/
bool Adafruit_CC3100::deleteProfiles(void)
{
  if (!_initialised)
  {
    return false;
  }

  CHECK_SUCCESS(sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0),
               "deleteProfiles connection failure", false);
  CHECK_SUCCESS(sl_WlanProfileDel((int16_t)255),
               "Failed deleting profiles", false);

  return true;
}

/**************************************************************************/
/*!
    @brief    Reads the MAC address

    @param    address  Buffer to hold the 6 byte Mac Address

    @returns  False if an error occured!
*/
/**************************************************************************/
bool Adafruit_CC3100::getMacAddress(uint8_t address[6])
{
  if (!_initialised)
  {
    return false;
  }

  CHECK_SUCCESS(sl_NetCfgGet(SL_MAC_ADDRESS_GET,NULL, (uint8_t *)&address, (uint8_t *)address),
                "Failed reading MAC address!", false);

  return true;
}

/**************************************************************************/
/*!
    @brief    Sets a new MAC address

    @param    address   Buffer pointing to the 6 byte Mac Address

    @returns  False if an error occured!
*/
/**************************************************************************/
bool Adafruit_CC3100::setMacAddress(uint8_t address[6])
{
  if (!_initialised)
  {
    return false;
  }

  if (address[0] == 0)
  {
    return false;
  }

  CHECK_SUCCESS(sl_NetCfgSet(SL_MAC_ADDRESS_SET, 1, SL_MAC_ADDR_LEN,(uint8_t*)address),
                "Failed setting MAC address!", false);

  sl_Stop(0);
  delay_ms(200);
  sl_Start(0, 0, 0);

  return true;
}

/**************************************************************************/
/*!
    @brief    Set the CC3100 to use a static IP address when it's connected
              to the network.  Use the cc3100.IP2U32 function to specify the
              IP, subnet mask (typically 255.255.255.0), default gateway
              (typically 192.168.1.1), and DNS server (can use Google's DNS
              of 8.8.8.8 or 8.8.4.4).  Note that the static IP configuration
              will be saved in the CC3100's non-volatile storage and reused
              on next reconnect.  This means you only need to call this once
              and the CC3100 will remember the setting forever.  To revert
              back to use DHCP, call the cc3100.setDHCP function.

    @param    ip               IP address
    @param    subnetmask       Subnet mask
    @param    defaultGateway   Default gateway
    @param    dnsServer        DNS server

    @returns  False if an error occurred, true if successfully set.
*/
/**************************************************************************/
bool Adafruit_CC3100::setStaticIPAddress(uint32_t ip, uint32_t subnetMask, uint32_t defaultGateway, uint32_t dnsServer)
{
  // Reverse order of bytes in parameters so IP2U32 packed values can be used with the netapp_dhcp function.
  SlNetCfgIpV4Args_t ipV4;
  ipV4.ipV4 = (ip >> 24) | ((ip >> 8) & 0x0000FF00L) | ((ip << 8) & 0x00FF0000L) | (ip << 24);                 
  ipV4.ipV4Mask = (subnetMask >> 24) | ((subnetMask >> 8) & 0x0000FF00L) | ((subnetMask << 8) & 0x00FF0000L) | (subnetMask << 24);
  ipV4.ipV4Gateway = (defaultGateway >> 24) | ((defaultGateway >> 8) & 0x0000FF00L) | ((defaultGateway << 8) & 0x00FF0000L) | (defaultGateway << 24);
  ipV4.ipV4DnsServer = (dnsServer >> 24) | ((dnsServer >> 8) & 0x0000FF00L) | ((dnsServer << 8) & 0x00FF0000L) | (dnsServer << 24);

  // Update DHCP state with specified values.
  if (sl_NetCfgSet(SL_IPV4_STA_P2P_CL_STATIC_ENABLE, IPCONFIG_MODE_ENABLE_IPV4, sizeof(SlNetCfgIpV4Args_t), (uint8_t *)&ipV4) != 0) {
    return false;
  }
  // Reset CC3100 to use modified setting.
  sl_Stop(0);
  delay_ms(200);
  sl_Start(0, 0, 0);
  return true;
}

/**************************************************************************/
/*!
    @brief    Set the CC3100 to use request an IP and network configuration
              using DHCP.  Note that this DHCP state will be saved in the 
              CC3100's non-volatile storage and reused on next reconnect.
              This means you only need to call this once and the CC3100 will 
              remember the setting forever.  To switch to use a static IP,
              call the cc3100.setStaticIPAddress function.

    @returns  False if an error occurred, true if successfully set.
*/
/**************************************************************************/
bool Adafruit_CC3100::setDHCP()
{
  return setStaticIPAddress(0,0,0,0);
}

/**************************************************************************/
/*!
    @brief   Reads the current IP address

    @returns  False if an error occured!
*/
/**************************************************************************/
bool Adafruit_CC3100::getIPAddress(uint32_t *retip, uint32_t *netmask, uint32_t *gateway, uint32_t *dhcpserv, uint32_t *dnsserv)
{
  uint8_t len = sizeof(SlNetCfgIpV4Args_t);
  uint8_t dhcpIsOn = 0;
  
  if (!_initialised) return false;
  if (!cc3100Bitset.test(CC3100BitSet::IsConnected)) return false;
  if (!cc3100Bitset.test(CC3100BitSet::IsIPAcquired)) return false;

  SlNetCfgIpV4Args_t ipV4 = {0};
  sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO, &dhcpIsOn, &len, (uint8_t *)&ipV4);

  /* If byte 1 is 0 we don't have a valid address */
  if (*((uint8_t*)(&ipV4.ipV4)) == 0) return false;

  *retip = ipV4.ipV4;
  *netmask = ipV4.ipV4Mask;
  *gateway = ipV4.ipV4Gateway;
  *dnsserv = ipV4.ipV4DnsServer;

  return true;
}

/**************************************************************************/
/*!
    @brief    Gets the two byte ID for the firmware patch version

    @note     This command isn't available when the CC3100 is configured
              in 'CC3100_TINY_DRIVER' mode

    @returns  False if an error occured!
*/
/**************************************************************************/
#ifndef CC3100_TINY_DRIVER
bool Adafruit_CC3100::getFirmwareVersion(uint8_t *major, uint8_t *minor)
{
  uint8_t configOpt;
  uint8_t configLen;

  if (!_initialised)
  {
    return false;
  }

  SlVersionFull ver;
  configLen = sizeof(ver);
  configOpt = SL_DEVICE_GENERAL_VERSION;
  CHECK_SUCCESS(sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &configOpt, &configLen, (uint8_t *)(&ver)),
                "Unable to read the firmware version", false);

  *major = ver.ChipFwAndPhyVersion.FwVersion[0];
  *minor = ver.ChipFwAndPhyVersion.FwVersion[3];

  return true;
}
#endif

/**************************************************************************/
/*!
    @Brief   Prints out the current status flag of the CC3100

    @note     This command isn't available when the CC3100 is configured
              in 'CC3100_TINY_DRIVER' mode
*/
/**************************************************************************/
#ifndef CC3100_TINY_DRIVER
status_t Adafruit_CC3100::getStatus()
{
  uint32_t statusWlan;
  uint8_t configOpt;
  uint8_t configLen;
  uint8_t results;
  
  if (!_initialised)
  {
    return STATUS_DISCONNECTED;
  }

  configOpt = SL_EVENT_CLASS_WLAN;
  configLen = sizeof(uint32_t);

  sl_DevGet(SL_DEVICE_STATUS, &configOpt, &configLen, (uint8_t *)(&statusWlan));
  results = (uint8_t)(statusWlan >> 24);

  switch(results)
  {
    case 1:
      return STATUS_SCANNING;
      break;
    case 2:
      return STATUS_CONNECTING;
      break;
    case 128:
      return STATUS_CONNECTED;
      break;
    case 0:
    default:
      return STATUS_DISCONNECTED;
      break;
  }
}
#endif

/**************************************************************************/
/*!
    @brief    Calls listSSIDs and then displays the results of the SSID scan

              For the moment we only list these via CC3XPrinter->print since
              this can consume a lot of memory passing all the data
              back with a buffer

    @note     This command isn't available when the CC3100 is configured
              in 'CC3100_TINY_DRIVER' mode

    @returns  False if an error occured!
*/
/**************************************************************************/
#ifndef CC3100_TINY_DRIVER



bool Adafruit_CC3100::startSSIDscan(uint32_t *index) {
  if (!_initialised)
  {
    return false;
  }
	Sl_WlanNetworkEntry_t netEntries[20];
	_u32  policyVal = 0;
	_i32  mode = -1;


	sl_Stop(SL_STOP_TIMEOUT);
	delay_ms(100);

	mode = sl_Start(0, 0, 0);

	if (ROLE_STA != mode)
	{
		if (ROLE_AP == mode)
		{
			while(!cc3100Bitset.test(CC3100BitSet :: IsIPAcquired)) { _SlNonOsMainLoopTask(); }
		}
		sl_WlanSetMode(ROLE_STA);
		sl_Stop(SL_STOP_TIMEOUT);
		sl_Start(0, 0, 0);
	}
	sl_Stop(SL_STOP_TIMEOUT);

	sl_Start(0, 0, 0);

	sl_WlanPolicySet(SL_POLICY_CONNECTION , SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0);
	policyVal = SCAN_INTERVAL;
	sl_WlanPolicySet(SL_POLICY_SCAN , SL_SCAN_POLICY(1), (_u8 *)&policyVal, sizeof(policyVal));

	Delay(1000);
	*index = sl_WlanGetNetworkList(0, 19,&netEntries[0]);
	return true;
}

void Adafruit_CC3100::stopSSIDscan(void) {

  // Stop scanning
  scanSSIDs(0);
}

uint8_t Adafruit_CC3100::getNextSSIDList(const uint8_t Index, const uint8_t Count, Sl_WlanNetworkEntry_t *pEntries) {
  return sl_WlanGetNetworkList(Index, Count, pEntries);
}
#endif

/**************************************************************************/
/*!
    @brief    Starts the smart config connection process

    @note     This command isn't available when the CC3100 is configured
              in 'CC3100_TINY_DRIVER' mode

    @returns  False if an error occured!
*/
/**************************************************************************/
#ifndef CC3100_TINY_DRIVER
bool Adafruit_CC3100::startSmartConfig(const char *_deviceName, const char *smartConfigKey, uint32_t timeout)
{
	cc3100Bitset.clear();
   configureSimpleLinkToDefaultState(); // To DO bhautik,make cc3100 in default mode

   sl_Start(0,0,0); // to do bhautik,trigger cc3100
    //CHECK_PRINTER {
	   CLI_Write("CHECK_PRINTER working");
   // }
  
 CLI_Write("Started startSmartconfig in adafruitcc3100\n\r");
  bool enableAES = smartConfigKey != NULL;
  //debuggerald
  timeout = 40000;

  uint32_t   time = 0;
  //usb printing
  //debuggerald
 // printf("StartSmartconfig"); printf("\r\n"); //printf("%d",h);

  if (!_initialised) {
	  //usb printing
	  //printf("return false cc3100 not initialized"); printf("\r\n");
    return false;
  }

  // Reset all the previous configurations
  CHECK_SUCCESS(sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(WIFI_DISABLE, WIFI_DISABLE, WIFI_DISABLE, WIFI_DISABLE, WIFI_DISABLE), NULL, 0),
                "Failed setting the connection policy", false);
  
  // Delete existing profile data
  CHECK_SUCCESS(sl_WlanProfileDel(255),
                "Failed deleting existing profiles", false);

  CLI_Write("Disconnecting");
  // Wait until CC3100 is disconnected
  while (cc3100Bitset.test(CC3100BitSet::IsConnected)) {
    //cc3k_int_poll();
    _SlNonOsMainLoopTask();
    CHECK_SUCCESS(sl_WlanDisconnect(),
                  "Failed to disconnect from AP", false);
    delay_ms(10);
    //hci_unsolicited_event_handler();
  }

  // Reset the CC3100
  sl_Stop(0);
  delay_ms(1000);
  sl_Start(0, 0, 0);

  if (enableAES)
  {
    //CC3XPrinter->println("Start config");
    // Start the SmartConfig start process
    CHECK_SUCCESS(sl_WlanSmartConfigStart(0,              // groupIdBitmask
                  SMART_CONFIG_CIPHER_AES,                // cipher
                  16,                                     // publicKeyLen
                  0,                                      // group1KeyLen
                  0,                                      // group2KeyLen
                  (const uint8_t *)smartConfigKey,        // publicKey
                  (const uint8_t *)"",                    // group1Key
                  (const uint8_t *)""),                   // group2Key
                  "Failed starting smart config", false);
  } 
  else
  {
    CHECK_SUCCESS(sl_WlanSmartConfigStart(0,              // groupIdBitmask
                  SMART_CONFIG_CIPHER_NONE,               // cipher
                  0,                                      // publicKeyLen
                  0,                                      // group1KeyLen
                  0,                                      // group2KeyLen
                  (const uint8_t *)"",                    // publicKey
                  (const uint8_t *)"",                    // group1Key
                  (const uint8_t *)""),                   // group2Key
                  "Failed starting smart config", false);
  }

  // Wait for smart config process complete
  while (!cc3100Bitset.test(CC3100BitSet::IsSmartConfigFinished))
  {
    _SlNonOsMainLoopTask();
    //cc3k_int_poll();
    // waiting here for event SIMPLE_CONFIG_DONE
    time+=10;
    if (time > timeout)   // default of 60s
    {
      //usb printing
      //printf("timeout:"); printf("%d",time);printf("\r\n"); //
	  //printf("Returning false, timeout: "); printf("%d",timeout);printf("\r\n"); //
	  return false;
    }
    //delay_ms(10); // ms
	delay_ms(10);
     CLI_Write(".");
  }

  //CHECK_PRINTER {
   CLI_Write("Got smart config data");
    delay_ms(2000);
//  }
  // Connect automatically to the AP specified in smart config settings
  CHECK_SUCCESS(sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(WIFI_ENABLE, WIFI_DISABLE, WIFI_DISABLE, WIFI_DISABLE, WIFI_DISABLE), NULL, 0),
                "Failed setting connection policy", false);
  
  //usb printing
  printf("Reset");printf("\r\n"); //
  // Reset the CC3100
  sl_Stop(0);
  delay_ms(1000);
  sl_Start(0, 0, 0);
  //usb printing
 // printf("After Reset");printf("\r\n"); //

  // Wait for a connection
  //******************************************************************************** here 
  time = 0;
  
  while(!cc3100Bitset.test(CC3100BitSet::IsConnected))
  {
	//cc3k_int_poll();
    _SlNonOsMainLoopTask();
    if (time > WLAN_CONNECT_TIMEOUT) // default of 10s
    {
        //usb printing
        printf("Timed outZ");printf("\r\n"); //
	  //CHECK_PRINTER {
       CLI_Write("Timed out waiting to connect");
     // }
      return false;
    }
    time += 10;
    delay_ms(40);
  }
  
  
  delay_ms(1000);  
  if (cc3100Bitset.test(CC3100BitSet::IsIPAcquired))
  {
   //mdnsAdvertiser(1, (char *) _deviceName, strlen(_deviceName));
  }
  
  return true;
}

#endif

/**************************************************************************/
/*!
    Connect to an unsecured SSID/AP(security)

    @param  ssid      The named of the AP to connect to (max 32 chars)

    @returns  False if an error occured!
*/
/**************************************************************************/
bool Adafruit_CC3100::connectOpen(const char *ssid)
{
  if (!_initialised) {
    return false;
  }

  #ifndef CC3100_TINY_DRIVER
    CHECK_SUCCESS(sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 0), NULL, 0),
                 "Failed to set connection policy", false);
    delay_ms(500);
    SlSecParams_t SecParams = {0};
    SecParams.Type = SL_SEC_TYPE_OPEN;
    CHECK_SUCCESS(sl_WlanConnect((const int8_t*)ssid, pal_Strlen(ssid), NULL, (const SlSecParams_t*)&SecParams, NULL),
          "SSID connection failed", false);
  #endif

  return true;
}

//*****************************************************************************
//
//! CC3100_UsynchCallback
//!
//! @param  lEventType Event type
//! @param  data
//! @param  length
//!
//! @return none
//!
//! @brief  The function handles asynchronous events that come from CC3100
//!         device and operates a led for indicate
//
//*****************************************************************************
/*void CC3100_UsynchCallback(long lEventType, char * data, unsigned char length)
{
	
  if (lEventType == HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE)
  {
    cc3100Bitset.set(CC3100BitSet::IsSmartConfigFinished);
  }

  if (lEventType == HCI_EVNT_WLAN_UNSOL_CONNECT)
  {
    cc3100Bitset.set(CC3100BitSet::IsConnected);
  }

  if (lEventType == HCI_EVNT_WLAN_UNSOL_DISCONNECT)
  {
    cc3100Bitset.reset(CC3100BitSet::IsConnected | CC3100BitSet::HasDHCP);
  }
  
  if (lEventType == HCI_EVNT_WLAN_UNSOL_DHCP)
  {
    cc3100Bitset.set(CC3100BitSet::HasDHCP);
  }

  if (lEventType == HCI_EVENT_CC3100_CAN_SHUT_DOWN)
  {
    cc3100Bitset.set(CC3100BitSet::OkToShutDown);
  }

  if (lEventType == HCI_EVNT_WLAN_ASYNC_PING_REPORT)
  {
    //PRINT_F("CC3100: Ping report\n\r");
    pingReportnum++;
    memcpy(&pingReport, data, length);
  }

  if (lEventType == HCI_EVNT_BSD_TCP_CLOSE_WAIT) {
    uint8_t socketnum;
    socketnum = data[0];
    //PRINT_F("TCP Close wait #"); printDec(socketnum);
    if (socketnum < MAX_SOCKETS)
      closed_sockets[socketnum] = true;
  }
}*/

/**************************************************************************/
/*!
    Connect to an SSID/AP(security)

    @note     This command isn't available when the CC3100 is configured
              in 'CC3100_TINY_DRIVER' mode

    @returns  False if an error occured!
*/
/**************************************************************************/
#ifndef CC3100_TINY_DRIVER
bool Adafruit_CC3100::connectSecure(const char *ssid, const char *key, int32_t secMode)
{
  if (!_initialised) {
    return false;
  }
  
  if ( (secMode < 0) || (secMode > 3)) {
    //CHECK_PRINTER {
     CLI_Write("Security mode must be between 0 and 3");
    //}
    return false;
  }

  if (strlen(ssid) > MAXSSID) {
   // CHECK_PRINTER {
      CLI_Write("SSID length must be < ");
     CLI_Write(MAXSSID);
   // }
    return false;
  }

  if (strlen(key) > MAXLENGTHKEY) {
    //CHECK_PRINTER {
      CLI_Write("Key length must be < ");
      CLI_Write(MAXLENGTHKEY);
   // }
    return false;
  }

  CHECK_SUCCESS(sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(1, 0, 0, 0, 0), NULL, 0), 
                "Failed setting the connection policy",
                false);
  delay_ms(500);
  SlSecParams_t secParams = {0};
  secParams.Key = (int8_t*)key;
  secParams.KeyLen = strlen(key);
  secParams.Type = secMode;

  CHECK_SUCCESS(sl_WlanConnect((const int8_t*)ssid, pal_Strlen(ssid), NULL, (const SlSecParams_t*)&secParams, NULL),
                 "SSID connection failed", false);

  return true;
}
#endif

// Connect with timeout
bool Adafruit_CC3100::connectToAP(const char *ssid, const char *key, uint8_t secmode, uint8_t attempts) {
  if (!_initialised) {
    return false;
  }

  int16_t timer;
  char ssid1[15];

  // If attempts is zero interpret that as no limit on number of retries.
  bool retryForever = attempts == 0;

  do {
    // Stop if the max number of attempts have been tried.
    if (!retryForever) {
      if (attempts == 0) {
        return checkConnected();
      }
      attempts -= 1;
    }

    //cc3k_int_poll();
	_SlNonOsMainLoopTask();
    /* MEME: not sure why this is absolutely required but the cc3k freaks
       if you dont. maybe bootup delay? */
    // Setup a 4 second SSID scan
    scanSSIDs(4000);
    // Wait for results
    delay_ms(4500);
    scanSSIDs(0);
    
    /* Attempt to connect to an access point */
    //CHECK_PRINTER {
      CLI_Write("\n\rConnecting to "); 
	  sprintf(ssid1,"%s",ssid);
      CLI_Write(ssid1);
     CLI_Write("...");
   // }
    if ((secmode == 0) || (strlen(key) == 0)) {
      /* Connect to an unsecured network */
      if (! connectOpen(ssid)) {
       // CHECK_PRINTER {
         CLI_Write("Failed!");
       // }
        continue;
      }
    } else {
      /* NOTE: Secure connections are not available in 'Tiny' mode! */
#ifndef CC3100_TINY_DRIVER
      /* Connect to a secure network using WPA2, etc */
      if (! connectSecure(ssid, key, secmode)) {
       // CHECK_PRINTER {
         CLI_Write("Failed!");
       // }
        continue;
      }
#endif
    }
    
    timer = WLAN_CONNECT_TIMEOUT;

    /* Wait around a bit for the async connected signal to arrive or timeout */
   // CHECK_PRINTER {
     CLI_Write("Waiting to connect...");
  //  }
    while ((timer > 0) && !checkConnected())
    {
      //cc3k_int_poll();
	  _SlNonOsMainLoopTask();
      delay_ms(20);
      timer -= 10;
    }
    if (timer <= 0) {
    //  CHECK_PRINTER {
       CLI_Write("Timed out!");
    //  }
    }
  } while (!checkConnected());

  return true;
}


#ifndef CC3100_TINY_DRIVER
uint16_t Adafruit_CC3100::ping(uint32_t ip, uint8_t attempts, uint16_t timeout, uint8_t size) {
  if (!_initialised) return 0;
  if (!cc3100Bitset.test(CC3100BitSet::IsConnected)) return 0;
  if (!cc3100Bitset.test(CC3100BitSet::IsIPAcquired)) return 0;

  uint32_t revIP = (ip >> 24) | ((ip >> 8) & 0xFF00) | ((ip & 0xFF00) << 8) | (ip << 24);

  pingReportnum = 0;

  //if (CC3XPrinter != 0) {
  //  CC3XPrinter->print(F("Pinging ")); printIPdots(revIP); CC3XPrinter->print(" ");
  //  CC3XPrinter->print(attempts); CC3XPrinter->println(F(" times"));
  //}
  
  SlPingStartCommand_t *pingCommand;
  SlPingReport_t txPingReport;
  
  pingCommand->Ip = revIP;
  pingCommand->PingSize = size;                   // size of ping, in bytes
  pingCommand->PingIntervalTime = 100;           // delay between pings, in milliseconds
  pingCommand->PingRequestTimeout = timeout;        // timeout for every ping in milliseconds
  pingCommand->TotalNumberOfAttempts = attempts;       // max number of ping requests. 0 - forever
  pingCommand->Flags = 0;                        // report only when finished
  
  sl_NetAppPingStart((const SlPingStartCommand_t*)&pingCommand, AF_INET, &txPingReport, NULL);
  //gp
  //delay(timeout*attempts*2);
  count = timeout*attempts*2;
  
  while(count--){
	  delay_ms(1);
  }
  //if (CC3XPrinter != 0) CC3XPrinter->println(F("Req report"));
  //netapp_ping_report();
  //if (CC3XPrinter != 0) { CC3XPrinter->print(F("Reports: ")); CC3XPrinter->println(pingReportnum); }

  if (txPingReport.PacketsReceived) {
    /*
    if (CC3XPrinter != 0) {
      CC3XPrinter->print(F("Sent: ")); CC3XPrinter->println(pingReport.packets_sent);
      CC3XPrinter->print(F("Recv: ")); CC3XPrinter->println(pingReport.packets_received);
      CC3XPrinter->print(F("MinT: ")); CC3XPrinter->println(pingReport.min_round_time);
      CC3XPrinter->print(F("MaxT: ")); CC3XPrinter->println(pingReport.max_round_time);
      CC3XPrinter->print(F("AvgT: ")); CC3XPrinter->println(pingReport.avg_round_time);
    }
    //*/
    return txPingReport.PacketsReceived;
  } else {
    return 0;
  }
}

#endif

#ifndef CC3100_TINY_DRIVER
uint16_t Adafruit_CC3100::getHostByName(char *hostname, uint32_t *ip) {
  if (!_initialised) return 0;
  if (!cc3100Bitset.test(CC3100BitSet::IsConnected)) return 0;
  if (!cc3100Bitset.test(CC3100BitSet::IsIPAcquired)) return 0;

  int16_t r = gethostbyname((int8_t*)hostname, strlen((char*)hostname), (uint32_t*)ip, AF_INET);
  //if (CC3XPrinter != 0) { CC3XPrinter->print("Errno: "); CC3XPrinter->println(r); }
  return r;
}
#endif

/**************************************************************************/
/*!
    Checks if the device is connected or not

    @returns  True if connected
*/
/**************************************************************************/
bool Adafruit_CC3100::checkConnected(void)
{
  _SlNonOsMainLoopTask();
  return cc3100Bitset.test(CC3100BitSet::IsConnected);
}

/**************************************************************************/
/*!
    Checks if the DHCP process is complete or not

    @returns  True if DHCP process is complete (IP address assigned)
*/
/**************************************************************************/
bool Adafruit_CC3100::checkDHCP(void)
{
  // Ugly hack to fix UDP issues with the 1.13 firmware by calling
  // gethostbyname on localhost.  The output is completely ignored
  // but for some reason this call is necessary or else UDP won't 
  // work.  See this thread from TI for more details and the genesis
  // of the workaround: http://e2e.ti.com/support/wireless_connectivity/f/851/t/342177.aspx
  // Putting this in checkDHCP is a nice way to make it just work
  // for people without any need to add to their sketch.
  uint16_t tuCnt = 500;
  do 
  {
	_SlNonOsMainLoopTask();
	 tuCnt--;
	 delay_ms(1);
  } while ((!cc3100Bitset.test(CC3100BitSet ::IsIPAcquired)) & (tuCnt >= 1));
 
  if (cc3100Bitset.test(CC3100BitSet::IsIPAcquired)) {
    uint32_t output;
    gethostbyname((int8_t *)"localhost", 9, &output, AF_INET);
  }
 
  return cc3100Bitset.test(CC3100BitSet::IsIPAcquired);
}

/**************************************************************************/
/*!
    Checks if the smart config process is finished or not

    @returns  True if smart config is finished
*/
/**************************************************************************/
bool Adafruit_CC3100::checkSmartConfigFinished(void)
{
  return cc3100Bitset.test(CC3100BitSet::IsSmartConfigFinished);
}

#ifndef CC3100_TINY_DRIVER
/**************************************************************************/
/*!
    Gets the IP config settings (if connected)

    @returns  True if smart config is finished
*/
/**************************************************************************/
bool Adafruit_CC3100::getIPConfig(SlNetCfgIpV4Args_t ipConfig)
{
  uint8_t len = sizeof(SlNetCfgIpV4Args_t);
  uint8_t dhcpIsOn = 0;

  if (!_initialised)      return false;
  if (!cc3100Bitset.test(CC3100BitSet::IsConnected)) return false;
  if (!cc3100Bitset.test(CC3100BitSet::IsIPAcquired))      return false;
  
  sl_NetCfgGet(SL_IPV4_STA_P2P_CL_GET_INFO, &dhcpIsOn, &len, (uint8_t *)&ipConfig);
  return true;
}
#endif


/**************************************************************************/
/*!
    @brief  Quick socket test to pull contents from the web
*/
/**************************************************************************/
Adafruit_CC3100_Client Adafruit_CC3100::connectTCP(uint32_t destIP, uint16_t destPort)
{
  sockaddr      socketAddress;
  int32_t       tcp_socket;

  // Create the socket(s)
  //if (CC3XPrinter != 0) CC3XPrinter->print(F("Creating socket ... "));
  tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (-1 == tcp_socket)
  {
    //CHECK_PRINTER {
     CLI_Write("Failed to open socket");
  //  }
    return Adafruit_CC3100_Client();
  }
  //CC3XPrinter->print(F("DONE (socket ")); CC3XPrinter->print(tcp_socket); CC3XPrinter->println(F(")"));

  closed_sockets[tcp_socket] = false; // Clear any previous closed event

  // Try to open the socket
  memset(&socketAddress, 0x00, sizeof(socketAddress));
  socketAddress.sa_family = AF_INET;
  socketAddress.sa_data[0] = (destPort & 0xFF00) >> 8;  // Set the Port Number
  socketAddress.sa_data[1] = (destPort & 0x00FF);
  socketAddress.sa_data[2] = destIP >> 24;
  socketAddress.sa_data[3] = destIP >> 16;
  socketAddress.sa_data[4] = destIP >> 8;
  socketAddress.sa_data[5] = destIP;

 // CHECK_PRINTER {
   CLI_Write("\n\rConnect to ");
    printIPdotsRev(destIP);
//	char c;
//	sprintf(&c,"%d",destPort);
   CLI_Write(":");
   CLI_Write("80");
   CLI_Write("\r\n");
  //}

  //printHex((byte *)&socketAddress, sizeof(socketAddress));
  //if (CC3XPrinter != 0) CC3XPrinter->print(F("Connecting socket ... "));
  if (-1 == ::connect(tcp_socket, &socketAddress, sizeof(socketAddress)))
  {
   // CHECK_PRINTER {
     CLI_Write("Connection error");
    //}
    sl_Close(tcp_socket);
    return Adafruit_CC3100_Client();
  }
  //if (CC3XPrinter != 0) CC3XPrinter->println(F("DONE"));
  return Adafruit_CC3100_Client(tcp_socket);
}


Adafruit_CC3100_Client Adafruit_CC3100::connectUDP(uint32_t destIP, uint16_t destPort)
{
  sockaddr      socketAddress;
  int32_t       udp_socket;

  // Create the socket(s)
  // socket   = SOCK_STREAM, SOCK_DGRAM, or SOCK_RAW
  // protocol = IPPROTO_TCP, IPPROTO_UDP or IPPROTO_RAW
  //if (CC3XPrinter != 0) CC3XPrinter->print(F("Creating socket... "));
  udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (-1 == udp_socket)
  {
    //CHECK_PRINTER {
     CLI_Write("Failed to open socket");
   // }
    return Adafruit_CC3100_Client();
  }
  //if (CC3XPrinter != 0) { CC3XPrinter->print(F("DONE (socket ")); CC3XPrinter->print(udp_socket); CC3XPrinter->println(F(")")); }

  // Try to open the socket
  memset(&socketAddress, 0x00, sizeof(socketAddress));
  socketAddress.sa_family = AF_INET;
  socketAddress.sa_data[0] = (destPort & 0xFF00) >> 8;  // Set the Port Number
  socketAddress.sa_data[1] = (destPort & 0x00FF);
  socketAddress.sa_data[2] = destIP >> 24;
  socketAddress.sa_data[3] = destIP >> 16;
  socketAddress.sa_data[4] = destIP >> 8;
  socketAddress.sa_data[5] = destIP;

 // CHECK_PRINTER {
   CLI_Write("Connect to ");
    printIPdotsRev(destIP);
//	char c;
//	sprintf(&c,"%d",destPort);
//    CLI_Write(":");
//   CLI_Write(&c);
   CLI_Write("\r\n");
 // }
//
  //printHex((byte *)&socketAddress, sizeof(socketAddress));
  if (-1 == ::connect(udp_socket, &socketAddress, sizeof(socketAddress)))
  {
    //CHECK_PRINTER {
     CLI_Write("Connection error");
    //}
    sl_Close(udp_socket);
    return Adafruit_CC3100_Client();
  }

  return Adafruit_CC3100_Client(udp_socket);
}


/**********************************************************************/
Adafruit_CC3100_Client::Adafruit_CC3100_Client(void) {
  _socket = -1;
}

Adafruit_CC3100_Client::Adafruit_CC3100_Client(int32_t s) {
  _socket = s; 
  bufsiz = 0;
  _rx_buf_idx = 0;
}

Adafruit_CC3100_Client::Adafruit_CC3100_Client(const Adafruit_CC3100_Client& copy) {
  // Copy all the members to construct this client.
  _socket = copy._socket;
  bufsiz = copy.bufsiz;
  _rx_buf_idx = copy._rx_buf_idx;
  memcpy(_rx_buf, copy._rx_buf, RXBUFFERSIZE);
}

void Adafruit_CC3100_Client::operator=(const Adafruit_CC3100_Client& other) {
  // Copy all the members to assign a new value to this client.
  _socket = other._socket;
  bufsiz = other.bufsiz;
  _rx_buf_idx = other._rx_buf_idx;
  memcpy(_rx_buf, other._rx_buf, RXBUFFERSIZE);
}

Adafruit_CC3100_Client::operator bool()
{
  return connected();
}

int Adafruit_CC3100_Client::connect(const char *host, uint16_t port){
  
  // if (!_initialised) return 0;
  // if (!ulCC3100Connected) return 0;
  // if (!ulCC3100DHCP) return 0;

  uint32_t ip = 0;

  int16_t r = gethostbyname((int8_t *)host, (const uint16_t)strlen(host), &ip, AF_INET);

  if (ip!=0 && r!=0)
    return connect(ip, port);
  else 
    return 0;
}

int Adafruit_CC3100_Client::connect(uint32_t destIP, uint16_t destPort)
{

  bufsiz = 0;
  _rx_buf_idx = 0;
  sockaddr      socketAddress;
  int32_t       tcp_socket;

  // Create the socket(s)
  //if (CC3XPrinter != 0) CC3XPrinter->print(F("Creating socket ... "));
  tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (-1 == tcp_socket)
  {
   // CHECK_PRINTER {
     CLI_Write("Failed to open socket");
   // }
    return 0;
  }
  //CC3XPrinter->print(F("DONE (socket ")); CC3XPrinter->print(tcp_socket); CC3XPrinter->println(F(")"));

  closed_sockets[tcp_socket] = false; // Clear any previous closed event

  // Try to open the socket
  memset(&socketAddress, 0x00, sizeof(socketAddress));
  socketAddress.sa_family = AF_INET;
  socketAddress.sa_data[0] = (destPort & 0xFF00) >> 8;  // Set the Port Number
  socketAddress.sa_data[1] = (destPort & 0x00FF);
  socketAddress.sa_data[2] = destIP >> 24;
  socketAddress.sa_data[3] = destIP >> 16;
  socketAddress.sa_data[4] = destIP >> 8;
  socketAddress.sa_data[5] = destIP;

  //CHECK_PRINTER {
   CLI_Write("\n\rConnect to ");
   CLI_Write(&destIP);
 //  char c;
 //  sprintf(&c,"%d",destPort);
 //  CLI_Write(":");
 //  CLI_Write(&c);
   CLI_Write("\r\n");
  //}

  //printHex((byte *)&socketAddress, sizeof(socketAddress));
  //if (CC3XPrinter != 0) CC3XPrinter->print(F("Connecting socket ... "));
  if (-1 == ::connect(tcp_socket, &socketAddress, sizeof(socketAddress)))
  {
   // CHECK_PRINTER {
     CLI_Write("Connection error");
   // }
    ::sl_Close(tcp_socket);
    return 0;
  }
  // if (CC3XPrinter != 0) CC3XPrinter->println(F("DONE"));

  _socket = tcp_socket;
  return 1;
}

uint8_t Adafruit_CC3100_Client::connected(void) { 
  if (_socket < 0) return false;

  if (! available() && closed_sockets[_socket] == true) {
    //if (CC3XPrinter != 0) CC3XPrinter->println("No more data, and closed!");
    ::sl_Close(_socket);
    closed_sockets[_socket] = false;
    _socket = -1;
    return false;
  }

  else return true;  
}

size_t Adafruit_CC3100_Client::write(const void *buf, uint16_t len, uint32_t flags)
{
  return send(_socket, buf, len, flags);
}

size_t Adafruit_CC3100_Client::write(const uint8_t *buf, size_t len)
{
  return write(buf, len, 0);
}

size_t Adafruit_CC3100_Client::write(uint8_t c)
{
  int32_t r;
  r = send(_socket, &c, 1, 0);
  if ( r < 0 ) return 0;
  return r;
}

size_t Adafruit_CC3100_Client::fastrprint_n(const char *ifsh)
{
  char _tx_buf[TXBUFFERSIZE];
  uint8_t idx = 0;

  const char PROGMEM *p = (const char PROGMEM *)ifsh;
  size_t n = 0;
  while (1) {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) break;
    _tx_buf[idx] = c;
    idx++;
    if (idx >= TXBUFFERSIZE) {
      // lets send it!
      n += send(_socket, _tx_buf, TXBUFFERSIZE, 0);
      idx = 0;
    }
  }
  if (idx > 0) {
    // Send any remaining data in the transmit buffer.
    n += send(_socket, _tx_buf, idx, 0);
  }

  return n;
}

size_t Adafruit_CC3100_Client::fastrprintln_n(const char *ifsh)
{
  size_t r = 0;
  r = fastrprint(ifsh);
  r+= fastrprint_n("\r\n");
  return r;
}

size_t Adafruit_CC3100_Client::fastrprintln(const char *str)
{
  size_t r = 0;
  size_t len = strlen(str);
  if (len > 0) {
    if ((r = write(str, len, 0)) <= 0) return 0;
  }
  if ((r += write("\r\n", 2, 0)) <= 0) return 0;  // meme fix
  return r;
}

size_t Adafruit_CC3100_Client::fastrprint(const char *str)
{
  size_t len = strlen(str);
  if (len > 0) {
    return write(str, len, 0);
  }
  else {
    return 0;
  }
}

size_t Adafruit_CC3100_Client::fastrprint(char *str)
{
  size_t len = strlen(str);
  if (len > 0) {
    return write(str, len, 0);
  }
  else {
    return 0;
  }
}

size_t Adafruit_CC3100_Client::fastrprintln(char *str) {
  size_t r = 0;
  r = fastrprint(str);
  r+= fastrprint_n("\r\n");
  return r;
}

int Adafruit_CC3100_Client::read(void *buf, uint16_t len, uint32_t flags) 
{
  return recv(_socket, buf, len, flags);

}

int Adafruit_CC3100_Client::read(uint8_t *buf, size_t len) 
{
  return read(buf, len, 0);
}

int32_t Adafruit_CC3100_Client::close(void) {
  int32_t x = ::sl_Close(_socket);
  _socket = -1;
  return x;
}

void Adafruit_CC3100_Client::stop(){
  close();
}

int Adafruit_CC3100_Client::read(void) 
{
  while ((bufsiz <= 0) || (bufsiz == _rx_buf_idx)) {
    //cc3k_int_poll();
	_SlNonOsMainLoopTask();
    // buffer in some more data
    bufsiz = recv(_socket, _rx_buf, sizeof(_rx_buf), 0);
    if (bufsiz == -57) {
      close();
      return 0;
    }
    //if (CC3XPrinter != 0) { CC3XPrinter->println("Read "); CC3XPrinter->print(bufsiz); CC3XPrinter->println(" bytes"); }
    _rx_buf_idx = 0;
  }
  uint8_t ret = _rx_buf[_rx_buf_idx];
  _rx_buf_idx++;
  //if (CC3XPrinter != 0) { CC3XPrinter->print("("); CC3XPrinter->write(ret); CC3XPrinter->print(")"); }
  return ret;
}

int Adafruit_CC3100_Client::available(void) {
  // not open!
  if (_socket < 0) return 0;

  if ((bufsiz > 0) // we have some data in the internal buffer
      && (_rx_buf_idx < bufsiz)) {  // we havent already spit it all out
    return (bufsiz - _rx_buf_idx);
  }

  // do a select() call on this socket
  timeval timeout;
  fd_set fd_read;

  memset(&fd_read, 0, sizeof(fd_read));
  FD_SET(_socket, &fd_read);

  timeout.tv_sec = 0;
  timeout.tv_usec = 5000; // 5 millisec

  int16_t s = select(_socket+1, &fd_read, NULL, NULL, &timeout);
  //if (CC3XPrinter != 0) } CC3XPrinter->print(F("Select: ")); CC3XPrinter->println(s); }
  if (s >= 1) return 1;  // some data is available to read
  else return 0;  // no data is available
}

void Adafruit_CC3100_Client::flush(){
  // No flush implementation, unclear if necessary.
}

int Adafruit_CC3100_Client::peek(){
  while ((bufsiz <= 0) || (bufsiz == _rx_buf_idx)) {
    //cc3k_int_poll();
	_SlNonOsMainLoopTask();
    // buffer in some more data
    bufsiz = recv(_socket, _rx_buf, sizeof(_rx_buf), 0);
    if (bufsiz == -57) {
      close();
      return 0;
    }
    //if (CC3XPrinter != 0) { CC3XPrinter->println("Read "); CC3XPrinter->print(bufsiz); CC3XPrinter->println(" bytes"); }
    _rx_buf_idx = 0;
  }
  uint8_t ret = _rx_buf[_rx_buf_idx];

  //if (CC3XPrinter != 0) { CC3XPrinter->print("("); CC3XPrinter->write(ret); CC3XPrinter->print(")"); }
  return ret;
}

void Adafruit_CC3100::setPrinter() {
 // CC3XPrinter = p;
}

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pSlWlanEvent)
{
	if(pSlWlanEvent == NULL)
	CLI_Write(" [WLAN EVENT] NULL Pointer Error \n\r");
	
	switch(pSlWlanEvent->Event)
	{
		case SL_WLAN_CONNECT_EVENT:
		{
			cc3100Bitset.set(CC3100BitSet::IsConnected);
		}
		break;

		case SL_WLAN_DISCONNECT_EVENT:
		{
			slWlanConnectAsyncResponse_t*  pEventData = NULL;

			cc3100Bitset.reset(CC3100BitSet::IsConnected);
			cc3100Bitset.reset(CC3100BitSet::IsIPAcquired);

			pEventData = &pSlWlanEvent->EventData.STAandP2PModeDisconnected;

			if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
			{
				CLI_Write(" Device disconnected from the AP on application's request \n\r");
			}
			else
			{
				CLI_Write(" Device disconnected from the AP on an ERROR..!! \n\r");
				bConnectionError = true;
			}
		}
		break;
		
		case SL_WLAN_STA_CONNECTED_EVENT:
            break;

        case SL_WLAN_STA_DISCONNECTED_EVENT:
          
            break;
		
		case SL_WLAN_SMART_CONFIG_COMPLETE_EVENT:
		
		cc3100Bitset.set(CC3100BitSet::IsSmartConfigFinished);
		cc3100Bitset.set(CC3100BitSet::SmartConfigStoped);
		break;

		case SL_WLAN_SMART_CONFIG_STOP_EVENT:
		
		cc3100Bitset.set(CC3100BitSet::SmartConfigStoped);
		cc3100Bitset.reset(CC3100BitSet::IsSmartConfigFinished);

		default:
		{
			CLI_Write(" [WLAN EVENT] Unexpected event \n\r");
		}
		break;
	}
}

void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
	if(pNetAppEvent == NULL)
    {
       CLI_Write(" [NETAPP EVENT] NULL Pointer Error \n\r");
        return;
    }
 
    if(pNetAppEvent == NULL)
       CLI_Write(" [NETAPP EVENT] NULL Pointer Error \n\r");
 
    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
        {
			cc3100Bitset.set(CC3100BitSet::IsIPAcquired);
        }
        break;

        case SL_NETAPP_IP_LEASED_EVENT:
		
        break;

        default:
        {
           CLI_Write(" [NETAPP EVENT] Unexpected event \n\r");
        }
        break;
    }
}



void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pSlDeviceEvent)
{
	
	CLI_Write(" [GENERAL EVENT] \n\r");
}


void SimpleLinkSockEventHandler(SlSockEvent_t* pSock)
{
	if(pSock == NULL)
	CLI_Write(" [SOCK EVENT] NULL Pointer Error \n\r");
	
	switch( pSock->Event )
	{
		case SL_SOCKET_TX_FAILED_EVENT:
		{
			switch( pSock->socketAsyncEvent.SockTxFailData.status)
			{
				case SL_ECLOSE:
				//CLI_Write(" [SOCK EVENT] Close socket operation failed to transmit all queued packets\n\r");
				break;


				default:
				//CLI_Write(" [SOCK EVENT] Unexpected event \n\r");
				break;
			}
		}
		break;

		default:
		//CLI_Write(" [SOCK EVENT] Unexpected event \n\r");
		break;
	}
	
}
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pEvent,SlHttpServerResponse_t *pResponse)
{
	if(pEvent == NULL || pResponse == NULL)
	{
		CLI_Write(" [HTTP EVENT] NULL Pointer Error \n\r");
		return;
	}

	switch (pEvent->Event)
	{
		case SL_NETAPP_HTTPGETTOKENVALUE_EVENT:
		{
			_u8 *ptr;
			_u8 num = 0;

			if (0== pal_Memcmp (pEvent->EventData.httpTokenName.data,
			GET_STATUS,pEvent->EventData.httpTokenName.len))
			{
				if(cc3100Bitset.test(CC3100BitSet::IsConnectedToConfAp))
				{
					pal_Strcpy (pResponse->ResponseData.token_value.data,
					"TRUE");
					pResponse->ResponseData.token_value.len = pal_Strlen("TRUE");
				}
				else
				{
					pal_Strcpy (pResponse->ResponseData.token_value.data,
					"FALSE");
					pResponse->ResponseData.token_value.len = pal_Strlen("FALSE");
				}
			}
			else
			{
				ptr = pResponse->ResponseData.token_value.data;
				pResponse->ResponseData.token_value.len = 0;
				if(pal_Memcmp(pEvent->EventData.httpTokenName.data, GET_TOKEN,pal_Strlen(GET_TOKEN)) == 0)
				{
					num = pEvent->EventData.httpTokenName.data[8] - '0';
					num *= 10;
					num += pEvent->EventData.httpTokenName.data[9] - '0';

					if(g_ssid_list[num][0] == '\0')
					{
						pal_Strcpy(ptr, NO_AP);
					}
					else
					{
						pal_Strcpy(ptr, g_ssid_list[num]);
					}
					pResponse->ResponseData.token_value.len = pal_Strlen(ptr);
				}
			}
			break;
		}

		case SL_NETAPP_HTTPPOSTTOKENVALUE_EVENT:
		{

			if ((0 == pal_Memcmp(pEvent->EventData.httpPostData.token_name.data, "__SL_P_U.C", pEvent->EventData.httpPostData.token_name.len)) &&
			(0 == pal_Memcmp(pEvent->EventData.httpPostData.token_value.data, "Connect", pEvent->EventData.httpPostData.token_value.len)))
			{
				cc3100Bitset.set(CC3100BitSet::IsProfileAdded);
			}
			if (0 == pal_Memcmp (pEvent->EventData.httpPostData.token_name.data, "__SL_P_U.D", pEvent->EventData.httpPostData.token_name.len))
			{
				pal_Memcpy (g_Wlan_SSID,  pEvent->EventData.httpPostData.token_value.data, pEvent->EventData.httpPostData.token_value.len);
				g_Wlan_SSID[pEvent->EventData.httpPostData.token_value.len] = 0;
			}
			if (0 == pal_Memcmp (pEvent->EventData.httpPostData.token_name.data, "__SL_P_U.E", pEvent->EventData.httpPostData.token_name.len))
			{
				if (pEvent->EventData.httpPostData.token_value.data[0] == '0')
				{
					g_secParams.Type =  SL_SEC_TYPE_OPEN;

				}
				else if (pEvent->EventData.httpPostData.token_value.data[0] == '1')
				{
					g_secParams.Type =  SL_SEC_TYPE_WEP;

				}
				else if (pEvent->EventData.httpPostData.token_value.data[0] == '2')
				{
					g_secParams.Type =  SL_SEC_TYPE_WPA;
				}
				else if (pEvent->EventData.httpPostData.token_value.data[0] == '3')
				{
					g_secParams.Type =  SL_SEC_TYPE_WPA;
				}
			}
			if (0 == pal_Memcmp (pEvent->EventData.httpPostData.token_name.data, "__SL_P_U.F", pEvent->EventData.httpPostData.token_name.len))
			{
				pal_Memcpy (g_Wlan_Security_Key,pEvent->EventData.httpPostData.token_value.data, pEvent->EventData.httpPostData.token_value.len);
				g_Wlan_Security_Key[pEvent->EventData.httpPostData.token_value.len] = 0;
				g_secParams.Key = g_Wlan_Security_Key;
				g_secParams.KeyLen = pEvent->EventData.httpPostData.token_value.len;
			}
			if (0 == pal_Memcmp (pEvent->EventData.httpPostData.token_name.data, "__SL_P_U.0", pEvent->EventData.httpPostData.token_name.len))
			{
				cc3100Bitset.set(CC3100BitSet::IsApProvisioningDone);
			}
			break;
		}
		default:
		break;
	}
}