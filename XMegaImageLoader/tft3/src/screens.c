/*
 * screens.c
 *
 * Created: 6/18/2015 7:46:35 AM
 *  Author: ssenthil
 */ 

#include <asf.h>
#include "conf_ads7843.h"
#include "board.h"
#include "ioport.h"
#include "asf/common/services/resistive_touch/rtouch.h"
#include <util/delay.h>
#include "window_manager.h"
#include "screens.h"
#include "ui_objects.h"

#include "flash_fs.h"

#include "logo9.h"




static struct ui_news news[10];
static struct ui_sports sports[10];
static struct ui_email email[10];
static struct ui_stocks stocks[10];

static int num_news=0, num_sports=0, num_email=0, num_stocks=0;
static int disp_news=1, disp_sports=0, disp_email=0, disp_stocks=0;
static int disp_news_i=1, disp_sports_i=1, disp_email_i=1, disp_stocks_i=1;
static int news_shown=0, sports_shown=0, email_shown=0, stocks_shown=0;
static int num_active_contents=0, disp_content=0, disp_content_i=0;
static uint8_t content_no;

static int disp_news_temp=1, disp_sports_temp=0, disp_email_temp=0, disp_stocks_temp=0;


void init_content(void)
{

	news[0].logo.data.start_sector = 10;
	news[0].title="All News";
	news[0].time_lapse = "5 mins ago";
	news[0].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";

	news[1].logo.data.start_sector = 10;
	news[1].title="News";
	news[1].time_lapse = "15 mins ago";
	news[1].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";
	
	news[2].logo.data.start_sector = 0;
	news[2].title="Sports";
	news[2].time_lapse = "8 mins ago";
	news[2].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";

	news[3].logo.data.start_sector = 20;
	news[3].title="Entertainment";
	news[3].time_lapse = "20 mins ago";
	news[3].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";

	news[4].logo.data.start_sector = 10;
	news[4].title="Business";
	news[4].time_lapse = "30 mins ago";
	news[4].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";

	news[5].logo.data.start_sector = 0;
	news[5].title="Technology";
	news[5].time_lapse = "60 mins ago";
	news[5].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";

	news[6].logo.data.start_sector = 20;
	news[6].title="Rest";
	news[6].time_lapse = "8 mins ago";
	news[6].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";

	news[7].logo.data.start_sector = 10;
	news[7].title="Network";
	news[7].time_lapse = "23 mins ago";
	news[7].message="Why Sage therapeutics Inc. Shares are soaring higher - Sage therapeutics inc has released promising early stage data for SAGE-547 in postpartum depression patient";

	num_news=8;

	email[0].logo.data.start_sector = 0;
	email[0].from="Michel and Brad";
	email[0].time_lapse = "5 days ago";
	email[0].subject="Kate and Wills Beach Birthday";
	email[0].message="You're invited! Kate and Will's Beach birthday Michelle and Brad sent you an invitation. View invitation...";

	num_email=1;
	sports[0].logo.data.start_sector = 20;
	sports[0].title="MONDAY 06/08";
	sports[0].team1_name = "Milwaukee Brewers";
	sports[0].team1_score="2-8-0";
	sports[0].team2_name = "Pittsburg Pirates";
	sports[0].team2_score="0-5-1";
	sports[0].game_status= "Final";
	num_sports=1;
	
	stocks[0].logo.data.start_sector = 0;
	stocks[0].up_down = 1;
	stocks[0].title="S&P 500";
	stocks[0].time_lapse = "4:35PM ET";
	stocks[0].ticker="2080.15";
	stocks[0].change="+0.87";
	stocks[0].percent_change="+0.2%";
	stocks[0].news1="Stocks drift mostly lower treasury rates rise";
	stocks[0].news2="US stocks close little changes as Banks gain, Airlines Sli";
	num_stocks=1;
	
}


void my_content_display( uint8_t screen_location)
{
	set_email_status(0);
	num_active_contents = disp_news+disp_sports+disp_email+disp_stocks;

	if (disp_news && num_news && !news_shown)
	{
		screen_news(news[disp_news_i-1], screen_location);
		news_shown=1;
		if (disp_news_i<num_news)
		{
			disp_news_i++;
		}
		else
		{
			disp_news_i=1;
		}
	}
	else if (disp_sports && num_sports && !sports_shown)
	{
		screen_sports(sports[disp_sports_i-1], screen_location);
		sports_shown=1;
		if (disp_sports_i<num_sports)
		{
			disp_sports_i++;
		}
		else
		{
			disp_sports_i=1;
		}
	}
	else if (disp_email && num_email && !email_shown)
	{
		screen_email(email[disp_email_i-1], screen_location);
		email_shown=1;
		if (disp_email_i<num_email)
		{
			disp_email_i++;
		}
		else
		{
			disp_email_i=1;
		}
		set_email_status(1);
	}
	else if (disp_stocks && num_stocks && !stocks_shown)
	{
		screen_stocks(stocks[disp_stocks_i-1], screen_location);
		stocks_shown=1;
		if (disp_stocks_i<num_stocks)
		{
			disp_stocks_i++;
		}
		else
		{
			disp_stocks_i=1;
		}
	}

	if (content_no >= num_active_contents)
	{
		news_shown=0;
		sports_shown=0;
		email_shown=0;
		stocks_shown=0;
		content_no=0;
	}
	else
	content_no++;
	
}


void register_filter_callback(void)
{
	unregister_all_touch_objects();
	
	struct touch_object_t filter_touch;
	filter_touch.enable=1;
	filter_touch.x=0;
	filter_touch.y=0;
	filter_touch.l=320;
	filter_touch.b=240;
	filter_touch.touch_flag=0;
	filter_touch.press_callback= settings_filter;
	
	filter_touch.uid=register_touch_object(filter_touch);
}


static struct ui_icon_button	set_home_ib, set_save_ib;
//static struct ui_simple_button  set_news_b, set_sports_b, set_email_b, set_stocks_b;
static struct ui_icon_button  set_news_ib, set_sports_ib, set_email_ib, set_stocks_ib;

void set_save_callback (int touch_id)
{
	//	reset_idle_timer();
	draw_pressed_icon_button(set_save_ib);
	disp_news=disp_news_temp;
	disp_sports=disp_sports_temp;
	disp_email=disp_email_temp;
	disp_stocks=disp_stocks_temp;
}
void set_home_callback (int touch_id)
{
	go_home();
}
void set_news_callback(int touch_id)
{

	reset_idle_timer();
/*
	if (disp_news_temp)
	disp_news_temp=0;
	else
	disp_news_temp=1;
*/
	disp_news_temp=1;
	disp_sports_temp=0;
	disp_email_temp=0;
	disp_stocks_temp=0;
	display_settings_selection();
}
void set_sports_callback(int touch_id)
{

	reset_idle_timer();
/*
	if (disp_sports_temp)
	disp_sports_temp=0;
	else
	disp_sports_temp=1;
*/
	disp_news_temp=0;
	disp_sports_temp=1;
	disp_email_temp=0;
	disp_stocks_temp=0;
	display_settings_selection();
}
void set_email_callback(int touch_id)
{
	reset_idle_timer();
/*
	if (disp_email_temp)
	disp_email_temp=0;
	else
	disp_email_temp=1;
*/
	disp_news_temp=0;
	disp_sports_temp=0;
	disp_email_temp=1;
	disp_stocks_temp=0;
	display_settings_selection();
}
void set_stocks_callback(int touch_id)
{
	reset_idle_timer();
/*
	if (disp_stocks_temp)
	disp_stocks_temp=0;
	else
	disp_stocks_temp=1;
*/
	disp_news_temp=0;
	disp_sports_temp=0;
	disp_email_temp=0;
	disp_stocks_temp=1;
	display_settings_selection();
}

void settings_filter(int touch_id_i)
{
	unregister_all_touch_objects();
	clr_scr();
	disp_news_temp		=disp_news	;
	disp_sports_temp	=disp_sports;
	disp_email_temp		=disp_email	;
	disp_stocks_temp	=disp_stocks;
	
	set_save_ib.x=180;
	set_save_ib.y=line_no(13);
	set_save_ib.button_text= "  Save";
	set_save_ib.touch.enable=1;
	set_save_ib.touch.touch_flag=0;
	set_save_ib.touch.uid=0;
	set_save_ib.touch.press_callback=set_save_callback;
	set_save_ib.touch.uid=create_icon_button(set_save_ib);

	set_home_ib.x=60;
	set_home_ib.y=line_no(13);
	set_home_ib.button_text= "  Home";
	set_home_ib.touch.enable=1;
	set_home_ib.touch.touch_flag=0;
	set_home_ib.touch.uid=0;
	set_home_ib.touch.press_callback=set_home_callback;
	set_home_ib.touch.uid=create_icon_button(set_home_ib);
	


	set_news_ib.x=5;
	set_news_ib.y=10;
	set_news_ib.button_text= "NEWS";
	set_news_ib.touch.enable=1;
	set_news_ib.touch.touch_flag=0;
	set_news_ib.touch.uid=0;
	set_news_ib.touch.press_callback=set_news_callback;
	set_news_ib.touch.uid=create_icon_button(set_news_ib);



	//static int disp_news=1, disp_sports=1, disp_email=1, disp_stocks=1;
	
/*
	set_news_b.x =5;
	set_news_b.y =10;
	set_news_b.l =30;
	set_news_b.b = 20;
	set_news_b.button_color = GFX_COLOR_WHITE;
	set_news_b.text_color = GFX_COLOR_BLACK;
	set_news_b.button_text = "NEWS";
	set_news_b.font = &bigfont;
	set_news_b.touch.enable = 1;
	set_news_b.touch.x = 5;
	set_news_b.touch.y =10;
	set_news_b.touch.l = 60;
	set_news_b.touch.b=20;
	set_news_b.touch.press_callback=set_news_callback;
	set_news_b.touch.uid = create_button(set_news_b);
*/

	set_sports_ib.x=5;
	set_sports_ib.y=40;
	set_sports_ib.button_text="SPORTS";
	set_sports_ib.touch.enable=1;
	set_sports_ib.touch.touch_flag=0;
	set_sports_ib.touch.uid=0;
	set_sports_ib.touch.press_callback=set_sports_callback;
	set_sports_ib.touch.uid=create_icon_button(set_sports_ib);
	
/*
	set_sports_b.x =5;
	set_sports_b.y =40;
	set_sports_b.l =30;
	set_sports_b.b = 20;
	set_sports_b.button_color = GFX_COLOR_WHITE;
	set_sports_b.text_color = GFX_COLOR_BLACK;
	set_sports_b.button_text = "SPORTS";
	set_sports_b.font = &bigfont;
	set_sports_b.touch.enable = 1;	
	set_sports_b.touch.x = 5;
	set_sports_b.touch.y =40;
	set_sports_b.touch.l = 60;
	set_sports_b.touch.b=20;
	set_sports_b.touch.press_callback=set_sports_callback;
	set_sports_b.touch.uid = create_button(set_sports_b);
*/


	set_email_ib.x=5;
	set_email_ib.y=70;
	set_email_ib.button_text="Settings";
	set_email_ib.touch.enable=1;
	set_email_ib.touch.touch_flag=0;
	set_email_ib.touch.uid=0;
//	set_email_ib.touch.press_callback=set_email_callback;
	set_email_ib.touch.press_callback=screen_settings;
	set_email_ib.touch.uid=create_icon_button(set_email_ib);
	
/*
	set_email_b.x =5;
	set_email_b.y =70;
	set_email_b.l =30;
	set_email_b.b = 20;
	set_email_b.button_color = GFX_COLOR_WHITE;
	set_email_b.text_color = GFX_COLOR_BLACK;
	set_email_b.button_text = "EMAIL";
	set_email_b.font = &bigfont;
	set_email_b.touch.enable = 1;		
	set_email_b.touch.x = 5;
	set_email_b.touch.y =70;
	set_email_b.touch.l = 60;
	set_email_b.touch.b=20;
	set_email_b.touch.press_callback=set_email_callback;
	set_email_b.touch.uid = create_button(set_email_b);
*/

/*
	set_stocks_ib.x=5;
	set_stocks_ib.y=100;
	set_stocks_ib.button_text="Headlines";
	set_stocks_ib.touch.enable=1;
	set_stocks_ib.touch.touch_flag=0;
	set_stocks_ib.touch.uid=0;
	set_stocks_ib.touch.press_callback=set_stocks_callback;
	set_stocks_ib.touch.uid=create_icon_button(set_stocks_ib);
*/
	

	set_stocks_ib.x=5;
	set_stocks_ib.y=110;
	set_stocks_ib.button_text= "Headlines";	
	set_stocks_ib.l=100;
	set_stocks_ib.touch.enable=1;
	set_stocks_ib.touch.touch_flag=0;
	set_stocks_ib.touch.uid=0;
	set_stocks_ib.touch.press_callback=set_stocks_callback;
	set_stocks_ib.touch.uid=create_vicon_button(set_stocks_ib);
	
//	create_vicon_button
	
/*
	set_stocks_b.x =5;
	set_stocks_b.y =100;
	set_stocks_b.l =30;
	set_stocks_b.b = 20;
	set_stocks_b.button_color = GFX_COLOR_WHITE;
	set_stocks_b.text_color = GFX_COLOR_BLACK;
	set_stocks_b.button_text = "STOCKS";
	set_stocks_b.font = &bigfont;
	set_stocks_b.touch.enable = 1;	
	set_stocks_b.touch.x = 5;
	set_stocks_b.touch.y =100;
	set_stocks_b.touch.l = 60;
	set_stocks_b.touch.b=20;
	set_stocks_b.touch.press_callback=set_stocks_callback;
	set_stocks_b.touch.uid = create_button(set_stocks_b);
*/
	
	display_settings_selection();
	
}

void display_settings_selection(void)
{
	
/*
	gfx_draw_circle( 80, 20, 10, GFX_COLOR_GREEN, GFX_WHOLE);
	gfx_draw_circle( 80, 50, 10, GFX_COLOR_GREEN, GFX_WHOLE);
	gfx_draw_circle( 80, 80, 10, GFX_COLOR_GREEN, GFX_WHOLE);
	gfx_draw_circle( 80, 110, 10, GFX_COLOR_GREEN, GFX_WHOLE);
*/

	if (disp_news_temp)
		//gfx_draw_filled_circle( 80, 20, 5, GFX_COLOR_GREEN, GFX_WHOLE);
		draw_pressed_icon_button(set_news_ib);	
	else
		//gfx_draw_filled_circle( 80, 20, 5, GFX_COLOR_WHITE, GFX_WHOLE);
		draw_icon_button(set_news_ib);
	
	if (disp_sports_temp)
		//gfx_draw_filled_circle( 80, 50, 5, GFX_COLOR_GREEN, GFX_WHOLE);
		draw_pressed_icon_button(set_sports_ib);	
	else
		//gfx_draw_filled_circle( 80, 50, 5, GFX_COLOR_WHITE, GFX_WHOLE);
		draw_icon_button(set_sports_ib);

	if (disp_email_temp)
		//gfx_draw_filled_circle( 80, 80, 5, GFX_COLOR_GREEN, GFX_WHOLE);
		draw_pressed_icon_button(set_email_ib);	
	else
		//gfx_draw_filled_circle( 80, 80, 5, GFX_COLOR_WHITE, GFX_WHOLE);
		draw_icon_button(set_email_ib);

	if (disp_stocks_temp)
		//gfx_draw_filled_circle( 80, 110, 5, GFX_COLOR_GREEN, GFX_WHOLE);
		//draw_pressed_icon_button(set_stocks_ib);
		draw_pressed_vicon_button(set_stocks_ib);	
	else
		//gfx_draw_filled_circle( 80, 110, 5, GFX_COLOR_WHITE, GFX_WHOLE);
		//draw_icon_button(set_stocks_ib);
		draw_vicon_button(set_stocks_ib);
	
}









/////////////////////////////////////////////////////////////////////////////////////////////
// full screen clear
void clr_scr(void)
{
		gfx_draw_filled_rect(1,1,320,240,GFX_COLOR_WHITE);
}

// window clear will not affect the menu bar
void clr_window(void)  
{
	gfx_draw_filled_rect(1,1,320,190,GFX_COLOR_WHITE);
}

void screen_loading(void)
{
		struct gfx_bitmap LOGO9;
		LOGO9.width =150;
		LOGO9.height =94;
		LOGO9.type =GFX_BITMAP_PROGMEM;
		LOGO9.data.progmem = &logo9;
		
		gfx_draw_filled_rect(1,1,320,240,GFX_COLOR_WHITE);
		gfx_draw_bitmap(&LOGO9, 160-LOGO9.width/2,70-LOGO9.height/2);
		
		struct ui_text_box loading_text;
		loading_text.x=30;
		loading_text.y=140;
		loading_text.bg_color =GFX_COLOR_WHITE;
		loading_text.text_color =GFX_COLOR_BLACK;
		loading_text.box_color =GFX_COLOR_WHITE;
		loading_text.l=260;
		loading_text.b=40;
		loading_text.text_align=TEXT_ALIGN_CENTER;
		loading_text.text="www.poundnine.com\n Your Personal News Wire";
		loading_text.font= &bigfont;
		ui_draw_text_box(loading_text);
}

void screen_connecting(void)
{
		struct gfx_bitmap LOGO9;
		LOGO9.width =150;
		LOGO9.height =94;
		LOGO9.type =GFX_BITMAP_PROGMEM;
		LOGO9.data.progmem = &logo9;
		
		gfx_draw_filled_rect(1,1,320,240,GFX_COLOR_WHITE);
		gfx_draw_bitmap(&LOGO9, 160-LOGO9.width/2,70-LOGO9.height/2);
		
		struct ui_text_box connecting_text;
		
		connecting_text.x=90;
		connecting_text.y=140;
		connecting_text.bg_color =GFX_COLOR_WHITE;
		connecting_text.text_color =GFX_COLOR_BLACK;
		connecting_text.box_color =GFX_COLOR_WHITE;
		connecting_text.l=150;
		connecting_text.b=40;
		connecting_text.text_align=TEXT_ALIGN_CENTER;
		connecting_text.text="Connecting...";
		connecting_text.font = &bigfont;
		ui_draw_text_box(connecting_text);	
}


static struct ui_icon_button home_ib;
static struct ui_icon_button settings_manual_wifi_ib, settings_smart_config_ib, settings_save_ib;

static struct ui_icon_button trigger_notification_ib;

static struct ui_dropdown speed_dd, homepage_dd ;
void speed_dd_callback(int selected_option)
{
	if (selected_option< speed_dd.num_options)
	{
		speed_dd.selected_option = 	selected_option;
	}
	screen_settings();
}

void homepage_dd_callback(int selected_option)
{
	if (selected_option< homepage_dd.num_options)
	{
		homepage_dd.selected_option = 	selected_option;
	}
	screen_settings();
}

void home_callback(int touch_uid)
{
	draw_pressed_icon_button(home_ib);
	go_home();
}

void settings_save_callback(int touch_uid)
{
	reset_idle_timer();
	draw_pressed_icon_button(settings_save_ib);
}
void trigger_notification_callback(int touch_uid)
{
	draw_pressed_icon_button(trigger_notification_ib);
	screen_notification_triggered();
}

void settings_manual_callback(int touch_uid)
{
	//draw_pressed_icon_button(settings_manual_wifi_ib);
	draw_pressed_vicon_button(settings_manual_wifi_ib);
	screen_wifi_connect();
}
void settings_smart_config_callback(int touch_uid)
{
	//draw_pressed_icon_button(settings_smart_config_ib);
	draw_pressed_vicon_button(settings_smart_config_ib);	
	screen_wifi_smart_config();
}
void screen_settings(void)
{
	reset_idle_timer();
	unregister_all_touch_objects();
	unregister_all_dd();
	clr_scr();
	draw_battery_bar(100);
	gfx_draw_string("Settings",100, line_no(1), &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("Device ID        : 12345678",20, line_no(3), &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("Rotation Speed   :",20, line_no(5), &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("Default Home Page:",20, line_no(7), &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("Manual Wifi      :",20, line_no(9), &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("Wifi Smart Config:",20, line_no(11), &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);

	settings_manual_wifi_ib.x=220;
	settings_manual_wifi_ib.y=line_no(9);
	settings_manual_wifi_ib.l=45;	
	settings_manual_wifi_ib.button_text= " Go";
	settings_manual_wifi_ib.touch.enable=1;
	settings_manual_wifi_ib.touch.touch_flag=0;
	settings_manual_wifi_ib.touch.uid=0;
	settings_manual_wifi_ib.touch.press_callback=settings_manual_callback;
//	settings_manual_wifi_ib.touch.uid=create_icon_button(settings_manual_wifi_ib);
	settings_manual_wifi_ib.touch.uid=create_vicon_button(settings_manual_wifi_ib);
	
	
	
	settings_smart_config_ib.x=220;
	settings_smart_config_ib.y=line_no(11);
	settings_smart_config_ib.l=45;	
	settings_smart_config_ib.button_text= " Go";
	settings_smart_config_ib.touch.enable=1;
	settings_smart_config_ib.touch.touch_flag=0;
	settings_smart_config_ib.touch.uid=0;
	settings_smart_config_ib.touch.press_callback=settings_smart_config_callback;
//	settings_smart_config_ib.touch.uid=create_icon_button(settings_smart_config_ib);
	settings_smart_config_ib.touch.uid=create_vicon_button(settings_smart_config_ib);
	
	settings_save_ib.x=180;
	settings_save_ib.y=line_no(13);
	settings_save_ib.button_text= "  Save";
	settings_save_ib.touch.enable=1;
	settings_save_ib.touch.touch_flag=0;
	settings_save_ib.touch.uid=0;
	settings_save_ib.touch.press_callback=settings_save_callback;
	settings_save_ib.touch.uid=create_icon_button(settings_save_ib);


	trigger_notification_ib.x=10;
	trigger_notification_ib.y=10;
	trigger_notification_ib.button_text= "Notify";
	trigger_notification_ib.touch.enable=1;
	trigger_notification_ib.touch.touch_flag=0;
	trigger_notification_ib.touch.uid=0;
	trigger_notification_ib.touch.press_callback=trigger_notification_callback;
	trigger_notification_ib.touch.uid=create_icon_button(trigger_notification_ib);

	home_ib.x=60;
	home_ib.y=line_no(13);
	home_ib.button_text= "  Home";
	home_ib.touch.enable=1;
	home_ib.touch.touch_flag=0;
	home_ib.touch.uid=0;
	home_ib.touch.press_callback=home_callback;
	home_ib.touch.uid=create_icon_button(home_ib);
	
	
	speed_dd.x = 200; 
	speed_dd.y = 70;
	speed_dd.l = 80;
	speed_dd.b = 25;
	speed_dd.select_callback = speed_dd_callback;
	speed_dd.num_options = 3;
	speed_dd.button_color			= GFX_COLOR_GRAY;
	speed_dd.text_color				= GFX_COLOR_WHITE;
	
	speed_dd.option_b[0].x						= 200;
	speed_dd.option_b[0].y						= 95;
	speed_dd.option_b[0].l						= 80;
	speed_dd.option_b[0].b						= 20;
	speed_dd.option_b[0].button_color			= GFX_COLOR_GRAY;
	speed_dd.option_b[0].button_text			= "Fast";
	speed_dd.option_b[0].text_color				= GFX_COLOR_WHITE;
	speed_dd.option_b[0].touch.x				=200;
	speed_dd.option_b[0].touch.y				=95;
	speed_dd.option_b[0].touch.l				=80;
	speed_dd.option_b[0].touch.b				=20;
	speed_dd.option_b[0].touch.enable			=1;
	speed_dd.option_b[0].touch.touch_flag		=0;
	speed_dd.option_b[0].touch.uid				=0;

	speed_dd.option_b[1].x						= 200;
	speed_dd.option_b[1].y						= 115;
	speed_dd.option_b[1].l						= 80;
	speed_dd.option_b[1].b						= 20;
	speed_dd.option_b[1].button_color			= GFX_COLOR_GRAY;
	speed_dd.option_b[1].button_text			= "Medium";
	speed_dd.option_b[1].text_color				= GFX_COLOR_WHITE;
	speed_dd.option_b[1].touch.x				=200;
	speed_dd.option_b[1].touch.y				=115;
	speed_dd.option_b[1].touch.l				=80;
	speed_dd.option_b[1].touch.b				=20;
	speed_dd.option_b[1].touch.enable			=1;
	speed_dd.option_b[1].touch.touch_flag		=0;
	speed_dd.option_b[1].touch.uid				=0;

	speed_dd.option_b[2].x						= 200;
	speed_dd.option_b[2].y						= 135;
	speed_dd.option_b[2].l						= 80;
	speed_dd.option_b[2].b						= 20;
	speed_dd.option_b[2].button_color			= GFX_COLOR_GRAY;
	speed_dd.option_b[2].button_text			= "Slow";
	speed_dd.option_b[2].text_color				= GFX_COLOR_WHITE;
	speed_dd.option_b[2].touch.x				=200;
	speed_dd.option_b[2].touch.y				=135;
	speed_dd.option_b[2].touch.l				=80;
	speed_dd.option_b[2].touch.b				=20;
	speed_dd.option_b[2].touch.enable			=1;
	speed_dd.option_b[2].touch.touch_flag		=0;
	speed_dd.option_b[2].touch.uid				=0;
	
	create_dropdown(speed_dd);


	homepage_dd.x = 200;
	homepage_dd.y = 100;
	homepage_dd.l = 80;
	homepage_dd.b = 25;
	homepage_dd.select_callback = homepage_dd_callback;
	homepage_dd.num_options = 3;
	homepage_dd.button_color			= GFX_COLOR_GRAY;
	homepage_dd.text_color				= GFX_COLOR_WHITE;
	
	homepage_dd.option_b[0].x						= 200;
	homepage_dd.option_b[0].y						= 125;
	homepage_dd.option_b[0].l						= 80;
	homepage_dd.option_b[0].b						= 20;
	homepage_dd.option_b[0].button_color			= GFX_COLOR_GRAY;
	homepage_dd.option_b[0].button_text			= "Latest";
	homepage_dd.option_b[0].text_color				= GFX_COLOR_WHITE;
	homepage_dd.option_b[0].touch.x				=200;
	homepage_dd.option_b[0].touch.y				=125;
	homepage_dd.option_b[0].touch.l				=80;
	homepage_dd.option_b[0].touch.b				=20;
	homepage_dd.option_b[0].touch.enable			=1;
	homepage_dd.option_b[0].touch.touch_flag		=0;
	homepage_dd.option_b[0].touch.uid				=0;
	
	homepage_dd.option_b[1].x						= 200;
	homepage_dd.option_b[1].y						= 145;
	homepage_dd.option_b[1].l						= 80;
	homepage_dd.option_b[1].b						= 20;
	homepage_dd.option_b[1].button_color			= GFX_COLOR_GRAY;
	homepage_dd.option_b[1].button_text			= "History";
	homepage_dd.option_b[1].text_color				= GFX_COLOR_WHITE;
	homepage_dd.option_b[1].touch.x				=200;
	homepage_dd.option_b[1].touch.y				=145;
	homepage_dd.option_b[1].touch.l				=80;
	homepage_dd.option_b[1].touch.b				=20;
	homepage_dd.option_b[1].touch.enable			=1;
	homepage_dd.option_b[1].touch.touch_flag		=0;
	homepage_dd.option_b[1].touch.uid				=0;
	
	homepage_dd.option_b[2].x						= 200;
	homepage_dd.option_b[2].y						= 165;
	homepage_dd.option_b[2].l						= 80;
	homepage_dd.option_b[2].b						= 20;
	homepage_dd.option_b[2].button_color			= GFX_COLOR_GRAY;
	homepage_dd.option_b[2].button_text			= "Saved";
	homepage_dd.option_b[2].text_color				= GFX_COLOR_WHITE;
	homepage_dd.option_b[2].touch.x				=200;
	homepage_dd.option_b[2].touch.y				=165;
	homepage_dd.option_b[2].touch.l				=80;
	homepage_dd.option_b[2].touch.b				=20;
	homepage_dd.option_b[2].touch.enable			=1;
	homepage_dd.option_b[2].touch.touch_flag		=0;
	homepage_dd.option_b[2].touch.uid				=0;

	create_dropdown(homepage_dd);		
}



struct ui_text_box wifi_connect_help_t;
static struct ui_icon_button wifi_connect_ib;
static struct ui_icon_button settings_ib;

void settings_callback(void)
{
	draw_pressed_icon_button(settings_ib);
screen_settings();
}

void wifi_connect_callback(int touch_uid)
{
	reset_idle_timer();
	draw_pressed_icon_button(wifi_connect_ib);
		struct ui_progress_bar connecting_bar;
		connecting_bar.x = 60;
		connecting_bar.y = line_no(7);
		connecting_bar.l = 200;
		connecting_bar.b = 20;
		connecting_bar.bg_color = GFX_COLOR_WHITE;
		connecting_bar.fill_color = GFX_COLOR_BLUE;
		for (uint8_t i =0;i<100;i++)
		{
			connecting_bar.percent=i;
			draw_progress_bar(connecting_bar);
			_delay_ms(20);
		}
	struct ui_text_box messagebar_t;
	messagebar_t.x=20;
	messagebar_t.y=line_no(9);
	messagebar_t.l= 280;
	messagebar_t.b= 40;
	messagebar_t.bg_color=GFX_COLOR_WHITE;
	messagebar_t.text_align= TEXT_ALIGN_LEFT;
	messagebar_t.text_color=GFX_COLOR_GREEN;
	messagebar_t.text="Message output bar:Accesspoint found. IP:233.233.2.2";
	messagebar_t.font= &sysfont;
	ui_draw_text_box(messagebar_t);		
}


void screen_wifi_connect(void)
{
	reset_idle_timer();
	unregister_all_touch_objects();
	clr_scr();
	wifi_connect_help_t.x=10;
	wifi_connect_help_t.y=10;
	wifi_connect_help_t.l= 300;
	wifi_connect_help_t.b= 50;
	wifi_connect_help_t.bg_color=GFX_COLOR_WHITE;
	wifi_connect_help_t.text_align= TEXT_ALIGN_LEFT;
	wifi_connect_help_t.text_color=GFX_COLOR_BLACK;
	wifi_connect_help_t.text= "Click on connect below to enable Wifi Connect. Then go to http://mysimplelink.net on another device to complete the wifi setup.";
	wifi_connect_help_t.font= &sysfont;
	ui_draw_text_box(wifi_connect_help_t);	
	
	wifi_connect_ib.x=115;
	wifi_connect_ib.y=line_no(5)-8;
	wifi_connect_ib.button_text= "Connect";
	wifi_connect_ib.touch.enable=1;
	wifi_connect_ib.touch.touch_flag=0;
	wifi_connect_ib.touch.uid=0;
	wifi_connect_ib.touch.press_callback=wifi_connect_callback;
	wifi_connect_ib.touch.uid=create_icon_button(wifi_connect_ib);

	settings_ib.x=185;
	settings_ib.y=line_no(13);
	settings_ib.button_text= "Settings";
	settings_ib.touch.enable=1;
	settings_ib.touch.touch_flag=0;
	settings_ib.touch.uid=0;
	settings_ib.touch.press_callback=settings_callback;
	settings_ib.touch.uid=create_icon_button(settings_ib);

	home_ib.x=45;
	home_ib.y=line_no(13);
	home_ib.button_text= "  Home";
	home_ib.touch.enable=1;
	home_ib.touch.touch_flag=0;
	home_ib.touch.uid=0;
	home_ib.touch.press_callback=home_callback;
	home_ib.touch.uid=create_icon_button(home_ib);
			
}

void wifi_smart_connect_callback(int touch_uid)
{
	reset_idle_timer();
	draw_pressed_icon_button(wifi_connect_ib);
		struct ui_progress_bar connecting_bar;
		connecting_bar.x = 60;
		connecting_bar.y = line_no(7);
		connecting_bar.l = 200;
		connecting_bar.b = 20;
		connecting_bar.bg_color = GFX_COLOR_WHITE;
		connecting_bar.fill_color = GFX_COLOR_GREEN;
		for (uint8_t i =0;i<100;i++)
		{
			connecting_bar.percent=i;
			draw_progress_bar(connecting_bar);
			_delay_ms(40);
		}
	struct ui_text_box messagebar_t;
	messagebar_t.x=20;
	messagebar_t.y=line_no(9);
	messagebar_t.l= 280;
	messagebar_t.b= 40;
	messagebar_t.bg_color=GFX_COLOR_WHITE;
	messagebar_t.text_align= TEXT_ALIGN_LEFT;
	messagebar_t.text_color=GFX_COLOR_GREEN;
	messagebar_t.text="Message output bar:Accesspoint found. IP:233.233.2.2";
	messagebar_t.font= &sysfont;
	ui_draw_text_box(messagebar_t);		
		
}


void screen_wifi_smart_config(void)
{
	reset_idle_timer();
	unregister_all_touch_objects();
	clr_scr();
	wifi_connect_help_t.x=10;
	wifi_connect_help_t.y=10;
	wifi_connect_help_t.l= 300;
	wifi_connect_help_t.b= 50;
	wifi_connect_help_t.bg_color=GFX_COLOR_WHITE;
	wifi_connect_help_t.text_align= TEXT_ALIGN_LEFT;
	wifi_connect_help_t.text_color=GFX_COLOR_BLACK;
	wifi_connect_help_t.text= "Smart config enables you to connect this  device  to wifi using the #9 mobile app. press connect here and then immediately press connect on the #9 mobile app to sync wireless network credentials";
	wifi_connect_help_t.font= &sysfont;
	ui_draw_text_box(wifi_connect_help_t);
	
	wifi_connect_ib.x=115;
	wifi_connect_ib.y=line_no(5)-8;
	wifi_connect_ib.button_text= "Connect";
	wifi_connect_ib.touch.enable=1;
	wifi_connect_ib.touch.touch_flag=0;
	wifi_connect_ib.touch.uid=0;
	wifi_connect_ib.touch.press_callback=wifi_smart_connect_callback;
	wifi_connect_ib.touch.uid=create_icon_button(wifi_connect_ib);

	settings_ib.x=185;
	settings_ib.y=line_no(13);
	settings_ib.button_text= "Settings";
	settings_ib.touch.enable=1;
	settings_ib.touch.touch_flag=0;
	settings_ib.touch.uid=0;
	settings_ib.touch.press_callback=settings_callback;
	settings_ib.touch.uid=create_icon_button(settings_ib);

	home_ib.x=45;
	home_ib.y=line_no(13);
	home_ib.button_text= "  Home";
	home_ib.touch.enable=1;
	home_ib.touch.touch_flag=0;
	home_ib.touch.uid=0;
	home_ib.touch.press_callback=home_callback;
	home_ib.touch.uid=create_icon_button(home_ib);
	

		
}

void screen_loading_content(void)
{
	clr_scr();
	draw_battery_bar(100);
	gfx_draw_string("Loading updates for",60, 40, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("NEWS",130, 60, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string("Sources",110, 80, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);	
		struct ui_progress_bar connecting_bar;
		connecting_bar.x = 60;
		connecting_bar.y = line_no(9);
		connecting_bar.l = 200;
		connecting_bar.b = 20;
		connecting_bar.bg_color = GFX_COLOR_WHITE;
		connecting_bar.fill_color = GFX_COLOR_RED;
		for (uint8_t i =0;i<100;i++)
		{
			connecting_bar.percent=i;
			draw_progress_bar(connecting_bar);
			_delay_ms(10);
		}
		
}


void screen_news(struct ui_news news, uint8_t screen_location)
{
//	clr_scr();
	//draw_menu_bar();
	struct ui_text_box news_message_t;
	
	if (screen_location==0)
	{
		gfx_draw_bitmap_from_flash(news.logo.data.start_sector, 5, 2);
		gfx_draw_string(news.title,65, 5, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(news.time_lapse,65, 25, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		news_message_t.x=5;
		news_message_t.y=46;
		news_message_t.l= 310;
		news_message_t.b= 73;	
		gfx_draw_horizontal_line(20,120,280,GFX_COLOR_CYAN);	
	} 
	else
	{
		gfx_draw_bitmap_from_flash(news.logo.data.start_sector, 5, 122);
		gfx_draw_string(news.title,65, 125, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(news.time_lapse,65, 145, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		news_message_t.x=5;
		news_message_t.y=166;
		news_message_t.l= 310;
		news_message_t.b= 73;
	}
/*
	gfx_draw_bitmap_from_flash(news.logo.data.start_sector, 10, 10);
	gfx_draw_string(news.title,80, 20, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news.time_lapse,80, 40, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	struct ui_text_box news_message_t;
	news_message_t.x=10;
	news_message_t.y=60;
	news_message_t.l= 300;
	news_message_t.b= 140;*/
	news_message_t.bg_color=GFX_COLOR_WHITE;
	news_message_t.text_align= TEXT_ALIGN_LEFT;
	news_message_t.text_color=GFX_COLOR_BLACK;
	news_message_t.text= news.message;
	news_message_t.font= &bigfont;
	ui_draw_text_box(news_message_t);
}

void screen_email(struct ui_email email, uint8_t screen_location)
{
	clr_scr();
	//draw_menu_bar();
	gfx_draw_bitmap_from_flash(email.logo.data.start_sector, 5, 2);
	gfx_draw_string(email.from,65, 5, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(email.time_lapse,65, 25, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	struct ui_text_box email_subject_t;
	email_subject_t.x=5;
	email_subject_t.y=60;
	email_subject_t.l= 310;
	email_subject_t.b= 30;
	email_subject_t.bg_color=GFX_COLOR_WHITE;
	email_subject_t.text_align= TEXT_ALIGN_LEFT;
	email_subject_t.text_color=GFX_COLOR_BLACK;
	email_subject_t.text= email.subject;
	email_subject_t.font= &bigfont;
	ui_draw_text_box(email_subject_t);

	struct ui_text_box email_message_t;
	email_message_t.x=5;
	email_message_t.y=100;
	email_message_t.l= 300;
	email_message_t.b= 100;
	email_message_t.bg_color=GFX_COLOR_WHITE;
	email_message_t.text_align= TEXT_ALIGN_LEFT;
	email_message_t.text_color=GFX_COLOR_BLACK;
	email_message_t.text= email.message;
	email_message_t.font = &bigfont;
	ui_draw_text_box(email_message_t);
//	gfx_draw_horizontal_line(20,120,280,GFX_COLOR_CYAN);

}

void screen_stocks(struct ui_stocks stocks, uint8_t screen_location)
{
	
	if (!screen_location)
	{
//		clr_scr();
		//draw_menu_bar();
		//gfx_draw_bitmap_from_flash(stocks.logo.data.start_sector, 5, 2);

		if (stocks.up_down)
		{
			ui_draw_filled_triangle(8, 42, 40, 42, 25, 15, GFX_COLOR_GREEN);
		}
		else
		{
			ui_draw_filled_triangle(65, 25, 90, 25, 80, 45, GFX_COLOR_RED);
		}
		gfx_draw_string(stocks.title,65, 5, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(stocks.ticker,65, 25, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(stocks.change,170, 5, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(stocks.percent_change,170, 25, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		

		struct ui_text_box stocks_news1_t;
		stocks_news1_t.x=5;
		stocks_news1_t.y=50;
		stocks_news1_t.l= 300;
		stocks_news1_t.b= 60;
		stocks_news1_t.bg_color=GFX_COLOR_WHITE;
		stocks_news1_t.text_align= TEXT_ALIGN_LEFT;
		stocks_news1_t.text_color=GFX_COLOR_BLACK;
		stocks_news1_t.text= stocks.news1;
		stocks_news1_t.font= &bigfont;
		ui_draw_text_box(stocks_news1_t);
		
		/*struct ui_text_box stocks_news2_t;
		stocks_news2_t.x=5;
		stocks_news2_t.y=140;
		stocks_news2_t.l= 300;
		stocks_news2_t.b= 60;
		stocks_news2_t.bg_color=GFX_COLOR_WHITE;
		stocks_news2_t.text_align= TEXT_ALIGN_LEFT;
		stocks_news2_t.text_color=GFX_COLOR_BLACK;
		stocks_news2_t.text= stocks.news2;
		stocks_news2_t.font = &bigfont;
		ui_draw_text_box(stocks_news2_t);		*/
		
		 gfx_draw_horizontal_line(20,120,280,GFX_COLOR_CYAN);		
	}
	else
	{
		//gfx_draw_bitmap_from_flash(stocks.logo.data.start_sector, 5, 122);
		gfx_draw_string(stocks.title,90, 125, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		if (stocks.up_down)
		{
			//ui_draw_filled_triangle(5, 45, 45, 45, 25, 15, GFX_COLOR_GREEN);
			ui_draw_filled_triangle(5, 150, 45, 150, 25, 115, GFX_COLOR_GREEN);
		}
		else
		{
			ui_draw_filled_triangle(65, 25, 90, 25, 80, 45, GFX_COLOR_RED);
		}
		gfx_draw_string(stocks.ticker,90, 145, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(stocks.change,170, 145, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(stocks.percent_change,240, 145, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		

		struct ui_text_box stocks_news1_t;
		stocks_news1_t.x=5;
		stocks_news1_t.y=170;
		stocks_news1_t.l= 300;
		stocks_news1_t.b= 60;
		stocks_news1_t.bg_color=GFX_COLOR_WHITE;
		stocks_news1_t.text_align= TEXT_ALIGN_LEFT;
		stocks_news1_t.text_color=GFX_COLOR_BLACK;
		stocks_news1_t.text= stocks.news1;
		stocks_news1_t.font= &bigfont;
		ui_draw_text_box(stocks_news1_t);
			
	}
	

	
	
/*
	
//	clr_scr();
//	draw_menu_bar();
//	gfx_draw_bitmap_from_flash(stocks.logo.data.start_sector, 10, 10);
	gfx_draw_string(stocks.title,100, 20, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	if (stocks.up_down)
	{
		ui_draw_filled_triangle(75, 30, 95, 30, 85, 15, GFX_COLOR_GREEN);
	}
	else
	{
		ui_draw_filled_triangle(75, 30, 95, 30, 85, 45, GFX_COLOR_RED);		
	}
	gfx_draw_string(stocks.ticker,100, 40, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(stocks.change,180, 40, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(stocks.percent_change,250, 40, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	

	struct ui_text_box stocks_news1_t;
	stocks_news1_t.x=10;
	stocks_news1_t.y=60;
	stocks_news1_t.l= 300;
	stocks_news1_t.b= 60;
	stocks_news1_t.bg_color=GFX_COLOR_WHITE;
	stocks_news1_t.text_align= TEXT_ALIGN_LEFT;
	stocks_news1_t.text_color=GFX_COLOR_BLACK;
	stocks_news1_t.text= stocks.news1;
	stocks_news1_t.font= &bigfont;
	ui_draw_text_box(stocks_news1_t);
	
	struct ui_text_box stocks_news2_t;
	stocks_news2_t.x=10;
	stocks_news2_t.y=140;
	stocks_news2_t.l= 300;
	stocks_news2_t.b= 60;
	stocks_news2_t.bg_color=GFX_COLOR_WHITE;
	stocks_news2_t.text_align= TEXT_ALIGN_LEFT;
	stocks_news2_t.text_color=GFX_COLOR_BLACK;
	stocks_news2_t.text= stocks.news2;
	stocks_news2_t.font = &bigfont;
	ui_draw_text_box(stocks_news2_t);	*/	
}

void screen_sports(struct ui_sports sports, uint8_t screen_location)
{
//	clr_scr();
	//draw_menu_bar();
	if (!screen_location)
	{
		gfx_draw_bitmap_from_flash(sports.logo.data.start_sector, 5, 2);
		gfx_draw_string(sports.title,65, 5, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team1_name,20, 50, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team1_score,240, 50, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team2_name,20, 68, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team2_score,240, 68, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.game_status,20, 90, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_horizontal_line(20,120,280,GFX_COLOR_CYAN);	
	} 
	else
	{
		gfx_draw_bitmap_from_flash(sports.logo.data.start_sector, 5, 122);
		gfx_draw_string(sports.title,65, 125, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team1_name,20, 170, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team1_score,240, 170, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team2_name,20, 188, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.team2_score,240, 188, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_string(sports.game_status,20, 210, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
		gfx_draw_horizontal_line(20,120,280,GFX_COLOR_CYAN);	
	}
	
	
/*
	
	gfx_draw_bitmap_from_flash(sports.logo.data.start_sector, 10, 10);	
	gfx_draw_string(sports.title,100, 20, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(sports.team1_name,20, 80, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(sports.team1_score,240, 80, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(sports.team2_name,20, 120, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(sports.team2_score,240, 120, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(sports.game_status,20, 160, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);*/
}

void screen_notification_triggered(void)
{
	clr_scr();
	//draw_menu_bar();	
	draw_battery_bar(100);
	gfx_draw_string("Notification Triggered",60, 40, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);	
		struct ui_progress_bar connecting_bar;
		connecting_bar.x = 60;
		connecting_bar.y = line_no(5);
		connecting_bar.l = 200;
		connecting_bar.b = 20;
		connecting_bar.bg_color = GFX_COLOR_WHITE;
		connecting_bar.fill_color = GFX_COLOR_RED;
		for (uint8_t i =0;i<100;i++)
		{
			connecting_bar.percent=i;
			draw_progress_bar(connecting_bar);
			_delay_ms(10);
		}
	struct ui_text_box messagebar_t;
	messagebar_t.x=20;
	messagebar_t.y=line_no(8);
	messagebar_t.l= 280;
	messagebar_t.b= 60;
	messagebar_t.bg_color=GFX_COLOR_WHITE;
	messagebar_t.text_align= TEXT_ALIGN_LEFT;
	messagebar_t.text_color=GFX_COLOR_GREEN;
	messagebar_t.text="Message output bar: Initiated transfer.   Success..";
	messagebar_t.font= &bigfont;
	ui_draw_text_box(messagebar_t);		
			
}


// total no of menu item is a variable
// each menu item shall be registered with a button name and call back
// the navigation buttons in menu are automatically created when the number of menu items is greater than 3
// the 3 visible menu items to be displayed shall be marked
// the button which is highlighted shall be stored in separate variable
/*

#define M_ALL			0
#define M_NEWS			1
#define M_SPORTS		2
#define M_ENTERTAINMENT	3
#define M_BUSINESS		4
#define M_TECHNOLOGY	5
#define M_REST			6
#define M_NETWORK		7
#define M_EMAILS		8
#define M_SCORES		9
#define M_STOCKS		10
*/

#define MAX_NUM_MENU_BUTTONS	20
static uint8_t num_menu_buttons=0;
static struct menu_button_t menu_buttons[MAX_NUM_MENU_BUTTONS];
static uint8_t selected_menu_item	=0;
static uint8_t visible_first_menu_item	=0;
static struct ui_icon_button left_ib , right_ib, menu1_ib, menu2_ib, menu3_ib;

void left_callback(int touch_uid)
{
	draw_pressed_vicon_button(left_ib);	
	visible_first_menu_item--;
	_delay_ms(100);
	draw_menu_bar();

}
void right_callback(int touch_uid)
{
	draw_pressed_vicon_button(right_ib);
	visible_first_menu_item++;
_delay_ms(100);
	draw_menu_bar();

}

void draw_menu_selection(uint8_t button_no)
{
	switch (button_no)
	{
		case 0:
			draw_pressed_vicon_button(menu1_ib);
			break;
		case 1:
			draw_pressed_vicon_button(menu2_ib);
			break;
		case 2:
			draw_pressed_vicon_button(menu3_ib);
			break;
	}
}

void menu1_callback(int touch_uid)
{
	draw_menu_selection(0);
	draw_pressed_vicon_button(menu1_ib);
	selected_menu_item=visible_first_menu_item;
	menu_buttons[selected_menu_item].press_callback(selected_menu_item);
	draw_vicon_button(menu1_ib);
}

void menu2_callback(int touch_uid)
{
	draw_menu_selection(1);
	draw_pressed_vicon_button(menu2_ib);
	selected_menu_item=visible_first_menu_item+1;
	menu_buttons[selected_menu_item].press_callback(selected_menu_item);
	draw_vicon_button(menu2_ib);
}

void menu3_callback(int touch_uid)
{
	draw_menu_selection(2);	
	draw_pressed_vicon_button(menu3_ib);
	selected_menu_item=visible_first_menu_item+2;
	menu_buttons[selected_menu_item].press_callback(selected_menu_item);
	draw_vicon_button(menu3_ib);
}
void init_menus(struct menu_button_t my_menu[], uint8_t num_buttons)
{
	num_menu_buttons= num_buttons;
	for (uint8_t i=0; i<num_menu_buttons; i++)
	{
		menu_buttons[i].button_text=my_menu[i].button_text;
		menu_buttons[i].press_callback=my_menu[i].press_callback;
	}

	left_ib.x=0;
	left_ib.y=line_no(13);
	left_ib.l=30;
	left_ib.button_text= "<<";
	left_ib.touch.enable=1;
	left_ib.touch.touch_flag=0;
	left_ib.touch.uid=0;
	left_ib.touch.press_callback=left_callback;
	
	right_ib.x=290;
	right_ib.y=line_no(13);
	right_ib.l=30;	
	right_ib.button_text= ">>";
	right_ib.touch.enable=1;
	right_ib.touch.touch_flag=0;
	right_ib.touch.uid=0;
	right_ib.touch.press_callback=right_callback;

	menu1_ib.x=40;
	menu1_ib.y=line_no(13);
	menu1_ib.l=70;
	menu1_ib.touch.enable=1;
	menu1_ib.touch.touch_flag=0;
	menu1_ib.touch.uid=0;
	menu1_ib.touch.press_callback=menu1_callback;
	
	menu2_ib.x=125;
	menu2_ib.y=line_no(13);
	menu2_ib.l=70;
	menu2_ib.touch.enable=1;
	menu2_ib.touch.touch_flag=0;
	menu2_ib.touch.uid=0;
	menu2_ib.touch.press_callback=menu2_callback;
	
	menu3_ib.x=210;
	menu3_ib.y=line_no(13);
	menu3_ib.l=70;	
	menu3_ib.touch.enable=1;
	menu3_ib.touch.touch_flag=0;
	menu3_ib.touch.uid=0;
	menu3_ib.touch.press_callback=menu3_callback;
}

void draw_menu_bar()
{
	gfx_draw_filled_rect(1,200,320,240,GFX_COLOR_WHITE);	
	unregister_all_touch_objects();
	
	if (num_menu_buttons>3)
	{
		if(visible_first_menu_item > 0)
		{// show left arrow
			left_ib.touch.uid=create_vicon_button(left_ib);
		}
		if(visible_first_menu_item+3 < num_menu_buttons)
		{// show right arrow
			right_ib.touch.uid=create_vicon_button(right_ib);			
		}
	}
	
	menu1_ib.button_text= menu_buttons[visible_first_menu_item].button_text;	
	menu1_ib.touch.uid=create_vicon_button(menu1_ib);
	
	menu2_ib.button_text= menu_buttons[visible_first_menu_item+1].button_text;
	menu2_ib.touch.uid=create_vicon_button(menu2_ib);
	
	menu3_ib.button_text= menu_buttons[visible_first_menu_item+2].button_text;
	menu3_ib.touch.uid=create_vicon_button(menu3_ib);
//	draw_menu_selection(selected_menu_item-visible_first_menu_item);	
}



#define NUM_DISP_FILES 8

static int fs_start_file, fs_sector, fs_selected_file;
static struct ui_icon_button fs_left_ib , fs_right_ib, fs_up_ib, fs_down_ib;

void fs_left_ib_callback(int touch_uid)
{
	draw_pressed_vicon_button(fs_left_ib);
	if (fs_start_file > NUM_DISP_FILES-1)
	{
		fs_start_file-=NUM_DISP_FILES;
		fs_selected_file=fs_start_file;
		ffs_display_file_table(fs_start_file,fs_start_file+NUM_DISP_FILES-1, fs_selected_file);	
		if (ffs_display_icon_from_flash(fs_selected_file+1,  120, 140)==-1)
		{
			gfx_draw_filled_rect(120, 140, 50, 50, GFX_COLOR_WHITE);
		}
	}
	draw_vicon_button(fs_left_ib);	
}
void fs_right_ib_callback(int touch_uid)
{int end_file;
	draw_pressed_vicon_button(fs_right_ib);
	if (fs_start_file < 576)
	{
		fs_start_file+=NUM_DISP_FILES;
		end_file = fs_start_file+NUM_DISP_FILES-1;
		fs_selected_file=fs_start_file;	
			
		ffs_display_file_table(fs_start_file, end_file, fs_selected_file);		
		if (ffs_display_icon_from_flash(fs_selected_file+1,  120, 140)==-1)
		{
			gfx_draw_filled_rect(120, 140, 50, 50, GFX_COLOR_WHITE);
		}
	}
	draw_vicon_button(fs_right_ib);
}
void fs_up_ib_callback(int touch_uid)
{
	draw_pressed_vicon_button(fs_up_ib);
	if (fs_selected_file>(fs_start_file))
	{
		fs_selected_file--;
	}
	ffs_display_file_table(fs_start_file, fs_start_file+NUM_DISP_FILES-1, fs_selected_file);		
	if (ffs_display_icon_from_flash(fs_selected_file+1,  120, 140)==-1)
	{
		gfx_draw_filled_rect(120, 140, 50, 50, GFX_COLOR_WHITE);
	}
	draw_vicon_button(fs_up_ib);	
}
void fs_down_ib_callback(int touch_uid)
{
	draw_pressed_vicon_button(fs_down_ib);
	if (fs_selected_file<(fs_start_file+NUM_DISP_FILES-1))
	{
		fs_selected_file++;
	}
	ffs_display_file_table(fs_start_file, fs_start_file+NUM_DISP_FILES-1, fs_selected_file);		
	if (ffs_display_icon_from_flash(fs_selected_file+1,  120, 140)==-1)
	{
		gfx_draw_filled_rect(120, 140, 50, 50, GFX_COLOR_WHITE);
	}
	draw_vicon_button(fs_down_ib);	
}

void screen_explorer(void)
{
	fs_start_file=0;
	fs_sector=0;
	fs_selected_file =0;
	clr_scr();
	unregister_all_touch_objects();	
	ffs_display_file_table(fs_start_file,fs_start_file+NUM_DISP_FILES-1, fs_selected_file);
	
	fs_left_ib.x=10;
	fs_left_ib.y=line_no(12);
	fs_left_ib.l=60;
	fs_left_ib.touch.enable=1;
	fs_left_ib.touch.touch_flag=0;
	fs_left_ib.touch.uid=0;
	fs_left_ib.touch.press_callback=fs_left_ib_callback;	
	fs_left_ib.button_text= " <<";
	fs_left_ib.touch.uid=create_vicon_button(fs_left_ib);
	
	fs_up_ib.x=90;
	fs_up_ib.y=line_no(12);
	fs_up_ib.l=60;
	fs_up_ib.touch.enable=1;
	fs_up_ib.touch.touch_flag=0;
	fs_up_ib.touch.uid=0;
	fs_up_ib.touch.press_callback=fs_up_ib_callback;
	fs_up_ib.button_text= " ^ ";
	fs_up_ib.touch.uid=create_vicon_button(fs_up_ib);
	
	fs_down_ib.x=170;
	fs_down_ib.y=line_no(12);
	fs_down_ib.l=60;
	fs_down_ib.touch.enable=1;
	fs_down_ib.touch.touch_flag=0;
	fs_down_ib.touch.uid=0;
	fs_down_ib.touch.press_callback=fs_down_ib_callback;
	fs_down_ib.button_text= " v ";
	fs_down_ib.touch.uid=create_vicon_button(fs_down_ib);


	fs_right_ib.x=250;
	fs_right_ib.y=line_no(12);
	fs_right_ib.l=60;
	fs_right_ib.touch.enable=1;
	fs_right_ib.touch.touch_flag=0;
	fs_right_ib.touch.uid=0;
	fs_right_ib.touch.press_callback=fs_right_ib_callback;
	fs_right_ib.button_text= " >> ";
	fs_right_ib.touch.uid=create_vicon_button(fs_right_ib);


	if (ffs_display_icon_from_flash(fs_selected_file+1,  120, 140)==-1)
	{
		gfx_draw_filled_rect(120, 140, 50, 50, GFX_COLOR_WHITE);
	}
	
}