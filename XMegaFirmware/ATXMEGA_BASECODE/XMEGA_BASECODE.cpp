/**
 * 
 * XMEGA_BASECODE.cpp
 /*
 */

//Atmel ASF DOES NOT compile under C++ (as of Atmel Studio 6.2) Must use C compiler
extern "C" {
#include "src/asf.h"	
	};
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>

//cc3100 Adafruit for Arduino - modified by gp for atxmega
#include <Adafruit_CC3100.h>
#include "spi_wrapper.h" 
#include <string.h>

#include "cli_uart_wrapper.h" // Debug Only
#include "screens.h" // uses in screen loading
#include "ui_objects.h" // uses in touch test
#include "SRAM23K256.h" //uses for ram
#include "BaseCodeSub.h"

typedef char CHAR; 
//function prototype
//arduino compatibility
void setup();
void loop();
void wifi_test();
void setUp32MhzInternalOsc();
void setColor(int,int,int);
void clearStr(char*);
void addChar (char ch, char* str);
boolean matchTag (char* searchTag);
//gerald
char* appendCharToCharArray(char*, char);

//cc31000
uint16_t checkFirmwareVersion();
void displayMACAddress();
void listSSIDResults();

bool connect_accesspoint();
uint8_t getConfigModeFromNVM(uint8_t); //argument is NVM address location
boolean process_xml(int);
boolean process_restart();
int get_server_content();
 
//XMEGA SPECIFIC NVM
#define EEPROM_SSID_ADR			5
#define EEPROM_PWD_ADR			45
#define	EEPROM_CONFIGMODE_ADR	0
// config mode serial (3)

//cc3100
//Activating CC3100
#define SWITCH_POWER_CC3100		29
#define ADAFRUIT_CC3100_IRQ		24  // xmega gpio0, PD0
#define ADAFRUIT_CC3100_VBAT	25  //xmega pin25, gpio1
#define ADAFRUIT_CC3100_CS		20  //xmega SS pin port C
Adafruit_CC3100 cc3100			= Adafruit_CC3100(ADAFRUIT_CC3100_CS, ADAFRUIT_CC3100_IRQ, ADAFRUIT_CC3100_VBAT, SPI_CLOCK_DIVIDER);

// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY			SL_SEC_TYPE_WPA_WPA2
#define IDLE_TIMEOUT_MS			15000
#define DEVICE_NAME				"CC3100"
#define AP_ATTEMPTS				1 //how many attempts at connecting to access point
#define DHCP_TIMEOUT			60000 //in ms.

//OLED
#define __AVR_XMEGA__
#define OLED_POWER_12V			4 //pa4, for gracefully powering up the 12V on the Oled.
#define RESET_OLED				27
#define CS_OLED					28
#define A0_OLED					26

//LEDs DEFINITION:
#define LED_PA0_R				0
#define LED_PA1_B				1
#define LED_PA2_G				2

// Config Button
#define BUTTON_PIN				IOPORT_CREATE_PIN(PORTF,1) // SW1 on xplained board xmega256a3bu


// set device serial id YYMM, Software Version (4), id (8)
#define P9_SERIALID          "set serial" 
#define WEBSITE              "set url"

//global variables & flag
uint32_t       ip;
char           fail        = 'N';
char           format[3];
char           loop_speed[3];
int            iEnd        = 0;
const unsigned long dhcpTimeout     = 60L * 1000L; // Max time to wait for address from DHCP
long previousMillis = 0;        // will store last time LED was updated
unsigned int iConnectFail = 0;
unsigned int i;


//Required pointer global variables for passing router credentials between functions
char* ssid1;
char* pwd1;	  

#define YOUR_SSID "set ssid" // only when not using smart config or ap provisioning
#define PASSWORD "set password"

//#define YOUR_SSID "GalaxyS5"
//#define PASSWORD "123456789"

//for xml parsing
#define       MAX_STRING_LEN           258
char          tagStr[10]               = "";
char          dataStr[MAX_STRING_LEN]  = "";
char          tmpStr[MAX_STRING_LEN]   = "";
char          endTag[3]                = {'<', '/', '\0'};
unsigned int  len;

// Flags to differentiate XML tags from document elements (ie. data)
boolean tagFlag = false;
boolean dataFlag = false;


const char TextA[] PROGMEM = "Hello from #9\r\n";
const char TextB[] PROGMEM = "Please enter a configuration mode  ...\r\n";
const char TextC[] PROGMEM = "Not able to get IP address for poundnine.com\r\n";
const char TextD[] PROGMEM = "PoundNine.com IP: ";
const char TextE[] PROGMEM = "Reset Connection, >3 failed attempts\r\n";
const char TextF[] PROGMEM = "Connection failed\r\n";
const char TextG[] PROGMEM = "Format:";
const char TextH[] PROGMEM = "Speed: ";
const char TextI[] PROGMEM = "Header: ";
const char TextJ[] PROGMEM = "Line: ";
const char TextK[] PROGMEM = "Time Since: ";
const char TextL[] PROGMEM = "ClearBuffers\r\n";
const char TextM[] PROGMEM = "End XML Result\r\n";
const char TextO[] PROGMEM = "Unable to retrieve the firmware version!\r\n";
const char TextP[] PROGMEM = "Firmware V. :";
const char TextQ[] PROGMEM = "Unable to retrieve MAC Address!\r\n";
const char TextR[] PROGMEM = "MAC Address : not printed but there\r\n";
const char TextS[] PROGMEM = "SSID scan failed!\r\n";
const char TextT[] PROGMEM = "Networks found: ";
const char TextU[] PROGMEM = "================================================\r\n";
const char TextV[] PROGMEM = "SSID Name:";
const char TextW[] PROGMEM = "Connecting to Access Point:";
const char TextX[] PROGMEM = ".";
const char TextY[] PROGMEM = "Reconnecting with SmartConfig ...\r\n";
const char TextZ[] PROGMEM = "Initializing the CC3100 ...\r\n";
const char TextA1[] PROGMEM = "SmartConfig Reconnect Failed";
const char TextB1[] PROGMEM = "Reconnected ...\r\n";
const char TextC1[] PROGMEM = "Connecting with USBConfig ...\r\n";
const char TextD1[] PROGMEM = "SmartConfig failed, try again";
const char TextE1[] PROGMEM = "Unable to initialize the CC3100";
const char TextF1[] PROGMEM = "Deleting old connection profiles ...\r\n";
const char TextG1[] PROGMEM = "Failed deleting old profiles";
const char TextH1[] PROGMEM = "Please enter a configuration mode  ...\r\n";
const char TextI1[] PROGMEM = "Unable to retrieve the IP Address!\r\n";
const char TextJ1[] PROGMEM = "IP Addr: ";
const char TextK1[] PROGMEM = "Netmask: ";
const char TextL1[] PROGMEM = "Gateway: ";
const char TextM1[] PROGMEM = "DHCPsrv: ";
const char TextN1[] PROGMEM = "DNSserv: ";
const char TextO1[] PROGMEM = "Connect to AP failed";
const char TextP1[] PROGMEM = "Process result set";
const char TextQ1[] PROGMEM = "Connection Parameters Saved ...\r\n";
const char TextR1[] PROGMEM = "Requesting DHCP ...\r\n";
const char TextS1[] PROGMEM = "DHCP failed, try again\r\n";
const char TextT1[] PROGMEM = "Successfully connected to Access Point\r\n";
const char TextU1[] PROGMEM = "Internet Setup completed \r\n";

const char TextN[] PROGMEM = "\r\n";

// To Do bhautik,http request
const char Text1[] PROGMEM = "GET ";
const char Text2[] PROGMEM = "/wp-content/themes/jupiter/p9/datafeed_ajax.php?mode=xml"; //set url to get xml content
const char Text3[] PROGMEM = "&id=";
const char Text3A[] PROGMEM = "&x=";
const char Text4[] PROGMEM = " HTTP/1.1\r\n";
const char Text5[] PROGMEM = "Host: ";
const char Text6[] PROGMEM = "\r\n";



//spi SRAM microchips
#define MAXSPIRAM	11000 //max is 32000 for 256kbits chip
char wValue;
char rValue;
uint16_t address = 0; //up to 65536
uint16_t indexer = 0;

#define SS_PIN 05 //pin1 on xmega128a4u, PA5

byte clock = 0;
extern int8_t g_Wlan_SSID[];
extern int8_t g_Wlan_Security_Key[];
extern bool bConnectionError;

int		cycles = 0;
int		last_char = 0;
int		h; //total num chars in response

// Extern from BaseCodeSub.h
extern ScreenStateInfo_e gxScreenStatePrevious;
extern CredentialInfo_t gxCredentialInfo;
//extern ContentInfo_t gxContentInfo;
extern ScreenInfo_t gxScreenInfo;

// Other
boolean bRestartLoop = false;
static char FilterInURL[20] = "";
// Implemented In last
void vInitScreen(void);
void vInitCredentialProcess(void);
void vFlipScreen180(void);
struct ui_news news, news1;  // to hold processed xml for display
// Declared in screen.h,called from screen.c
void go_home(void)
{
	
}
void vFrom_Screen_FilterSetting(void)
{
	gxScreenInfo.bFilterScreenEnabled = true;
}
void vFilterProcess(char* filterName)
{
	unregister_all_touch_objects();
	gxScreenInfo.bLoadingContentProgressBAR = true;
	bRestartLoop = true;
	gxScreenInfo.bFilterScreenEnabled = false;
	
	if(strcmp("Settings",filterName) == 0) // for this filter stops filter displaying process
	{
		gxScreenInfo.xScreenState = SCREEN_STATE_SETTINGS;
		vUpdateScreen();
		gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_UNKNOWN;
		gxCredentialInfo.bCredentialProcessDone = false;
		gxCredentialInfo.bSettingCredentialButtonPressed = true;
		CLI_Write("\r\nSelected Filter Is :- Settings ");
	}
	else
	{
		clearStr(FilterInURL);
		strcpy(FilterInURL,filterName);
		CLI_Write("\r\nSelected Filter Is :- ");
		CLI_Write(FilterInURL);
	}
}
void set_email_status(int email_disp_status)
{
	
}
void reset_idle_timer(void)
{
	
}
void click_on_credential_type(uint8_t tuReceivedCredentialProcessType)
{
	gxCredentialInfo.bSettingCredentialButtonPressed = true;
	gxCredentialInfo.bCredentialProcessDone = false;
	switch(tuReceivedCredentialProcessType)
	{
		case CREDENTIAL_PROCESS_SMARTCONFIG:
			gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_SMARTCONFIG;
			break;
			
		case CREDENTIAL_PROCESS_PROVISIONING_AP:
			gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_PROVISIONING_AP;
			break;
			
		default:
			break;
	}
}



void setup(void) {
	
  	 sysclk_init(); //Init clock compatible with USB
	 //setUp32MhzInternalOsc(); //USB will not work if enabled because of timing issues.
	 vInitClkBoard(); //board_init(); // TO Do Bhautik,Right now Initilize Device(cc3100) also with it
	 delay_init(sysclk_get_cpu_hz());
	 vInitScreen();
	 CLI_Configure(); // To Do Bhautik,Debug Purpose Only
	 // Initialize interrupt vector table support.
	irq_initialize_vectors();

	// Enable interrupts
	cpu_irq_enable();

	/* Call a local utility routine to initialize C-Library Standard I/O over
	 * a USB CDC protocol. Tunable parameters in a conf_usb.h file must be
	 * supplied to configure the USB device correctly.
	 */

	//Initialize delay within system clocking for accurate timing
	
	//Initialize real time clock timing for milliseconds timing results. from internal 32khz clocking
	rtc_init();
	
	InitSRAM();	
	
	ioport_configure_pin(SS_PIN, IOPORT_DIR_OUTPUT); // to do Bhautik,now we are using asf module to configure pins
		
    ioport_configure_pin(OLED_POWER_12V, IOPORT_DIR_OUTPUT | IOPORT_PIN_LEVEL_LOW); // to do Bhautik,now we are using asf module to configure pins

   	ioport_set_pin_low(RESET_OLED);    // turn the OLED off by making the voltage LOW//to do Bhautik,now we are using asf module to configure pins
	delay_ms(100);               //
	ioport_set_pin_high(RESET_OLED);   // turn the OLED on (HIGH is the voltage level)//to do Bhautik,now we are using asf module to configure pins
	  
	//wifi start
	ioport_configure_pin(SWITCH_POWER_CC3100, IOPORT_DIR_OUTPUT);
	ioport_pin_is_low(SWITCH_POWER_CC3100);
	delay_ms(1000); //LET THE CC3100 POWER UP
	
	setColor(0, 0, 0);  // white
	strcpy(FilterInURL,"Latest"); // Init Filter Text
	vInitCredentialProcess();
}
	

void vGetPoundNineIP(void)
{
	
	// get website.com IP address
	while (ip == 0) {
		setColor(255,0,255); //green
		//	delay(100);
		if (!cc3100.getHostByName(WEBSITE, &ip)) {
			setColor(0, 255, 255);  // red
		}
		setColor(0, 0, 0);  // white
	}
	
	cc3100.printIPdotsRev(ip);
	
	  
	
		 
}


void loop(void) {
	
	uint32_t currentMillis = rtc_get_time();
	char buffer [40];
	
	// either first load or reached end of xml response, time to load from server
	if ((h == 0) || (last_char >= MAXSPIRAM) || (bRestartLoop == true) ) {
		
		last_char = 0;
		
		//Print number of time though loop
		cycles++;
		CLI_Write("\r\n");
		sprintf(buffer,"Millis # %ld\r\n",currentMillis);
		CLI_Write("\r\n");
		CLI_Write(buffer);
		sprintf(buffer,"Iteration # %d\r\n",cycles);
		CLI_Write("\r\n");
		CLI_Write(buffer);
		CLI_Write("----------------------------------------------------------------\r\n");
		
		previousMillis = currentMillis;
		setColor(0, 0, 255);  // yellow
				
		// 3 connection failures, reset and try again
		if (iConnectFail > 3) {
				if (process_restart()) {
					CLI_Write("restart sucess");
					setColor(255, 0, 255);  // green
					} else {
					CLI_Write("restart fail");
					setColor(0, 255, 255);  // red
					while(1);
				}
		} 
		
		h = get_server_content();
		// need to figure out how to pass failure of get_server_content
		
		//Clean up SPI SRam memory by filling with 0 - for debug but may not be necessary for production
		for(indexer=0;indexer<MAXSPIRAM;indexer++){
			//SpiRam.write_byte(indexer,0);
		}
		bRestartLoop = false;
		gxScreenInfo.bDoNOtWaitGoForDisplayingAfterEveryIteration = true;
	}
	else
	{
		// print updates every 8 seconds
		if ((gxScreenInfo.iContentScreenDisplayTime >= DISPLAY_CONTENT_TIMEOUT) || (gxScreenInfo.bDoNOtWaitGoForDisplayingAfterEveryIteration == true))
		{
			gxScreenInfo.bDoNOtWaitGoForDisplayingAfterEveryIteration = false;
			if(gxScreenInfo.bLoadingContentProgressBAR == true)
			{
				vScreenLoadingBar(8);
				vScreenLoadingBar(9);
				gxScreenInfo.bLoadingContentProgressBAR = false;
			}
			if(process_xml(h))
			{
				register_filter_callback();
				CLI_Write("------\r\n");
				gxScreenInfo.iContentScreenDisplayTime = 0;
			}
		} 
		else
		{
			if(gxScreenInfo.bFilterScreenEnabled == false)
			{
				_delay_ms(1);
				gxScreenInfo.iContentScreenDisplayTime++;
			}
		}
	
	}
			
}  // end loop


/**************************************************************************/
/*!
    @brief  Tries to read the CC3100's internal firmware patch ID
*/
/**************************************************************************/
uint16_t checkFirmwareVersion(void) {
  
  uint8_t major, minor;
  uint16_t version;
  
  if(!cc3100.getFirmwareVersion(&major, &minor))
  {
    printf_P(TextO);
	//printf("Unable to retrieve the firmware version!\r\n");
    version = 0;
  }
  else
  {
	printf_P(TextP);
    //printf("Firmware V. : ");
    printf("%d",major); 
	printf_P(TextX);
	//printf("."); 
	printf("%d",minor);
	printf_P(TextN);
    version = major; version <<= 8; version |= minor;
  }

  return version;
}

/**************************************************************************/
/*!
    @brief  Tries to read the 6-byte MAC address of the CC3100 module
*/
/**************************************************************************/
void displayMACAddress(void) {
  
  uint8_t macAddress[6];
  
  if(!cc3100.getMacAddress(macAddress))
  {
    printf_P(TextQ);
	//printf("Unable to retrieve MAC Address!\r\n");
  }
  else
  {
	printf_P(TextR);
    //printf("MAC Address : not printed but there\r\n");
    cc3100.printHex((byte*)&macAddress, 6);
  }
}



/**************************************************************************/
/*!
    @brief  Begins an SSID scan and prints out all the visible networks
*/
/**************************************************************************/

void listSSIDResults(void) {
 
  uint32_t index;
  uint8_t valid, rssi, sec,tuNumOfSSIDCnt;
  Sl_WlanNetworkEntry_t ssidname[20];
  char ptcSSIDBuff[50];

  if (!cc3100.startSSIDscan(&index)) {
	  printf_P(TextS);
	  //printf("SSID scan failed!\r\n");
	  return;
  }

  printf_P(TextT);
  //printf("Networks found: ");
  printf("%d",index);
  printf_P(TextN);
  //printf("\r\n");
  printf_P(TextU);
  //printf("================================================\r\n");
CLI_Write("SSID Scan FIKnished");
sprintf(ptcSSIDBuff,"Total SSID :- %d\r\n",index);
CLI_Write(ptcSSIDBuff);
CLI_Write("SSID Scanning finished\r\n");
cc3100.getNextSSIDList(0,index,ssidname);
CLI_Write("Here is a List :- ");

  //valid = cc3100.getNextSSIDList(rssi, sec,(Sl_WlanNetworkEntry_t*) ssidname); // TO DO Bhautik,update this function
  for(tuNumOfSSIDCnt = 0;tuNumOfSSIDCnt < index;tuNumOfSSIDCnt++)
  {
	  printf_P(TextV);
	  //printf("SSID Name: ");
	  printf("%s",ssidname[tuNumOfSSIDCnt].ssid);
	  printf_P(TextN);
	  CLI_Write(TextN);
	  //printf("\r\n");
	  CLI_Write("     ");
	  CLI_Write("(");
	  CLI_Write(tuNumOfSSIDCnt+1);
	  CLI_Write("). ");
	  sprintf(ptcSSIDBuff,"%s\r\n",ssidname[tuNumOfSSIDCnt].ssid);
	  CLI_Write(ptcSSIDBuff);
  }
  
  
  printf_P(TextU);
  //printf("================================================\r\n");
  cc3100.stopSSIDscan();

}



/**************************************************************************/
//    @brief  Set led color
/**************************************************************************/
void setColor(int red, int green, int blue) {

	//analogWrite(LED_PA0_R, red);
	//analogWrite(LED_PA2_G, green);
	//analogWrite(LED_PA1_B, blue);
	
}

void setUp32MhzInternalOsc() {
	
	OSC_CTRL |= OSC_RC32MEN_bm; //Setup 32Mhz crystal
	
	while(!(OSC_STATUS & OSC_RC32MRDY_bm));
	
	CCP = CCP_IOREG_gc; //Trigger protection mechanism
	CLK_CTRL = CLK_SCLKSEL_RC32M_gc; //Enable internal  32Mhz crystal
	
	
}


/**************************************************************************/
//    @brief  Function to clear a string
/**************************************************************************/
void clearStr (char* str) {
	
	int len = strlen(str);
	for (int c = 0; c < len; c++) {
		str[c] = 0;
	}
	
}

char* appendCharToCharArray(char* array, char a)
{
	size_t len = strlen(array);

	char *ret =(char*) malloc(len+2);

	strcpy(ret, array);
	ret[len] = a;
	ret[len+1] = '\0';

	return ret;
}





/**************************************************************************/
/*!
    @brief  Connect using save credentials
  */
  /**************************************************************************/
  bool configSavedCredentials(void) {
	  
	  //use stored credentials, when button is not pressed
	  // check for last config mode used

	  //char           ssid[30];
	  //char           pwd[30];
	  /* ======================== 2 ========================== */	  
	  if(gxCredentialInfo.uCredentialInfo == CREDENTIAL_PROCESS_SMARTCONFIG){
		  
			  
		  setColor(255, 0, 0);  // aqua
		  
		  if (!cc3100.begin(false, true, DEVICE_NAME))
		  {
			  setColor(0, 255, 255);  // red
			 nvm_eeprom_write_byte(EEPROM_CONFIGMODE_ADR, 0);
			 if(gxCredentialInfo.bIsCredentialSaved != true)
			 {
				 vProgressBAR(4,"Error ,Please Restart Device");
				 unregister_all_touch_objects();
			 }
			  return false;
			  //configSavedCredentials(); //try again
		  }
		
		  delay_ms(1500);
		  setColor(0, 0, 255);  // yellow
		  
		  
	  }
	  

	  /* ======================== 3 ========================== */
	  if (gxCredentialInfo.uCredentialInfo == CREDENTIAL_PROCESS_PROVISIONING_AP)
	  {
		  
		 if (!cc3100.begin(false, true, DEVICE_NAME))
		 {
			 nvm_eeprom_write_byte(EEPROM_CONFIGMODE_ADR, 0);
			 if(gxCredentialInfo.bIsCredentialSaved != true)
			 {
				 vProgressBAR(4,"Error ,Please Restart Device");
				 unregister_all_touch_objects();
			 }
			 return false;
		 }
		 delay_ms(1500);
		 return true;
		  
	  }

	  
  }
  

/**************************************************************************/
/*!
    @brief  Connect using TI Smart Config App
*/
/**************************************************************************/
bool configSmartConfig(void)
{
   
     if (!cc3100.begin(false))
      {
         setColor(0, 255, 255);  // red
		 unregister_all_touch_objects();
		 vProgressBAR(4,"Error ,Please Restart Device");
         return false;
		 //while(1);
      }
  
      displayMACAddress();
   
	    if (!cc3100.deleteProfiles()) {
                  // clearStr(row4buffer);
                  // snprintf (row4buffer,42, "%s%s", row4buffer, "Failed ..."); // to do bhautik,need to make snprintf
					//while(1);
					unregister_all_touch_objects();
					vProgressBAR(4,"Error ,Please Restart Device");
					return false;
      }
 
      vProgressBAR(1,"Click Start On Phone");
      if (!cc3100.startSmartConfig(DEVICE_NAME))
      {
 	   //printf_P(TextD1);
  	   setColor(0, 255, 255);  // red
 		 vProgressBAR(4,"Error ,Please Restart Device");
		 unregister_all_touch_objects();
		 return false;
      }
      setColor(0, 0, 255);  // yellow
 	  nvm_eeprom_write_byte(EEPROM_CONFIGMODE_ADR,CREDENTIAL_PROCESS_SMARTCONFIG);
      delay_ms(1000);
	  return true;
         
}

/**************************************************************************/
/*!
    @brief  Connect using input from USB serial terminal
*/
/**************************************************************************/
bool configProvisioningAP(void)
{
   int8_t index = 0;
   if (!cc3100.begin(false))
   {
	   setColor(0, 255, 255);  // red
	  vProgressBAR(4,"Error ,Please Restart Device");
	  unregister_all_touch_objects();
	  return false;
   }
   
   displayMACAddress();
   
   if (!cc3100.startProvisioningAP())
   {
	   CLI_Write("Provisioning AP Failed");
	   setColor(0, 255, 255);  // red
	   vProgressBAR(4,"Error ,Please Restart Device");
	   unregister_all_touch_objects();
	   return false;
   }
   else
   {
	   CLI_Write("Provisioning AP done");
   }
	
	// config mode provisioningAP(3)
   nvm_eeprom_write_byte(EEPROM_CONFIGMODE_ADR, CREDENTIAL_PROCESS_PROVISIONING_AP);
   
   //credentials saved - done.
   if(gxCredentialInfo.bIsCredentialSaved != true)
   {
	   vProgressBAR(1,"Credential Received");
	   vProgressBAR(2,"connecting to access point");
   }
   
   return true;    
  }

/**************************************************************************/
//    @brief  get configmode from NVM
/**************************************************************************/
uint8_t getConfigModeFromNVM(uint8_t NVMaddress){

return nvm_eeprom_read_byte(NVMaddress);
	
}
  
/**************************************************************************/
//    @brief  Get IP addres, DNS, Gateway, etc
/**************************************************************************/
bool request_dhcp() {
	
	uint32_t t;

	printf_P(TextN);
	printf_P(TextR1);
	delay_ms(1500);
	
	// Wait for DHCP to complete 
	t=rtc_get_time(); //get time before trying the dhcp procedure
	while (!cc3100.checkDHCP())
	{
		delay_ms(100); 
		if ((rtc_get_time() - t) > DHCP_TIMEOUT) {
			delay_ms(1500);
			printf_P(TextS1); //DHCP failed, try again
			//while(1);
			vProgressBAR(4,"Error ,Please Restart Device");
			unregister_all_touch_objects();
			return false;
			
		}
	}
	
	setColor(255, 0, 255);  // green
	
}

/**************************************************************************/
//    @brief  Tries to read the IP address and other connection details
/**************************************************************************/
bool displayConnectionDetails(void) {
	
	uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
	
	if(!cc3100.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
	{
		printf_P(TextI1);
		//printf("Unable to retrieve the IP Address!\r\n");
		setColor(0, 255, 255);  // red
		vProgressBAR(4,"Error ,Please Restart Device");
		unregister_all_touch_objects();
		return false;
	}
	else
	{
		char row4buffer[40];
		printf_P(TextU1); //success connecting to internet.
				
		uint8_t ip1 =  (ipAddress >> 24) ;
		uint8_t ip2 =  (ipAddress >> 16) ;
		uint8_t ip3 =  (ipAddress >> 8) ;
		uint8_t ip4 =  (ipAddress) ;
		
		clearStr(row4buffer);
		sprintf (row4buffer, "%s%s", row4buffer, "IP Address: ");
		sprintf (row4buffer, "%s%i", row4buffer, ip1);
		sprintf (row4buffer, "%s%s", row4buffer, ".");
		sprintf (row4buffer, "%s%i", row4buffer, ip2);
		sprintf (row4buffer, "%s%s", row4buffer, ".");
		sprintf (row4buffer, "%s%i", row4buffer, ip3);
		sprintf (row4buffer, "%s%s", row4buffer, ".");
		sprintf (row4buffer, "%s%i", row4buffer, ip4);
		
		delay_ms(1500);
		if(gxCredentialInfo.bIsCredentialSaved != true)
		{
			vProgressBAR(3,row4buffer);
			vProgressBAR(4,"Connected");
			unregister_all_touch_objects();
		}
		
		return true;
	}
}

/**************************************************************************/
//    @brief  Connect to AP - ONLY FOR SERIAL CONFIG
/**************************************************************************/
bool connect_accesspoint() {

	//Initialize the cc3100    	  
			  printf_P(TextZ);
	    	  if (!cc3100.begin(false))
	    	  {
		    	  	//clearStr(row4buffer);
		    	  	//sprintf (row4buffer, "%s%s", row4buffer, "Failed initializing CC3100 ...");
		    	  	//strcat(row4buffer, ssid1);
		    	  setColor(0, 255, 255);  // red
		    	 // while(1);
				 return false;
	    	  }

	
	uint32_t t;
	/* Attempt to connect to an access point */
	printf_P(TextW);
	//printf("Connecting to Access Point: "); 
	printf("%s\n",ssid1);
	printf_P(TextN);


	if(!cc3100.connectToAP(YOUR_SSID, PASSWORD, WLAN_SECURITY,AP_ATTEMPTS)) {
		
		 //printf("Connect to AP failed");
		 printf_P(TextO1);
		 
		 //clearStr(row4buffer);
		// sprintf (row4buffer, "%s%s", row4buffer, "Connecting to Access Point Failed");
		vScreen_SettingButton();
		// unregister_all_touch_objects();
		 return false;
		 
		 };

	printf_P(TextT1);
	SlSecParams_t secParams = {0};
	secParams.Key = (int8_t*)PASSWORD;
	secParams.KeyLen = strlen(PASSWORD);
	secParams.Type = SL_SEC_TYPE_WPA_WPA2;
	delay_ms(500);
	sl_WlanProfileAdd((const _i8*)YOUR_SSID,strlen(YOUR_SSID),0,&secParams,0,1,0);
	delay_ms(1500);
	return true;
	 
}

/**************************************************************************/
//Function to add a char to a string and check its length
/**************************************************************************/
void addChar (char ch, char* str) {
	
	char *tagMsg  = "<TRUNCATED_TAG>";
	char *dataMsg = "-TRUNCATED_DATA-";

	// Check the max size of the string to make sure it doesn't grow too
	// big.  If string is beyond MAX_STRING_LEN assume it is unimportant
	// and replace it with a warning message.
	if (strlen(str) > MAX_STRING_LEN - 2) {
		if (tagFlag) {
			clearStr(tagStr);
			strcpy(tagStr,tagMsg);
		}
		if (dataFlag) {
			clearStr(dataStr);
			strcpy(dataStr,dataMsg);
		}

		// Clear the temp buffer and flags to stop current processing
		clearStr(tmpStr);
		tagFlag = false;
		dataFlag = false;

		} else {
		// Add char to string
		//bw skip line feed
		if (ch != '\n') {
			str[strlen(str)] = ch;
		}
	}
}

/**************************************************************************/
// Function to check the current tag for a specific string
/**************************************************************************/
boolean matchTag (char* searchTag) {
	
	if ( strcmp(tagStr, searchTag) == 0 ) {
		return true;
		} else {
		return false;
	}
	
}


/**************************************************************************/
// Function to restart cc3100 /
/**************************************************************************/
boolean process_restart () {
	
	printf_P(TextE);
	CLI_Write("Reset Connection, >3 failed attempts\n");
	//clearStr(row0buffer);clearStr(row1buffer);clearStr(row2buffer);clearStr(row3buffer);clearStr(row4buffer);
	iConnectFail = 0;
	boolean disconnected = cc3100.disconnect();
	cc3100.stop();
	delay_ms(200);
	
	//Shutdown CC3100 - For at least 5 seconds to discharge power line Capacitor
	//wifi cc3100 powering control
	ioport_configure_pin(SWITCH_POWER_CC3100, IOPORT_DIR_OUTPUT);//to do Bhautik,now we are using asf module to configure pins
	ioport_set_pin_high(SWITCH_POWER_CC3100); //to do Bhautik,now we are using asf module to configure pins
	delay_ms(5000); //LET THE CC3100 cool down
	ioport_set_pin_low(SWITCH_POWER_CC3100); //to do Bhautik,now we are using asf module to configure pins
	delay_ms(1000); //LET THE CC3100 power up
	
	// reset everything in CC3100
	cc3100 = Adafruit_CC3100(ADAFRUIT_CC3100_CS, ADAFRUIT_CC3100_IRQ, ADAFRUIT_CC3100_VBAT,
	SPI_CLOCK_DIVIDER);
	configSavedCredentials();
	
	// get poundnine.com IP address, if needed
	while (ip == 0) {
		setColor(255,0,255); //green
		delay_ms(100);
		if (!cc3100.getHostByName(WEBSITE, &ip)) {
			printf_P("Not able to get IP address for poundnine.com");
			setColor(0, 255, 255);  // red
			//clearStr(row0buffer);clearStr(row1buffer);clearStr(row2buffer);clearStr(row3buffer);clearStr(row4buffer);
			//sprintf (row4buffer, "%s%s", row4buffer, "PoundNine.com Connection Error");
			return false;
		}
		setColor(0, 0, 0);  // white
		delay_ms(100);
	}
	
	//printf_P(TextD);
	//printf("PoundNine.com IP: ");
	cc3100.printIPdotsRev(ip);
	printf_P(TextN);
	//printf("\n");
	
	return true;
	
}


/**************************************************************************/
// Function to retrieve xml content from server
/**************************************************************************/
int get_server_content () {
	
	char curChar;
	char prevChar;
	char prevChar1;
	char prevChar2;
	int h;
	bool endhead; // end header
	char buffer [24];
	uint8_t reconnectingCnt = 0;
	CLI_Write("get server content \r\n");
	_SlNonOsMainLoopTask();
	if(bConnectionError == true)
	{
		sl_Stop(500);
		delay_ms(500);
		sl_Start(0,0,0);
		screen_displayingReconnecting(0);
		while(1)
		{
			if(cc3100.checkDHCP())
			{
				break;
			}
			reconnectingCnt++;
			
			if(reconnectingCnt%2)
			{
				screen_displayingReconnecting(1);
			}
			else
			{
				screen_displayingReconnecting(2);
			}
			
		}
		delay_ms(500);
		bConnectionError = false;
	}

	Adafruit_CC3100_Client www;
	uint32_t lastRead = rtc_get_time();
	if(gxScreenInfo.bLoadingContentProgressBAR == true)
	{
		vScreenLoadingBar(1);	
	}
	while ((rtc_get_time() - lastRead) < IDLE_TIMEOUT_MS) {
		www = cc3100.connectTCP(ip, 80);
		if (www.connected()) {
			setColor(0, 0, 0);  // white
			break;
			} else {
			setColor(255, 255, 255);  // off
			delay_ms(50);
			setColor(0, 255, 255);  // red
			delay_ms(50);
		}
	}
	_SlNonOsMainLoopTask();
	if(bConnectionError == true)
	{
		return 0;
	}
	if(gxScreenInfo.bLoadingContentProgressBAR == true)
	{
		vScreenLoadingBar(2);
	}
	if (www.connected()) {
		
		//iConnectFail = 0;
		//clearStr(row0buffer);clearStr(row1buffer);clearStr(row2buffer);clearStr(row3buffer);clearStr(row4buffer);
		//sprintf (row4buffer, "%s%s", row4buffer, "Loading latest wire data");
		sprintf(buffer,"%d",cycles);
		
		while ((rtc_get_time() - lastRead) < IDLE_TIMEOUT_MS) { // getting stuck hear, tryingthis timeout 2014-02-11
			
			//printf("fastrprint start\n");
			
			www.fastrprint_n(Text1);
			www.fastrprint_n(Text2);
			//www.fastrprint(FilterInURL);
			www.fastrprint_n(Text3);
			www.fastrprint(P9_SERIALID);
			www.fastrprint_n(Text3A);
			www.fastrprint(buffer);
			//www.fastrprint_n(Text2A); //sampledata
			www.fastrprint_n(Text4);
			www.fastrprint_n(Text5);
			www.fastrprint(WEBSITE);
			www.fastrprint_n(Text6);
			www.fastrprint_n(Text6);
			www.fastrprint_n(Text6);
			
			
			delay_ms(100);
			
			//printf("fastrprint end\n");
			break; //made it to end
		}
		
		i++;
	} else
	{
		printf_P(TextF);
		iConnectFail++;
		setColor(0, 255, 255);  // red
		
		//  need to figure out what to do on repeated connection fails 
		fail = 'Y';
		//return;
		
	} // end if www.connected
	if(gxScreenInfo.bLoadingContentProgressBAR == true)
	{
		vScreenLoadingBar(3);
	}
	/* Read data until either the connection is closed, or the idle timeout is reached. */
	lastRead = rtc_get_time();
	//char c;
	h=0;
	endhead = false;
	if(gxScreenInfo.bLoadingContentProgressBAR == true)
	{
		vScreenLoadingBar(4);
		vScreenLoadingBar(5);
	}
	_SlNonOsMainLoopTask();
	if(bConnectionError == true)
	{
		return 0;
	}
	delay_ms(500);
	//CLI_Write("Received Response Is :- "); // To DO Bhautik,Debug Only
	while (www.connected() && (rtc_get_time() - lastRead < IDLE_TIMEOUT_MS)) {
		
		while (www.available() ) {
			
			setColor(255,0,255); //green
			prevChar2 = prevChar1;
			prevChar1 = prevChar;
			prevChar = curChar;
			curChar = www.read();
			//CLI_Write(&curChar); // To Do Bhautik,Debug Only
			//start copying content after you get to end of http header: utf-8
			if ((curChar == '8') && (prevChar == '-') && (prevChar1 == 'f') && (prevChar2 == 't')) {
				endhead = true;
			}
			if ( (endhead == true)  && (h < MAXSPIRAM)) {
				
				Sram_Write_Byte(h,curChar); // to do bhautik,store in ram
				h++;
				
			}
			if ((curChar == '>') && (prevChar == 'M') && (prevChar1 == '/') && (prevChar2 == '<')) {
				
				www.stop();
			}
			//delay_ms(10);
			
		} // end while www.available
		
		
		
	} // end while connected
	
	// printf("Connection to be closed\n");
	
	www.close();
	if(gxScreenInfo.bLoadingContentProgressBAR == true)
	{
		vScreenLoadingBar(6);
		vScreenLoadingBar(7);
	}
	sprintf(buffer,"h: %d\r\n",h);
	CLI_Write(buffer);
	
	// check to see if full content was received, if not, then increment error
	int b;	
	if (h <= 20) {
		for (b=1; b < h; b++) {
			char inChar = Sram_Read_Byte(b);//= SpiRam.read_byte(b);
			CLI_Write(&inChar);
			
		}
		
		iConnectFail++;
		CLI_Write("\r\nno valid content \r\n");
		bRestartLoop = true;
	}
	else
	{
		
		iConnectFail = 0;
		
	}
	
	
		
	return h;
}

/**************************************************************************/
// Function to process xml recieved from server
/**************************************************************************/
boolean process_xml (int h) {
	
	int b;
	int num_to_display	= 1;
	int num_diplayed	= 1;
	char buffer1 [20];
	char buffer2 [20];
	//int x;
	char curChar;
	char prevChar;
	char prevChar1;
	int c;
	
	news1 = news; //assign prior message
	
	// debugging ...
	//int y;
	//int z;
	
	// debugging ...
	sprintf(buffer2,"Last Position: %d\r\n",last_char);
	CLI_Write(buffer2);
	
	if (last_char > 30) {
		
		for (c=last_char; c > 0; c--) {
			
			prevChar1 = prevChar;
			prevChar = curChar;
			curChar = Sram_Read_Byte(c);
			//curChar = pgm_read_byte_near(WebPageResponse + c);
			
			// break on feed display
			if ((curChar == '<') && (prevChar == 'F') && (prevChar1 == '>') ) {
				
				// debugging ... 
				//for (y=c; y < last_char; y++) {
				//	z = WebPageResponse[y];
				//	CLI_Write(&z);
				//}
				
				last_char = c;
				break;
			}
			
		}
		
	}
		// debugging ... 
		sprintf(buffer2,"Last Position Ajdust: %d\r\n",last_char);
		CLI_Write(buffer2);
		sprintf(buffer2,"Content Length: %d\r\n",h);
		CLI_Write(buffer2);
		
		
		for (b=last_char; b < h; b++) {
			last_char++;
/*			
			
			tag legend
			<N>
			<F>		feed source			</F> -- format
			<I>		image				</I>	image
			<H>		heading		 		</H>	title
			<T>		time				</T>	time_lapse
			<U>		url					</U>
			<C>		content				</C>	message
			<P1>	stock news title 1 	</P1>	stock_title1
			<P2> 	stock news title 1 	</P2>	stock_title2
			<P3> 	stock news title 1 	</P3>
			<P>		stock price			</P>	home_score
			<AC>	amount change		</AC>	home_team
			<PC>	percent change		</PC>	away_team
			<V>		volume				<V>		end_text
			<T1>	away team			</T1>	away_team
			<S1>	away score			</S1>	away_score
			<T2>	home team			</T2>	home_team
			<S2>	home score			</S2>	home_score
			<E>		end text			</E>	end_text
			</N>
*/

			char inChar = Sram_Read_Byte(b);//= SpiRam.read_byte(b);
			//char inChar = pgm_read_byte_near(WebPageResponse + b); 
			//CLI_Write(&inChar);
			if (inChar == '<') {
				addChar(inChar, tmpStr);
				tagFlag = true;
				dataFlag = false;

				} else if (inChar == '>') {
				addChar(inChar, tmpStr);

				if (tagFlag) {
					strncpy(tagStr, tmpStr, strlen(tmpStr)+1);
				}

				// Clear tmp
				clearStr(tmpStr);

				tagFlag = false;
				dataFlag = true;
				
				} else if (inChar != 10) {
				
				if (tagFlag) {
					// Add tag char to string
					addChar(inChar, tmpStr);

					// Check for </XML> end tag, ignore it
					if ( tagFlag && strcmp(tmpStr, endTag) == 0 ) {
						clearStr(tmpStr);
						tagFlag = false;
						dataFlag = false;
					}
				}
				
				if (dataFlag) {
					// Add data char to string
					addChar(inChar, dataStr);
				}
				
			}  // end  else if (inChar != 10)
			
			// If a LF, process the line
			if (inChar == 10 ) {

				// Find specific tags and print data
				

				if (matchTag("<F>")) {
					if (num_diplayed > num_to_display) {
							sprintf(buffer2,"Last PositionatEnd: %d\r\n",last_char);
							CLI_Write(buffer2);
							clearStr(tmpStr);
							clearStr(tagStr);
							clearStr(dataStr);
							break;
					}
					
					CLI_Write("\r\n");
					sprintf(buffer1,"Number: %d\r\n",num_diplayed);
					//CLI_Write(buffer1);
					CLI_Write("Feed: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					num_diplayed++;
					strcpy( news.format, (const char *)&dataStr );
					
				}

				if (matchTag("<H>")) {
					CLI_Write("Heading: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					strcpy( news.title, (const char *)&dataStr );
				}
				
				if (matchTag("<T>")) {
					CLI_Write("Time: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					 strcpy( news.time_lapse, (const char *)&dataStr );
				}
				
				if (matchTag("<I>")) {
					CLI_Write("Image: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					news.image = atoi ((const char *)&dataStr);
					// strcpy( news.image, );
				}

				if (matchTag("<C>")) {
					CLI_Write("Content: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					strcpy( news.message, (const char *)&dataStr );
				}
				
				if (matchTag("<U>")) {
					CLI_Write("Url: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					
				}
				
				if (matchTag("<P1>")) {
					CLI_Write("Stock 1: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					  strcpy( news.stock_title1, (const char *)&dataStr );
				}
				
				if (matchTag("<P2>")) {
					//CLI_Write("Stock 2: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.stock_title2, (const char *)&dataStr );
				}
				
				if (matchTag("<P3>")) {
					//CLI_Write("Stock 3: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
				}
				
				if (matchTag("<P>")) {
					//CLI_Write("Price: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.home_score, (const char *)&dataStr );
				}
				
				if (matchTag("<AC>")) {
					//CLI_Write("Amount Ch: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.home_team, (const char *)&dataStr );
				}
				
				if (matchTag("<PC>")) {
					//CLI_Write("Perc Ch:: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.away_team, (const char *)&dataStr );
				}
				
				//if (matchTag("<V>")) {
				//	//CLI_Write("Volume: ");
				//	//CLI_Write(&dataStr);
				//	//CLI_Write("\r\n");
				//	strcpy( news.end_text, (const char *)&dataStr );
				//}
				
				if (matchTag("<T1>")) {
					//CLI_Write("Away Team: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.away_team, (const char *)&dataStr );
				
				
				}
				if (matchTag("<S1>")) {
					//CLI_Write("Away Score: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.away_score, (const char *)&dataStr );
				
				}
				if (matchTag("<T2>")) {
					//CLI_Write("Home Team: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.home_team, (const char *)&dataStr );
				
				}
				if (matchTag("<S2>")) {
					//CLI_Write("Home Score: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.home_score, (const char *)&dataStr );
				
				}
				if (matchTag("<E>")) {
					//CLI_Write("Ending: ");
					//CLI_Write(&dataStr);
					//CLI_Write("\r\n");
					strcpy( news.end_text, (const char *)&dataStr );
				
				}
				if (matchTag("<Z>")) {
					CLI_Write("File End: ");
					CLI_Write(&dataStr);
					CLI_Write("\r\n");
					last_char = MAXSPIRAM;
					clearStr(tmpStr);
					clearStr(tagStr);
					clearStr(dataStr);
					break;
					
				}
				
				// Clear all strings
				clearStr(tmpStr);
				clearStr(tagStr);
				clearStr(dataStr);
				
				
				// Clear Flags
				tagFlag = false;
				dataFlag = false;
				
			} //If a LF, process the line
			
			
		} //end for	
	 //
	// print top of screen

		//if ((strcmp ("NEWS",news.format) == 0) || (strcmp ("EMAIL",news.format) == 0) || (strcmp ("TWIT",news.format) == 0)){
		if ((strncmp ("NEWS",news.format,4) == 0) || (strncmp ("EMAIL",news.format,5) == 0) || (strncmp ("TWIT",news.format,4) == 0)){
					
			screen_news(&news);
			
		}
		
		//else if (strcmp ("SPORT",news.format) == 0) {
		else if (strncmp ("SPORT",news.format,5) == 0) {
				
			screen_sports(&news);
			
		}
		
		//else if (strcmp ("STOCK",news.format) == 0) {
		else if (strncmp ("STOCK",news.format,4) == 0) {
				
			screen_stocks(&news);
			
		}
	
	// print bottom of screen
	
		
		//if ((strcmp ("NEWS",news1.format) == 0) || (strcmp ("EMAIL",news1.format) == 0) || (strcmp ("TWIT",news1.format) == 0)){		
		if ((strncmp ("NEWS",news1.format,4) == 0) || (strncmp ("EMAIL",news1.format,5) == 0) || (strncmp ("TWIT",news1.format,4) == 0)){
			
			screen_news2(&news1);
			
		}
		
		//else if (strcmp ("SPORT",news1.format) == 0) {
		else if (strncmp ("SPORT",news1.format,5) == 0) {
					
			screen_sports2(&news1);
			
		}
	
		//else if (strcmp ("STOCK",news1.format) == 0) {
		else if (strncmp ("STOCK",news1.format,4) == 0) {
				
			screen_stocks2(&news1);
			
		}
		
		if (last_char == h)
		{
			CLI_Write("restart loop \r\n");
			bRestartLoop = true;
			
		}
	return true;
	
	
}



void vInitScreen(void)
{
	gfx_init();
	vFlipScreen180();
	at45dbx_init();
	PORTC_INT0MASK=0x01; // 0000 0001
	PORTC_PIN0CTRL=0x1A;	 // X001 1010    //no invert, pull up , falling edge
		
	CLEAR_TRX_IRQ();
	ENABLE_TRX_IRQ();
	cpu_irq_enable();
	gxScreenInfo.xScreenState  = SCREEN_STATE_LOADING;
	gxScreenStatePrevious = SCREEN_STATE_IDEAL;
	vUpdateScreen();
}
void vFlipScreen180(void)
{
	ioport_set_pin_level(CONF_ILI9341_DC_PIN, false);
	
	ioport_set_pin_level(CONF_ILI9341_CS_PIN, false);
	while (!usart_data_register_is_empty(CONF_ILI9341_USART_SPI));

	irqflags_t flags = cpu_irq_save();
	usart_clear_tx_complete(CONF_ILI9341_USART_SPI);
	usart_put(CONF_ILI9341_USART_SPI, 0x36);
	cpu_irq_restore(flags);
	
	while (!usart_tx_is_complete(CONF_ILI9341_USART_SPI));
	usart_clear_tx_complete(CONF_ILI9341_USART_SPI);
	
	ioport_set_pin_level(CONF_ILI9341_DC_PIN, true);
	while (!usart_data_register_is_empty(CONF_ILI9341_USART_SPI));
	
	flags = cpu_irq_save();
	usart_clear_tx_complete(CONF_ILI9341_USART_SPI);
	usart_put(CONF_ILI9341_USART_SPI, 0xE8);
	cpu_irq_restore(flags);
	while (!usart_tx_is_complete(CONF_ILI9341_USART_SPI));
	usart_clear_tx_complete(CONF_ILI9341_USART_SPI);
	
	ioport_set_pin_level(CONF_ILI9341_CS_PIN, true);
}
void vInitCredentialProcess(void)
{
	gxCredentialInfo.bCredentialProcessDone = false;
	gxCredentialInfo.bSettingCredentialButtonPressed = false;
	gxCredentialInfo.uCredentialInfo = getConfigModeFromNVM(EEPROM_CONFIGMODE_ADR);
	
	if(gxCredentialInfo.uCredentialInfo == CREDENTIAL_PROCESS_SMARTCONFIG)
	{
		gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_CREDENTIALS_SAVED;
		gxCredentialInfo.bIsCredentialSaved = true;
	}
	else if(gxCredentialInfo.uCredentialInfo == CREDENTIAL_PROCESS_PROVISIONING_AP)
	{
		gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_CREDENTIALS_SAVED;
		gxCredentialInfo.bIsCredentialSaved = true;
	}
	else
	{
		gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_UNKNOWN;
		gxCredentialInfo.bIsCredentialSaved = false;
	}
	
	//gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_CREDENTIALS_SAVED; // temporary
	//gxCredentialInfo.bIsCredentialSaved = true; // temporary
	gxScreenInfo.bFilterScreenEnabled = false;
	gxScreenInfo.iContentScreenDisplayTime = 0;
	
}

/**
 * \brief main function
 */
int main (void)
{
	setup();

	while (true)
	{
		if(bCredentialProcess())
		{
			vCredentialNetworkConnectProcess();
		}
		
		if(gxCredentialInfo.bCredentialProcessDone == true)
		{
			loop();
		}
		vFilterScreenDisplayCounting();
	}
}