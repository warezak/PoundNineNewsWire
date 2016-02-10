/*
 * BaseCodeSub.cpp
 *
 * Created: 05-10-2015 09:12:34
 *  Author: Ecloud6_2
 */ 

#include "BaseCodeSub.h"

ScreenInfo_t gxScreenInfo;
CredentialInfo_t gxCredentialInfo;
ScreenStateInfo_e gxScreenStatePrevious;

boolean bCredentialProcess(void)
{
	if(gxCredentialInfo.bSettingCredentialButtonPressed == true || (gxCredentialInfo.bCredentialProcessDone == false))
	{
		switch(gxCredentialInfo.xCredentialProcessInfo)
		{
			case CREDENTIAL_PROCESS_CREDENTIALS_SAVED:
			gxScreenInfo.xScreenState  = SCREEN_STATE_CONNECTING;
			vUpdateScreen();
			if(!connect_accesspoint())
			{
				gxCredentialInfo.xCredentialProcessInfo = CREDENTIAL_PROCESS_CONNECTING_TO_SETTING;
				return false;
			}
			gxCredentialInfo.bCredentialProcessDone = true;
			return true;
			break;
			case CREDENTIAL_PROCESS_SMARTCONFIG:
			gxCredentialInfo.uCredentialInfo = CREDENTIAL_PROCESS_SMARTCONFIG;
			gxScreenInfo.xScreenState  = SCREEN_STATE_MOBILE_SMARTCONFIG;
			gxCredentialInfo.bIsCredentialSaved =  false;
			if(configSmartConfig() == false)
			{
				return false;
			}
			vProgressBAR(2,"Credential Received");
			vProgressBAR(3,"connecting to access point ");
			break;
			case CREDENTIAL_PROCESS_PROVISIONING_AP:
			gxCredentialInfo.uCredentialInfo = CREDENTIAL_PROCESS_PROVISIONING_AP;
			gxScreenInfo.xScreenState  = SCREEN_STATE_PROVISIONING_AP;
			gxCredentialInfo.bIsCredentialSaved = false;
			vProgressBAR(1,"wifi connect enabled");
			if(configProvisioningAP() == false)
			{
				return false;
			}
			break;
			case CREDENTIAL_PROCESS_UNKNOWN :
				gxCredentialInfo.uCredentialInfo = CREDENTIAL_PROCESS_UNKNOWN;
				gxScreenInfo.xScreenState  = SCREEN_STATE_SETTINGS;
				vUpdateScreen();
				return false;
			break;
			case CREDENTIAL_PROCESS_CONNECTING_TO_SETTING :
					return false;
			break;
			default:
				return false;
			break;
		}
		
		if(configSavedCredentials())
		{
			gxCredentialInfo.bCredentialProcessDone = true;
			gxCredentialInfo.bSettingCredentialButtonPressed = false;
			return true;
		}
	}
	return false;
}

boolean vCredentialNetworkConnectProcess(void)
{
	if(gxCredentialInfo.bCredentialProcessDone == true)
	{
		request_dhcp();
		uint8_t tuRetryIpAccessCnt = 0;		
		while (! displayConnectionDetails()) {
			delay_ms(5000);
			tuRetryIpAccessCnt++;
			if(tuRetryIpAccessCnt >= 2)
			  break;
		}
		gxScreenInfo.xScreenState  = SCREEN_STATE_LOADING_CONTENT2;
		vUpdateScreen();
		vGetPoundNineIP();
		gxScreenInfo.bLoadingContentProgressBAR = true;
	}

	
}
void vUpdateScreen()
{
	if(gxScreenInfo.xScreenState != gxScreenStatePrevious)
	{
		switch(gxScreenInfo.xScreenState )
		{
			case SCREEN_STATE_IDEAL:
			break;
			
			case SCREEN_STATE_LOADING:
			screen_loading();
			ili9341_exit_standby1();
			delay_ms(500);
			break;
			
			case SCREEN_STATE_CONNECTING:
			screen_connecting();
			break;
			
			case SCREEN_STATE_LOADING_CONTENT2:
			screen_loading_content2("   View All");
			break;
			
			case SCREEN_STATE_SETTINGS:
			screen_settings();
			break;
			
			case SCREEN_STATE_MOBILE_SMARTCONFIG:
			screen_wifi_smart_config();
			break;
			
			case SCREEN_STATE_PROVISIONING_AP:
			screen_wifi_connect();
			break;
			
			default:
			break;
		}
		gxScreenStatePrevious = gxScreenInfo.xScreenState;
	}
}
void vProgressBAR(uint8_t tuPart,char* Mesage)
{
	vComplete_Progress_Bar(gxCredentialInfo.xCredentialProcessInfo,tuPart,Mesage);
}
void vFilterScreenDisplayCounting(void)
{
	if(gxScreenInfo.bFilterScreenEnabled == true)
	{
		delay_ms(1);
		gxScreenInfo.iFilterScreenDisplayTime++;
		if(gxScreenInfo.iFilterScreenDisplayTime >= DISPLAY_FILTER_SCREEN_TIMEOUT)
		{
			gxScreenInfo.bFilterScreenEnabled = false;
			gxScreenInfo.iFilterScreenDisplayTime = 0;
		}
	}
	
}
