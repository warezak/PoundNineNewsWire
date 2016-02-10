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
#include "tft/common/services/resistive_touch/rtouch.h"
#include <util/delay.h>
#include "window_manager.h"
#include "screens.h"
#include "ui_objects.h"

#include "flash_fs.h"

#include "logo9.h"

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
	filter_touch.press_callback= screen_filter;
	
	filter_touch.uid=register_touch_object(filter_touch);
}


static struct ui_icon_button	set_home_ib, set_save_ib;
//static struct ui_simple_button  set_news_b, set_sports_b, set_email_b, set_stocks_b;
static struct ui_icon_button  set_all_ib, set_news_ib, set_headlines_ib,set_sports_ib, set_business_ib, set_tech_ib, set_entert_ib, set_rest_ib, set_twitter_ib, set_feedly_ib, set_scores_ib, set_nhl_ib, set_nba_ib, set_mlb_ib, set_nfl_ib,  set_email_ib, set_stocks_ib, set_settings_ib;

void set_save_callback (int touch_id)
{
	//	reset_idle_timer();
	draw_pressed_icon_button(set_save_ib);
}
void set_home_callback (int touch_id)
{
	go_home();
}

/* column 1 */
void set_all_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_all_ib);
	draw_pressed_vicon_button(set_all_ib);
	_delay_ms(500);
	screen_loading_content2("     View All");
	go_home();
	vFilterProcess("Latest");
}
void set_news_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_news_ib);
	draw_pressed_vicon_button(set_news_ib);
	_delay_ms(500);
	screen_loading_content2("  Headline News");
	go_home();  
	vFilterProcess("Headline+News");
}
void set_sports_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_sports_ib);
	draw_pressed_vicon_button(set_sports_ib);
	_delay_ms(500);
	screen_loading_content2("    Sports News");
	go_home();
	vFilterProcess("Sports");	
}
void set_business_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_business_ib);
	draw_pressed_vicon_button(set_business_ib);
	_delay_ms(500);
	screen_loading_content2("  Business News");
	go_home();
	vFilterProcess("Business");
}

void set_tech_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_tech_ib);
	draw_pressed_vicon_button(set_tech_ib);
	_delay_ms(500);
	screen_loading_content2(" Technology News");
	go_home();
	vFilterProcess("Technology");
}
void set_entert_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_entert_ib);
	draw_pressed_vicon_button(set_entert_ib);
	_delay_ms(500);
	screen_loading_content2("Entertainment News");
	go_home();
	vFilterProcess("Entertainment");
}

/* column 2 */
void set_rest_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_rest_ib);
	draw_pressed_vicon_button(set_rest_ib);
	_delay_ms(500);
	screen_loading_content2("      Rest");
	go_home();
	vFilterProcess("The+Rest");
}

void set_twitter_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_twitter_ib);
	draw_pressed_vicon_button(set_twitter_ib);
	_delay_ms(500);
	screen_loading_content2("     Twitter");
	go_home();
	vFilterProcess("Twitter");
}
void set_feedly_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_feedly_ib);
	draw_pressed_vicon_button(set_feedly_ib);
	_delay_ms(500);
	screen_loading_content2("      Feedly");
	go_home();
	vFilterProcess("Feedly");
}
void set_scores_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_scores_ib);
	draw_pressed_vicon_button(set_scores_ib);
	_delay_ms(500);
	screen_loading_content2("   Sport Scores");
	go_home();
	vFilterProcess("Scores");
}
void set_nhl_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_nhl_ib);
	draw_pressed_vicon_button(set_nhl_ib);
	_delay_ms(500);
	screen_loading_content2("    NHL Scores");
	go_home();
	vFilterProcess("NHL");
}
void set_nba_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_nba_ib);
	draw_pressed_vicon_button(set_nba_ib);
	_delay_ms(500);
	screen_loading_content2("    NBA Scores");
	go_home();
	vFilterProcess("NBA");
}

/* column 3 */


void set_mlb_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_mlb_ib);
	draw_pressed_vicon_button(set_mlb_ib);
	_delay_ms(500);
	screen_loading_content2("    MLB Scores");
	go_home();
	vFilterProcess("MLB");
}

void set_nfl_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_nfl_ib);
	draw_pressed_vicon_button(set_nfl_ib);
	_delay_ms(500);
	screen_loading_content2("    NFL Scores");
	go_home();
	vFilterProcess("NFL");
}
void set_email_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_email_ib);
	draw_pressed_vicon_button(set_email_ib);
	_delay_ms(500);
	screen_loading_content2("      Email");
	go_home();
	vFilterProcess("Email");
}
void set_stocks_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_stocks_ib);
	draw_pressed_vicon_button(set_stocks_ib);
	_delay_ms(500);
	screen_loading_content2("    Stocks");
	go_home();
	vFilterProcess("Stocks");
}
void set_settings_callback(int touch_id)
{

	reset_idle_timer();
	//draw_pressed_icon_button(set_settings_ib);
	draw_pressed_vicon_button(set_settings_ib);
	_delay_ms(500);
	//screen_settings();
	//screen_state=IDLE;
	//go_home();
	vFilterProcess("Settings");
}
/* end */

void screen_filter(int touch_id_i)
{
	unregister_all_touch_objects();
	clr_scr();
	
	struct ui_text_box loading_text;
	loading_text.x=5;
	loading_text.y=0;
	loading_text.bg_color =GFX_COLOR_WHITE;
	loading_text.text_color =GFX_COLOR_BLACK;
	loading_text.box_color =GFX_COLOR_WHITE;
	loading_text.l=300;
	loading_text.b=20;
	loading_text.text_align=TEXT_ALIGN_CENTER;
	loading_text.text="Select your content filter:";
	loading_text.font= &bigfont;
	ui_draw_text_box(loading_text);
	
	set_all_ib.x=5;
	set_all_ib.y=30;
	set_all_ib.button_text= "View All";
	set_all_ib.l=100; // for variable icon
	set_all_ib.touch.enable=1;
	set_all_ib.touch.touch_flag=0;
	set_all_ib.touch.uid=0;
	set_all_ib.touch.press_callback=set_all_callback;
	//set_all_ib.touch.uid=create_icon_button(set_all_ib);
	set_all_ib.touch.uid=create_vicon_button(set_all_ib);

	set_news_ib.x=5;
	set_news_ib.y=62;
	set_news_ib.button_text="Headlines";
	set_news_ib.l=100;
	set_news_ib.touch.enable=1;
	set_news_ib.touch.touch_flag=0;
	set_news_ib.touch.uid=0;
	set_news_ib.touch.press_callback=set_news_callback;
	set_news_ib.touch.uid=create_vicon_button(set_news_ib);
	
	set_sports_ib.x=5;
	set_sports_ib.y=94;
	set_sports_ib.button_text="Sports";
	set_sports_ib.l=100;
	set_sports_ib.touch.enable=1;
	set_sports_ib.touch.touch_flag=0;
	set_sports_ib.touch.uid=0;
	set_sports_ib.touch.press_callback=set_sports_callback;
	set_sports_ib.touch.uid=create_vicon_button(set_sports_ib);
	
	set_business_ib.x=5;
	set_business_ib.y=126;
	set_business_ib.button_text="Business";
	set_business_ib.l=100;
	set_business_ib.touch.enable=1;
	set_business_ib.touch.touch_flag=0;
	set_business_ib.touch.uid=0;
	set_business_ib.touch.press_callback=set_business_callback;
	set_business_ib.touch.uid=create_vicon_button(set_business_ib);
	
	set_tech_ib.x=5;
	set_tech_ib.y=158;
	set_tech_ib.button_text="Tech";
	set_tech_ib.l=100;
	set_tech_ib.touch.enable=1;
	set_tech_ib.touch.touch_flag=0;
	set_tech_ib.touch.uid=0;
	set_tech_ib.touch.press_callback=set_tech_callback;
	set_tech_ib.touch.uid=create_vicon_button(set_tech_ib);
	
	set_entert_ib.x=5;
	set_entert_ib.y=190;
	set_entert_ib.button_text="Entertain";
	set_entert_ib.l=100;
	set_entert_ib.touch.enable=1;
	set_entert_ib.touch.touch_flag=0;
	set_entert_ib.touch.uid=0;
	set_entert_ib.touch.press_callback=set_entert_callback;
	set_entert_ib.touch.uid=create_vicon_button(set_entert_ib);
	
	
	/* column 2 */
	
	set_rest_ib.x=110;
	set_rest_ib.y=30;
	set_rest_ib.button_text= "Rest";
	set_rest_ib.l=100;
	set_rest_ib.touch.enable=1;
	set_rest_ib.touch.touch_flag=0;
	set_rest_ib.touch.uid=0;
	set_rest_ib.touch.press_callback=set_rest_callback;
	set_rest_ib.touch.uid=create_vicon_button(set_rest_ib);

	set_twitter_ib.x=110;
	set_twitter_ib.y=62;
	set_twitter_ib.button_text="Twitter";
	set_twitter_ib.l=100;
	set_twitter_ib.touch.enable=1;
	set_twitter_ib.touch.touch_flag=0;
	set_twitter_ib.touch.uid=0;
	set_twitter_ib.touch.press_callback=set_twitter_callback;
	set_twitter_ib.touch.uid=create_vicon_button(set_twitter_ib);
	
	set_feedly_ib.x=110;
	set_feedly_ib.y=94;
	set_feedly_ib.button_text="Feedly";
	set_feedly_ib.l=100;
	set_feedly_ib.touch.enable=1;
	set_feedly_ib.touch.touch_flag=0;
	set_feedly_ib.touch.uid=0;
	set_feedly_ib.touch.press_callback=set_feedly_callback;
	set_feedly_ib.touch.uid=create_vicon_button(set_feedly_ib);
	
	set_scores_ib.x=110;
	set_scores_ib.y=126;
	set_scores_ib.button_text="Scores";
	set_scores_ib.l=100;
	set_scores_ib.touch.enable=1;
	set_scores_ib.touch.touch_flag=0;
	set_scores_ib.touch.uid=0;
	set_scores_ib.touch.press_callback=set_scores_callback;
	set_scores_ib.touch.uid=create_vicon_button(set_scores_ib);
	
	set_nhl_ib.x=110;
	set_nhl_ib.y=158;
	set_nhl_ib.button_text="NHL";
	set_nhl_ib.l=100;
	set_nhl_ib.touch.enable=1;
	set_nhl_ib.touch.touch_flag=0;
	set_nhl_ib.touch.uid=0;
	set_nhl_ib.touch.press_callback=set_nhl_callback;
	set_nhl_ib.touch.uid=create_vicon_button(set_nhl_ib);
	
	set_nba_ib.x=110;
	set_nba_ib.y=190;
	set_nba_ib.button_text="NBA";
	set_nba_ib.l=100;
	set_nba_ib.touch.enable=1;
	set_nba_ib.touch.touch_flag=0;
	set_nba_ib.touch.uid=0;
	set_nba_ib.touch.press_callback=set_nba_callback;
	set_nba_ib.touch.uid=create_vicon_button(set_nba_ib);
	
	
		/* column 3 */
		
	set_mlb_ib.x=215;
	set_mlb_ib.y=30;
	set_mlb_ib.button_text= "MLB";
	set_mlb_ib.l=100;
	set_mlb_ib.touch.enable=1;
	set_mlb_ib.touch.touch_flag=0;
	set_mlb_ib.touch.uid=0;
	set_mlb_ib.touch.press_callback=set_mlb_callback;
	set_mlb_ib.touch.uid=create_vicon_button(set_mlb_ib);

	set_nfl_ib.x=215;
	set_nfl_ib.y=62;
	set_nfl_ib.button_text="NFL";
	set_nfl_ib.l=100;
	set_nfl_ib.touch.enable=1;
	set_nfl_ib.touch.touch_flag=0;
	set_nfl_ib.touch.uid=0;
	set_nfl_ib.touch.press_callback=set_nfl_callback;
	set_nfl_ib.touch.uid=create_vicon_button(set_nfl_ib);
	
	set_email_ib.x=215;
	set_email_ib.y=94;
	set_email_ib.button_text="Email";
	set_email_ib.l=100;
	set_email_ib.touch.enable=1;
	set_email_ib.touch.touch_flag=0;
	set_email_ib.touch.uid=0;
	set_email_ib.touch.press_callback=set_email_callback;
	set_email_ib.touch.uid=create_vicon_button(set_email_ib);
	
	set_stocks_ib.x=215;
	set_stocks_ib.y=126;
	set_stocks_ib.button_text="Stocks";
	set_stocks_ib.l=100;
	set_stocks_ib.touch.enable=1;
	set_stocks_ib.touch.touch_flag=0;
	set_stocks_ib.touch.uid=0;
	set_stocks_ib.touch.press_callback=set_stocks_callback;
	set_stocks_ib.touch.uid=create_vicon_button(set_stocks_ib);

	set_settings_ib.x=215;
	set_settings_ib.y=190;
	set_settings_ib.button_text= "Settings";
	set_settings_ib.l=100;
	set_settings_ib.touch.enable=1;
	set_settings_ib.touch.touch_flag=0;
	set_settings_ib.touch.uid=0;
	set_settings_ib.touch.press_callback=set_settings_callback;
	set_settings_ib.touch.uid=create_vicon_button(set_settings_ib);
	
	
	vFrom_Screen_FilterSetting();	
	
	//display_settings_selection();
	
}

void display_settings_selection(void)
{
	
/*
	gfx_draw_circle( 80, 20, 10, GFX_COLOR_GREEN, GFX_WHOLE);
	gfx_draw_circle( 80, 50, 10, GFX_COLOR_GREEN, GFX_WHOLE);
	gfx_draw_circle( 80, 80, 10, GFX_COLOR_GREEN, GFX_WHOLE);
	gfx_draw_circle( 80, 110, 10, GFX_COLOR_GREEN, GFX_WHOLE);
*/

	/*if (disp_news_temp)
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
		draw_pressed_icon_button(set_stocks_ib);	
	else
		//gfx_draw_filled_circle( 80, 110, 5, GFX_COLOR_WHITE, GFX_WHOLE);
		draw_icon_button(set_stocks_ib);*/
	
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
		
		//gfx_draw_filled_rect(1,1,320,240,GFX_COLOR_WHITE);
		gfx_draw_bitmap(&LOGO9, 160-LOGO9.width/2,70-LOGO9.height/2);
		
		struct ui_text_box loading_text;
		loading_text.x=30;
		loading_text.y=110;
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
		/*struct gfx_bitmap LOGO9;
		LOGO9.width =150;
		LOGO9.height =94;
		LOGO9.type =GFX_BITMAP_PROGMEM;
		LOGO9.data.progmem = &logo9;
		
		gfx_draw_filled_rect(1,1,320,240,GFX_COLOR_WHITE);
		gfx_draw_bitmap(&LOGO9, 160-LOGO9.width/2,70-LOGO9.height/2);*/
		
		struct ui_text_box connecting_text;
		
		connecting_text.x=30;
		connecting_text.y=110;
		connecting_text.bg_color =GFX_COLOR_WHITE;
		connecting_text.text_color =GFX_COLOR_BLACK;
		connecting_text.box_color =GFX_COLOR_WHITE;
		connecting_text.l=260;
		connecting_text.b=40;
		connecting_text.text_align=TEXT_ALIGN_CENTER;
		connecting_text.text="                            Connecting...";
		connecting_text.font = &bigfont;
		ui_draw_text_box(connecting_text);
}
void vScreen_SettingButton()
{
	struct ui_text_box connecting_text;
		
		connecting_text.x=30;
		connecting_text.y=110;
		connecting_text.bg_color =GFX_COLOR_WHITE;
		connecting_text.text_color =GFX_COLOR_RED;
		connecting_text.box_color =GFX_COLOR_WHITE;
		connecting_text.l=260;
		connecting_text.b=40;
		connecting_text.text_align=TEXT_ALIGN_CENTER;
		connecting_text.text="                          Connection Failed...";
		connecting_text.font = &bigfont;
		ui_draw_text_box(connecting_text);
		
		set_settings_ib.x=215;
		set_settings_ib.y=190;
		set_settings_ib.button_text= "Settings";
		set_settings_ib.l=100;
		set_settings_ib.touch.enable=1;
		set_settings_ib.touch.touch_flag=0;
		set_settings_ib.touch.uid=0;
		set_settings_ib.touch.press_callback=set_settings_callback;
		set_settings_ib.touch.uid=create_vicon_button(set_settings_ib);
	
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
	draw_pressed_icon_button(settings_manual_wifi_ib);
	screen_wifi_connect();
}
void settings_smart_config_callback(int touch_uid)
{
	draw_pressed_icon_button(settings_smart_config_ib);
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
	settings_manual_wifi_ib.button_text= "   Go";
	settings_manual_wifi_ib.touch.enable=1;
	settings_manual_wifi_ib.touch.touch_flag=0;
	settings_manual_wifi_ib.touch.uid=0;
	settings_manual_wifi_ib.touch.press_callback=settings_manual_callback;
	settings_manual_wifi_ib.touch.uid=create_icon_button(settings_manual_wifi_ib);
	
	settings_smart_config_ib.x=220;
	settings_smart_config_ib.y=line_no(11);
	settings_smart_config_ib.button_text= "   Go";
	settings_smart_config_ib.touch.enable=1;
	settings_smart_config_ib.touch.touch_flag=0;
	settings_smart_config_ib.touch.uid=0;
	settings_smart_config_ib.touch.press_callback=settings_smart_config_callback;
	settings_smart_config_ib.touch.uid=create_icon_button(settings_smart_config_ib);
	
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
	_delay_ms(500);
	screen_settings();
}

void settings_view_all(void)
{
	draw_pressed_icon_button(settings_ib);
	_delay_ms(500);
	screen_connecting();
}
void settings_headlines(void)
{
	draw_pressed_icon_button(settings_ib);
	_delay_ms(500);
	screen_connecting();
}

void wifi_connect_callback(int touch_uid)
{
	click_on_credential_type(3); // provisioning AP
	reset_idle_timer();
	draw_pressed_icon_button(wifi_connect_ib);
	struct ui_progress_bar connecting_bar;
	connecting_bar.x = 60;
	connecting_bar.y = line_no(7);
	connecting_bar.l = 200;
	connecting_bar.b = 20;
	connecting_bar.bg_color = GFX_COLOR_WHITE;
	connecting_bar.fill_color = GFX_COLOR_BLUE;
	connecting_bar.percent=0;
	draw_progress_bar(connecting_bar);
	struct ui_text_box messagebar_t;
	messagebar_t.x=20;
	messagebar_t.y=line_no(9);
	messagebar_t.l= 280;
	messagebar_t.b= 40;
	messagebar_t.bg_color=GFX_COLOR_WHITE;
	messagebar_t.text_align= TEXT_ALIGN_LEFT;
	messagebar_t.text_color=GFX_COLOR_GREEN;
	messagebar_t.text="  ";
	messagebar_t.font= &sysfont;
	ui_draw_text_box(messagebar_t);
	vComplete_Progress_Bar(3,0,"Initializing...");
	vComplete_Progress_Bar(3,1,"Progressing...");
	
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
	click_on_credential_type(2); // smartConfig
	reset_idle_timer();
	draw_pressed_icon_button(wifi_connect_ib);
	struct ui_progress_bar connecting_bar;
	connecting_bar.x = 60;
	connecting_bar.y = line_no(7);
	connecting_bar.l = 200;
	connecting_bar.b = 20;
	connecting_bar.bg_color = GFX_COLOR_WHITE;
	connecting_bar.fill_color = GFX_COLOR_GREEN;
	connecting_bar.percent=0;
	draw_progress_bar(connecting_bar);
	struct ui_text_box messagebar_t;
	messagebar_t.x=20;
	messagebar_t.y=line_no(9);
	messagebar_t.l= 280;
	messagebar_t.b= 40;
	messagebar_t.bg_color=GFX_COLOR_WHITE;
	messagebar_t.text_align= TEXT_ALIGN_LEFT;
	messagebar_t.text_color=GFX_COLOR_GREEN;
	messagebar_t.text="  ";
	messagebar_t.font= &sysfont;
	ui_draw_text_box(messagebar_t);
	vComplete_Progress_Bar(2,0,"Initializing...");
	vComplete_Progress_Bar(2,1,"Progressing...");
}

void vComplete_Progress_Bar(uint8_t tuType,uint8_t tuCnt,char* Message)
{
	struct ui_progress_bar connecting_bar;
	connecting_bar.x = 60;
	connecting_bar.y = line_no(7);
	connecting_bar.l = 200;
	connecting_bar.b = 20;
	if(tuType == 2)
	{
		connecting_bar.fill_color = GFX_COLOR_GREEN;
	}
	else
	{
		connecting_bar.fill_color = GFX_COLOR_BLUE;
	}
		
	connecting_bar.bg_color = GFX_COLOR_WHITE;
	
	gfx_draw_string("                                            ",28,(line_no(10)-10),&sysfont,GFX_COLOR_WHITE,GFX_COLOR_GREEN);
	gfx_draw_string(Message,28,(line_no(10)-10),&sysfont,GFX_COLOR_WHITE,GFX_COLOR_GREEN);
	
	for (uint8_t i = (tuCnt*20);i<=((tuCnt*20)+19);i++)
	{
		connecting_bar.percent=i;
		draw_progress_bar(connecting_bar);
		_delay_ms(40);
	}
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

void screen_loading_content2(char * feedname)
{
	clr_scr();
	draw_battery_bar(100);
	gfx_draw_string("Loading updates for:",60, 40, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(feedname,70, 65, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	//gfx_draw_string("Sources",110, 80, &bigfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	vScreenLoadingBar(0);
	
}
void vScreenLoadingBar(uint8_t tuPercent)
{
	struct ui_progress_bar connecting_bar;
	connecting_bar.x = 60;
	connecting_bar.y = line_no(9);
	connecting_bar.l = 200;
	connecting_bar.b = 20;
	connecting_bar.bg_color = GFX_COLOR_WHITE;
	connecting_bar.fill_color = GFX_COLOR_RED;
	for (uint8_t i = (tuPercent*10);i<=((tuPercent*10)+9);i++)
	{
		connecting_bar.percent=i;
		draw_progress_bar(connecting_bar);
		_delay_ms(10);
	}
	
}

void screen_email(struct ui_email email, uint8_t screen_location)
{
	clr_scr();
	//draw_menu_bar();
	ffs_display_icon_from_flash(email.logo.data.start_sector, 5, 2);
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
void screen_displayingReconnecting(uint8_t Cnt)
{
	struct ui_text_box error_message_t;
	error_message_t.x = 0;
	error_message_t.y = 100;
	error_message_t.l = 320;
	error_message_t.b = 40;
	error_message_t.bg_color = GFX_COLOR_WHITE;
	error_message_t.text_align = TEXT_POS_CENTER_LEFT;
	error_message_t.text_color = GFX_COLOR_RED;
	error_message_t.font = &bigfont;
	
	switch (Cnt)
	{
		case 0 :
			error_message_t.text = "Connection Lost..Retrying.";
			ui_draw_text_box(error_message_t);
		break;
		
		case 1 :
			error_message_t.x = 260;
			error_message_t.text = "..";
			ui_draw_text_box(error_message_t);
		break;
		
		case 2 :
			error_message_t.x = 260;
			error_message_t.text = "....";
			ui_draw_text_box(error_message_t);
		break;
		
		case 3 :
			error_message_t.text = "                               ";
			ui_draw_text_box(error_message_t);
			error_message_t.text = " Connection SuccessFulll     ....";
			ui_draw_text_box(error_message_t);
		break;
		default :
		break;
	}
}
void screen_news(struct ui_news *news)
{
	clr_scr();
		
	gfx_draw_string(news->title,5, 4, &hack_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news->time_lapse,275, 5, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	struct ui_text_box news_message_t;
	news_message_t.x=53;
	news_message_t.y=19;
	//news_message_t.l= 261;
	news_message_t.l= 257;
	news_message_t.b= 90;
	news_message_t.bg_color=GFX_COLOR_WHITE;
	news_message_t.text_align= TEXT_ALIGN_LEFT;
	news_message_t.text_color=GFX_COLOR_BLACK;
	news_message_t.text= news->message;
	//news_message_t.text=title;
	news_message_t.font= &bitstream_vera_font;
	ui_draw_text_box(news_message_t);
	
	gfx_draw_filled_rect(3, 25, 50, 50, GFX_COLOR_WHITE);
	if (ffs_display_icon_from_flash(news->image,  3, 25)==-1)
	{
		if (ffs_display_icon_from_flash(143,  3, 25)==-1)
		{
			gfx_draw_filled_rect(3, 25, 50, 50, GFX_COLOR_RED);
		}
	}
	
	gfx_draw_horizontal_line(25,116,270,GFX_COLOR_CYAN);
		
}

void screen_news2(struct ui_news *news1)
{

	gfx_draw_string(news1->title,5, 124, &hack_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news1->time_lapse,275, 125, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	struct ui_text_box news_message_t1;
	news_message_t1.x=53;
	news_message_t1.y=139;
	//news_message_t1.l= 261;
	news_message_t1.l= 257;
	news_message_t1.b= 90;
	news_message_t1.bg_color=GFX_COLOR_WHITE;
	news_message_t1.text_align= TEXT_ALIGN_LEFT;
	news_message_t1.text_color=GFX_COLOR_BLACK;
	news_message_t1.text= news1->message;
	//news_message_t.text=title;
	news_message_t1.font= &bitstream_vera_font;
	ui_draw_text_box(news_message_t1);
	
	gfx_draw_filled_rect(3, 145, 50, 50, GFX_COLOR_WHITE);
	if (ffs_display_icon_from_flash(news1->image,  3, 145)==-1)
	{
		if (ffs_display_icon_from_flash(143,  3, 145)==-1)
		{
			gfx_draw_filled_rect(3, 145, 50, 50, GFX_COLOR_RED);
		}
	}
	
}

void screen_stocks(struct ui_news *news)
{
	clr_scr();
	
	gfx_draw_filled_rect(3, 25, 55, 55, GFX_COLOR_WHITE);
	gfx_draw_string(news->title,5, 4, &hack_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	if (news->image == 1) 
	{
		ui_draw_filled_triangle(10,75,50,75,30,45, GFX_COLOR_GREEN);
	}
	else if (news->image == 2) 
	{
			ui_draw_filled_triangle(10, 45, 50, 45, 30, 75, GFX_COLOR_RED);
	}
		
	gfx_draw_string(news->time_lapse,270, 5, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK); //price
	gfx_draw_string(news->away_team,15, 21, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); //prc chg
	gfx_draw_string(news->home_score,90, 21, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); //price
	gfx_draw_string(news->home_team,190, 21, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); // amt chg
	////gfx_draw_string(news.end_text,250, 40, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); // volume
		
	struct ui_text_box stocks_news1_t;
	stocks_news1_t.x=53;
	stocks_news1_t.y=38;
	//stocks_news1_t.l= 261;
	stocks_news1_t.l= 257;
	stocks_news1_t.b= 40;
	stocks_news1_t.bg_color=GFX_COLOR_WHITE;
	stocks_news1_t.text_align= TEXT_ALIGN_LEFT;
	stocks_news1_t.text_color=GFX_COLOR_BLACK;
	stocks_news1_t.text= news->stock_title1;
	stocks_news1_t.font= &bitstream_vera_font;
	ui_draw_text_box(stocks_news1_t);
	
	struct ui_text_box stocks_news2_t;
	stocks_news2_t.x=53;
	stocks_news2_t.y=76;
	//stocks_news2_t.l= 261;
	stocks_news2_t.l= 257;
	stocks_news2_t.b= 40;
	stocks_news2_t.bg_color=GFX_COLOR_WHITE;
	stocks_news2_t.text_align= TEXT_ALIGN_LEFT;
	stocks_news2_t.text_color=GFX_COLOR_BLACK;
	stocks_news2_t.text= news->stock_title2;
	stocks_news2_t.font = &bitstream_vera_font;
	ui_draw_text_box(stocks_news2_t);
	
	
	gfx_draw_horizontal_line(20,120,280,GFX_COLOR_CYAN);
}

void screen_stocks2(struct ui_news *news)
{
	gfx_draw_filled_rect(3, 145, 55, 55, GFX_COLOR_WHITE);
	gfx_draw_string(news->title,5, 124, &hack_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	if (news->image == 1) 
	{
		ui_draw_filled_triangle(10,195,50,195,30,165, GFX_COLOR_GREEN);
	}
	else if (news->image == 2) 
	{
		ui_draw_filled_triangle(10, 165, 50, 165, 30, 195, GFX_COLOR_RED);
	}
	
	gfx_draw_string(news->time_lapse,270, 125, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK); //price
	gfx_draw_string(news->away_team,20, 141, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); //prc chg
	gfx_draw_string(news->home_score,95, 141, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); //price
	gfx_draw_string(news->home_team,195, 141, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); // amt chg
	////gfx_draw_string(news.end_text,250, 40, &bold_ariel_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK); // volume
	
	struct ui_text_box stocks_news1_t;
	stocks_news1_t.x=53;
	stocks_news1_t.y=158;
	//stocks_news1_t.l= 261;
	stocks_news1_t.l= 257;
	stocks_news1_t.b= 40;
	stocks_news1_t.bg_color=GFX_COLOR_WHITE;
	stocks_news1_t.text_align= TEXT_ALIGN_LEFT;
	stocks_news1_t.text_color=GFX_COLOR_BLACK;
	stocks_news1_t.text= news->stock_title1;
	stocks_news1_t.font= &bitstream_vera_font;
	ui_draw_text_box(stocks_news1_t);
	
	struct ui_text_box stocks_news2_t;
	stocks_news2_t.x=53;
	stocks_news2_t.y=196;
	//stocks_news2_t.l= 261;
	stocks_news2_t.l= 257;
	stocks_news2_t.b= 40;
	stocks_news2_t.bg_color=GFX_COLOR_WHITE;
	stocks_news2_t.text_align= TEXT_ALIGN_LEFT;
	stocks_news2_t.text_color=GFX_COLOR_BLACK;
	stocks_news2_t.text= news->stock_title2;
	stocks_news2_t.font = &bitstream_vera_font;
	ui_draw_text_box(stocks_news2_t);
	
}


void screen_sports(struct ui_news *news)
{
	clr_scr();
		
	gfx_draw_string(news->title,5, 4, &hack_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	//gfx_draw_string(news.time_lapse,275, 4, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news->away_team,58, 25, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news->away_score,245, 25, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news->home_team,58, 40, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news->home_score,245, 40, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	//gfx_draw_string(news->end_text,65, 80, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	
	struct ui_text_box news_message_t1;
	news_message_t1.x=53;
	news_message_t1.y=55;
	//news_message_t1.l= 261;
	news_message_t1.l= 257;
	news_message_t1.b= 90;
	news_message_t1.bg_color=GFX_COLOR_WHITE;
	news_message_t1.text_align= TEXT_ALIGN_LEFT;
	news_message_t1.text_color=GFX_COLOR_BLACK;
	news_message_t1.text= news->end_text;
	news_message_t1.font= &bitstream_vera_font;
	ui_draw_text_box(news_message_t1);
	
	gfx_draw_filled_rect(3, 25, 55, 55, GFX_COLOR_WHITE);
	if (ffs_display_icon_from_flash(news->image,  3, 25)==-1)
	{
		if (ffs_display_icon_from_flash(49,  3, 145)==-1)
		{
			gfx_draw_filled_rect(3, 25, 50, 50, GFX_COLOR_RED);
		}
	}
	
	
	gfx_draw_horizontal_line(20,120,280,GFX_COLOR_CYAN);		
		
		
}



void screen_sports2(struct ui_news *news1)
{
	
	gfx_draw_string(news1->title,5, 124, &hack_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	//gfx_draw_string(news.time_lapse,275, 4, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news1->away_team,58, 145, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news1->away_score,245, 145, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news1->home_team,58, 160, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	gfx_draw_string(news1->home_score,245, 160, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	//gfx_draw_string(news1->end_text,65, 200, &bitstream_vera_font,GFX_COLOR_WHITE, GFX_COLOR_BLACK);
	
	struct ui_text_box news_message_t1;
	news_message_t1.x=53;
	news_message_t1.y=175;
	//news_message_t1.l= 261;
	news_message_t1.l= 257;
	news_message_t1.b= 90;
	news_message_t1.bg_color=GFX_COLOR_WHITE;
	news_message_t1.text_align= TEXT_ALIGN_LEFT;
	news_message_t1.text_color=GFX_COLOR_BLACK;
	news_message_t1.text= news1->end_text;
	news_message_t1.font= &bitstream_vera_font;
	ui_draw_text_box(news_message_t1);
	
	gfx_draw_filled_rect(3, 145, 55, 55, GFX_COLOR_WHITE);
	if (ffs_display_icon_from_flash(news1->image,  3, 145)==-1)
	{
		if (ffs_display_icon_from_flash(49,  3, 145)==-1)
		{
			gfx_draw_filled_rect(3, 145, 50, 50, GFX_COLOR_RED);
		}
	}
	
	
}


void screen_notification_triggered(void)
{
	clr_scr();
	draw_menu_bar();	
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




static int fs_start_file, fs_sector, fs_selected_file;
static struct ui_icon_button fs_left_ib , fs_right_ib, fs_up_ib, fs_down_ib;

void fs_left_ib_callback(int touch_uid)
{
	draw_pressed_vicon_button(fs_left_ib);
	if (fs_start_file > 9)
	{
		fs_start_file-=10;
		fs_selected_file=fs_start_file;
		ffs_display_file_table(fs_start_file,fs_start_file+9, fs_selected_file);
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
		fs_start_file+=10;
		end_file = fs_start_file+9;
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
	ffs_display_file_table(fs_start_file, fs_start_file+9, fs_selected_file);
	if (ffs_display_icon_from_flash(fs_selected_file+1,  120, 140)==-1)
	{
		gfx_draw_filled_rect(120, 140, 50, 50, GFX_COLOR_WHITE);
	}
	draw_vicon_button(fs_up_ib);
}
void fs_down_ib_callback(int touch_uid)
{
	draw_pressed_vicon_button(fs_down_ib);
	if (fs_selected_file<(fs_start_file+9))
	{
		fs_selected_file++;
	}
	ffs_display_file_table(fs_start_file, fs_start_file+9, fs_selected_file);
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
	ffs_display_file_table(fs_start_file,fs_start_file+9, fs_selected_file);
	
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