/*
 * BaseCodeSub.h
 *
 * Created: 05-10-2015 09:13:56
 *  Author: Ecloud6_2
 */ 


#ifndef BASECODESUB_H_ 
#define BASECODESUB_H_

#ifdef __cplusplus
extern "C"
{
#endif

// Include header file
#include "delay.h"
#include "gfx.h"
#include "gfx_text.h"
#include "ads7843.h"
#include "cli_uart_wrapper.h"
#include "screens.h"
#include "window_manager.h"
#include "ui_objects.h"
#include "rtc.h"
// Macro definition
#define DISPLAY_CONTENT_TIMEOUT        5000
#define DISPLAY_FILTER_SCREEN_TIMEOUT  5000

// Global variable
typedef bool boolean;

// Credential process.................................starts
typedef enum
{
	CREDENTIAL_PROCESS_UNKNOWN = 0,
	CREDENTIAL_PROCESS_CREDENTIALS_SAVED = 1,
	CREDENTIAL_PROCESS_SMARTCONFIG = 2,
	CREDENTIAL_PROCESS_PROVISIONING_AP = 3,
	CREDENTIAL_PROCESS_CONNECTING_TO_SETTING = 4,
}CredentilProcessInfo_e;

typedef struct
{
	CredentilProcessInfo_e  xCredentialProcessInfo;
	boolean                 bCredentialProcessDone;
	boolean                 bIsCredentialSaved;
	uint8_t					uCredentialInfo;
	boolean				    bSettingCredentialButtonPressed;
}CredentialInfo_t;

//........................................credential process end

// Screen information starts.........................................
typedef enum
{
	SCREEN_STATE_IDEAL = 0,
	SCREEN_STATE_LOADING,
	SCREEN_STATE_CONNECTING,
	SCREEN_STATE_LOADING_CONTENT2,
	SCREEN_STATE_SETTINGS,
	SCREEN_STATE_MOBILE_SMARTCONFIG,
	SCREEN_STATE_PROVISIONING_AP,
	SCREEN_STATE_NEWS,
	SCREEN_STATE_FILTER,
}ScreenStateInfo_e;

typedef struct
{
	ScreenStateInfo_e     xScreenState;
	boolean               bLoadingCredentialProgressBAR;
	boolean				  bLoadingContentProgressBAR;
	boolean				  bFilterScreenEnabled;
	int					  iContentScreenDisplayTime;
	int					  iFilterScreenDisplayTime;
	boolean               bDoNOtWaitGoForDisplayingAfterEveryIteration;
}ScreenInfo_t;
//.................................................screen info end


// Function declaration
// From Ili9341.c
void ili9341_exit_standby1(void);

// From XMEGA_BASECODE.........................starts
bool configSmartConfig();
bool configProvisioningAP();
bool configSavedCredentials();
bool request_dhcp();
bool displayConnectionDetails();
bool connect_accesspoint();
void vGetPoundNineIP(void);

//..........................................From XMEGA_BASECODE end

// From BaseCodeSub...........................starts
boolean bCredentialProcess(void);
boolean vCredentialNetworkConnectProcess(void);
void vInitCredentialProcess();
void vUpdateScreen();
void vProgressBAR(uint8_t tuPart,char* Message);
void vFilterScreenDisplayCounting(void);
//.................................................................. From BaseCodeSub end


#ifdef __cplusplus
}
#endif // End __cplusplus

#endif /* BASECODESUB_H_ */