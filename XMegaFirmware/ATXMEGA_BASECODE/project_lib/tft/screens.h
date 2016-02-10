/*
 * screens.h
 *
 * Created: 6/18/2015 7:46:51 AM
 *  Author: ssenthil
 */ 


#ifndef SCREENS_H_
#define SCREENS_H_
#ifdef __cplusplus
extern "C"
{
#endif

#include "ui_objects.h"
#include "flash_fs.h"

struct menu_button_t{
	char * button_text;
	void (*press_callback) (uint8_t);
};

void screen_loading(void);
void screen_connecting(void);
void vScreen_SettingButton();
void screen_settings(void);
void screen_wifi_connect(void);
void screen_wifi_smart_config(void);
void screen_loading_content(void);
void screen_loading_content2(char * feedname);
void screen_email(struct ui_email, uint8_t screen_location);
void screen_news(struct ui_news *);
void screen_news2(struct ui_news *);
void screen_stocks(struct ui_news *);
void screen_stocks2(struct ui_news *);
void screen_sports(struct ui_news *);
void screen_sports2(struct ui_news *);
void screen_notification_triggered(void);
void screen_displayingReconnecting(uint8_t Cnt);
void clr_scr(void);
void draw_menu_bar(void);
void menu3_callback(int touch_uid);
void menu2_callback(int touch_uid);
void menu1_callback(int touch_uid);
void right_callback(int touch_uid);
void left_callback(int touch_uid);
void init_menus(struct menu_button_t my_menu[], uint8_t num_buttons);
void clr_window(void);
void draw_menu_selection(uint8_t button_no);

// functions in main
void click_on_credential_type(uint8_t abc);
void vComplete_Progress_Bar(uint8_t tuType,uint8_t tuCnt,char* Message);
void vScreenLoadingBar(uint8_t tuPercent);
void vFilterProcess(char* filterName);
void vFrom_Screen_FilterSetting(void);
void go_home(void);
void reset_idle_timer(void);
void wifi_smart_connect_callback(int touch_uid);
void wifi_connect_callback(int touch_uid);
void speed_dd_callback(int selected_option);
void settings_save_callback(int touch_uid);
void settings_manual_callback(int touch_uid);
void homepage_dd_callback(int selected_option);
void home_callback(int touch_uid);
void settings_smart_config_callback(int touch_uid);
void settings_callback(void);
void trigger_notification_callback(int touch_uid);
void screen_explorer(void);
void register_filter_callback(void);
#define line_no(x) (x*16)
void screen_filter(int touch_id_i);
void init_content(void);
void display_settings_selection(void);
void set_email_status(int email_disp_status);

void settings_view_all(void);
void settings_headlines(void);

#ifdef __cplusplus
}
#endif

#endif /* SCREENS_H_ */