/*
 * ui_objects.h
 *
 * Created: 6/13/2015 4:52:06 PM
 *  Author: ssenthil
 */ 


#ifndef UI_OBJECTS_H_
#define UI_OBJECTS_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "window_manager.h"



struct touch_object_t {
	int uid; // a unique number for an object only within each window. should never exceed the MAX_touch objects per window
	int enable; // shall be used to temporarily disable a touch object
	// touch coordinates / Area
	int16_t x;
	int16_t y;
	int16_t l;
	int16_t b;
	bool touch_flag;
	void (*press_callback) (int);
};



struct ui_simple_button {
	
	// button coordinates
	int16_t x;
	int16_t y;
	// button size
	int16_t l;
	int16_t b;
	
	uint16_t button_color;
	uint16_t text_color;
	
	// button text
	char * button_text;
	const struct font *font;
	struct touch_object_t touch;
	
};

struct ui_icon_button {
	
	// button coordinates
	int16_t x;
	int16_t y;

	int16_t l;
	// button text
	char * button_text;
	const struct font *font;
	struct touch_object_t touch;
	const struct gfx_bitmap *icon;
};

struct ui_progress_bar {
	
	// button coordinates
	int16_t x;
	int16_t y;
	// button size
	int16_t l;
	int16_t b;
	
	uint16_t fill_color;
	uint16_t bg_color;
	
	uint8_t percent;	
};
#define MAX_DROPDOWN_BUTTONS 5

struct ui_dropdown {
	int dd_uid;	
	uint8_t num_options;
	struct	ui_simple_button select_b;
	struct	ui_simple_button option_b[MAX_DROPDOWN_BUTTONS];
	uint8_t selected_option; 

	// button coordinates
	int16_t x;
	int16_t y;
	// button size
	int16_t l;
	int16_t b;
	
	uint16_t button_color;
	uint16_t text_color;	
	void (*select_callback) (int);	
};

struct ui_text_box {
	
	// box coordinates
	int16_t x;
	int16_t y;
	// box size
	int16_t l;
	int16_t b;
	
	int8_t lines;
	int8_t chars;
	
	uint16_t box_color;
	uint16_t bg_color;	
	uint16_t text_color;
	
	enum gfx_text_alignment text_align;
	// button text
	char * text;
	struct font *font;
};

struct ui_news {
	//struct gfx_bitmap logo;
	//char * title;
	//char * time_lapse;
	//char * message;
	char title[30];
	char time_lapse[30];
	char message[260];
	char format[10];
	int image;
	char away_team[25];
	char away_score[10];
	char home_team[25];
	char home_score[10];	
	char end_text[30];
	char stock_title1[60];
	char stock_title2[60];
	
				/*<F>		feed source			</F>
				<I>		image				</I>
				<H>		heading		 		</H>
				<T>		time				</T>
				<U>		url					</U>
				<C>		content				</C>
				<P1>	stock news title 1 	</P1>
				<PT1>  	stock news time 1   </PT1>
				<PU1> 	stock news url 1  	</PU1>
				<P2> 	stock news title 1 	</P2>
				<PT2>  	stock news time 1   </PT2>
				<PU2> 	stock news url 1  	</PU2>
				<P3> 	stock news title 1 	</P3>
				<PT3>  	stock news time 1   </PT3>
				<PU3> 	stock news url 1  	</PU3>
				<T1>	away team			</T1>
				<S1>	away score			</S1>
				<T2>	home team			</T2>
				<S2>	home score			</S2>
				<E>		end text			</E>
				</N>*/
};

struct ui_email {
	struct gfx_bitmap logo;
	char * from;
	char * time_lapse;
	char * subject;
	char * message;
};

struct ui_stocks {
	struct gfx_bitmap logo;
	char * title;
	char * time_lapse;
	uint8_t up_down;
	char * ticker;
	char * change;
	char * percent_change;
	char * news1;
	char * news2;
};

struct ui_sports {
	struct gfx_bitmap logo;
	char * title;
	char * team1_name;
	char * team1_score;
	char * team2_name;
	char * team2_score;
	char * game_status;
};

void ui_draw_text_box(struct ui_text_box tbox);
void draw_button(struct ui_simple_button button);
void draw_pressed_button(struct ui_simple_button button);
int create_button(struct ui_simple_button button_b);

void draw_icon_button(struct ui_icon_button button);
void draw_pressed_icon_button(struct ui_icon_button button);
int create_icon_button(struct ui_icon_button button_b);

void draw_vicon_button(struct ui_icon_button button);
void draw_pressed_vicon_button(struct ui_icon_button button);
int  create_vicon_button(struct ui_icon_button button_b);

void dropdown_edit_callback(int touch_uid);
int create_dropdown(struct ui_dropdown dropdown);
void draw_progress_bar(struct ui_progress_bar p_bar);
void draw_battery_bar(uint8_t battery_percent);
void ui_draw_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t color);
void unregister_all_dd(void);
void gfx_draw_bitmap_from_flash(uint16_t image_sector, gfx_coord_t x, gfx_coord_t y);
void ui_draw_filled_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t color);
void dropdown_select_callback(int touch_uid);
void gfx_draw_int(int number, gfx_coord_t x, gfx_coord_t y,const struct font *font, const gfx_color_t bg_color,const gfx_color_t text_color);

#ifdef __cplusplus
}
#endif
#endif /* UI_OBJECTS_H_ */