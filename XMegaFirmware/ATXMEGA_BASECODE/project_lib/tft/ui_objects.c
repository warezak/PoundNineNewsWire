/*
 * ui_objects.c
 *
 * Created: 6/13/2015 4:51:46 PM
 *  Author: ssenthil
 */ 


/************************************************************************/
/* 
1. Simple Button
2. Text input box - touch for keyboard

3. Text Display box - (no touch)
4. bmp Button
5. Drop down


keyboard:
-> collection of many buttons
-> return char[]
-> based on mode select each button will be taken as small/ captial letters/symbols1/symbols2

                                                                     */
/************************************************************************/

#include <string.h>
#include <asf.h>
#include "conf_ads7843.h"
#include "board.h"
#include "ioport.h"
#include "tft/common/services/resistive_touch/rtouch.h"
#include <util/delay.h>
#include "window_manager.h"
#include "ui_objects.h"

#include "button_blue.h"
#include "button_blue_pressed.h"
#include "Button_L.h"
#include "Button_M.h"
#include "Button_R.h"
#include "Button_P_L.h"
#include "Button_P_M.h"
#include "Button_P_R.h"

static	struct gfx_bitmap button_blue_b , button_l, button_r, button_m;

static uint8_t flash_buf[AT45DBX_SECTOR_SIZE]; //512 bytes


void ui_draw_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t color)
{
	gfx_generic_draw_line(x1, y1,x2,y2,color);
	gfx_generic_draw_line(x1, y1,x3,y3,color);
	gfx_generic_draw_line(x3, y3,x2,y2,color);
}

void ui_draw_filled_triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, int16_t color)
{
	gfx_generic_draw_line(x1, y1,x2,y2,color);
	gfx_generic_draw_line(x1, y1,x3,y3,color);
	gfx_generic_draw_line(x3, y3,x2,y2,color);
	
	float x, y;
	float slope, c;
	slope = (y2-y2)/(x2-x1);
//	c= y1-slope*x1;
	c= y2-slope*x2;
	
	for (x=x1; x<x2; x++)
	{
		y=slope*x+c;
		gfx_generic_draw_line(x, y,x3,y3,color);
	}
}



void gfx_draw_bitmap_from_flash(uint16_t image_sector, gfx_coord_t x, gfx_coord_t y)
{
ili9341_color_t flash_icon_buffer[0x900];
int i, j; 
uint8_t u;
struct gfx_bitmap DATA_FLASH_ICON;
DATA_FLASH_ICON.width =48;
DATA_FLASH_ICON.height =48;
DATA_FLASH_ICON.type =GFX_BITMAP_RAM;

for (u=0; u<9; u++)
{
	at45dbx_read_sector_open(image_sector+u);
	at45dbx_read_sector_to_ram(flash_buf);
	at45dbx_read_close();
	for(i=0, j=0;i<512;i+=2, j++) {
		flash_icon_buffer[(u*256) + j] = (flash_buf[i] << 8 ) | flash_buf[i+1];
	}
}
DATA_FLASH_ICON.data.pixmap= flash_icon_buffer;
gfx_draw_bitmap(&DATA_FLASH_ICON, x,y);
}



void ui_draw_text_box(struct ui_text_box tbox)
{
	gfx_draw_filled_rect(tbox.x		,tbox.y	,tbox.l,tbox.b, tbox.bg_color);
	//gfx_draw_rect(tbox.x		,tbox.y	,tbox.l,tbox.b, tbox.box_color);
	
	tbox.lines = (tbox.b / tbox.font->height);
	tbox.chars = tbox.l / tbox.font->width;
	uint8_t last_space_position =0, char_position =0, current_line_char_position=0, current_line=0;
	char current_char;
		
		do {
			
			current_char = *(tbox.text + char_position);
			/* Handle '\n' as newline, draw normal characters. */
			if (current_char == ' ') 
			{
				last_space_position = char_position;
			}
			if (current_char == '\n')
			{
				current_line++;
				current_line_char_position = 0;
			}
			if (current_line_char_position % tbox.chars == 0 && current_line_char_position != 0)
			{
				current_line++;
				*(tbox.text + last_space_position) = '\n';
				current_line_char_position = char_position - last_space_position;
			}
			if (current_line>= tbox.lines)
			{
				if (current_line_char_position>3)
				{
				*(tbox.text + last_space_position ) = '.';
				*(tbox.text + last_space_position +1) = '.';
				*(tbox.text + last_space_position +2) = '.';
				*(tbox.text + last_space_position +3) = '\0';
				break;
				}
				else
				{
					*(tbox.text + last_space_position + current_line_char_position -3) = '.';
					*(tbox.text + last_space_position + current_line_char_position -2) = '.';
					*(tbox.text + last_space_position + current_line_char_position -1) = '.';
					*(tbox.text + last_space_position + current_line_char_position) = '\0';
					break;
				}				
			}
			char_position++;
			current_line_char_position++;
					
		} while (current_char!= '\0');
	 //&bigfont
	gfx_draw_string_aligned(tbox.text, tbox.x+5, tbox.y+5, tbox.font,tbox.bg_color, tbox.text_color, TEXT_POS_LEFT, tbox.text_align);
}



//////////////////simple button///////////////////

void draw_button(struct ui_simple_button button)
{
	gfx_draw_filled_rect(button.x		,button.y	,button.l,button.b, button.button_color);
	gfx_draw_string(button.button_text,button.x+5, button.y+5, &bigfont,button.button_color, button.text_color);
}

void draw_pressed_button(struct ui_simple_button button)
{
	gfx_draw_filled_rect(button.x		,button.y	,button.l,button.b, GFX_COLOR_YELLOW);
	gfx_draw_string(button.button_text,button.x+5, button.y+5, &bigfont,GFX_COLOR_YELLOW, button.text_color);
}

int create_button(struct ui_simple_button button_b)
{
	draw_button(button_b);
	return(register_touch_object(button_b.touch));
}
///////////////////////////////////////////////////

//////////////////icon button///////////////////


void draw_icon_button(struct ui_icon_button button)
{
		button_blue_b.width =85; // will be passed
		button_blue_b.height =30;
		button_blue_b.type =GFX_BITMAP_PROGMEM;
		button_blue_b.data.progmem = &button_blue;
		gfx_draw_bitmap(&button_blue_b, button.x,button.y);
	gfx_draw_string(button.button_text,button.x+5, button.y+5, &bigfont,GFX_COLOR_TRANSPARENT, GFX_COLOR_WHITE);
}

void draw_pressed_icon_button(struct ui_icon_button button)
{
			button_blue_b.width =85; 
			button_blue_b.height =30;
			button_blue_b.type =GFX_BITMAP_PROGMEM;
			button_blue_b.data.progmem = &button_blue_pressed;
			gfx_draw_bitmap(&button_blue_b, button.x,button.y);
			
	gfx_draw_string(button.button_text,button.x+5, button.y+5, &bigfont,GFX_COLOR_TRANSPARENT, GFX_COLOR_WHITE);

}


int create_icon_button(struct ui_icon_button button_b)
{
	draw_icon_button(button_b);
	button_b.touch.x=button_b.x;
	button_b.touch.y=button_b.y;
	button_b.touch.l=85;
	button_b.touch.b=30;

	return(register_touch_object(button_b.touch));
}

///////////////////////////////////////////////////

//// variable length icon button

void draw_vicon_button(struct ui_icon_button button)
{
	uint8_t i=0, diff=0, num_p=0;
		
	button_l.width =17;
	button_l.height =30;
	button_l.type =GFX_BITMAP_PROGMEM;
	button_l.data.progmem = &Button_L;

	button_m.width =3;
	button_m.height =30;
	button_m.type =GFX_BITMAP_PROGMEM;
	button_m.data.progmem = &Button_M;

	button_r.width =17; 
	button_r.height =30;
	button_r.type =GFX_BITMAP_PROGMEM;
	button_r.data.progmem = &Button_R;

	if (button.l>34)
	{
		diff = button.l-34;
		num_p= 1+(diff/3);
		gfx_draw_bitmap(&button_l, button.x,button.y);
		gfx_draw_bitmap(&button_r, button.x + button.l - 17,button.y);
		for (i=0;i<num_p;i++)
		{
			gfx_draw_bitmap(&button_m, button.x +17 +i*3,button.y);
		}
	}
	else
	{
		gfx_draw_bitmap(&button_l, button.x,button.y);
		gfx_draw_bitmap(&button_r, button.x + button.l - 17,button.y);
	}
	gfx_draw_string(button.button_text,button.x+5, button.y+5, &bigfont,GFX_COLOR_TRANSPARENT, GFX_COLOR_WHITE);
}

void draw_pressed_vicon_button(struct ui_icon_button button)
{
	uint8_t i=0, diff=0, num_p=0;
	
	button_l.width =17;
	button_l.height =30;
	button_l.type =GFX_BITMAP_PROGMEM;
	button_l.data.progmem = &Button_P_L;

	button_m.width =3;
	button_m.height =30;
	button_m.type =GFX_BITMAP_PROGMEM;
	button_m.data.progmem = &Button_P_M;

	button_r.width =17;
	button_r.height =30;
	button_r.type =GFX_BITMAP_PROGMEM;
	button_r.data.progmem = &Button_P_R;
	
	if (button.l>34)
	{
			diff = button.l-34;
			num_p= 1+(diff/3);
			gfx_draw_bitmap(&button_l, button.x,button.y);
			gfx_draw_bitmap(&button_r, button.x + button.l - 17,button.y);	
			for (i=0;i<num_p;i++)
			{
				gfx_draw_bitmap(&button_m, button.x +17 +i*3,button.y);
			}
	}
	else 	
	{
			gfx_draw_bitmap(&button_l, button.x,button.y);
			gfx_draw_bitmap(&button_r, button.x + button.l - 17,button.y);
	}
	gfx_draw_string(button.button_text,button.x+5, button.y+5, &bigfont,GFX_COLOR_TRANSPARENT, GFX_COLOR_WHITE);

}


int create_vicon_button(struct ui_icon_button button_b)
{
	draw_vicon_button(button_b);
	button_b.touch.x=button_b.x;
	button_b.touch.y=button_b.y;
//	button_b.touch.l=85;
	button_b.touch.l=button_b.l;
	button_b.touch.b=30;
	return(register_touch_object(button_b.touch));
}

///////////////////////////////////////////////////


void draw_progress_bar(struct ui_progress_bar p_bar)
{
	float x_step , current_x;
	if (p_bar.l>=100)
	{
		x_step = p_bar.l/100;
	}
	else
		x_step = 1;

	if (p_bar.percent == 0)
	{
		gfx_draw_filled_rect(p_bar.x		,p_bar.y	,p_bar.l,p_bar.b, p_bar.bg_color);
		gfx_draw_rect(p_bar.x-1		,p_bar.y-1	,p_bar.l+1,p_bar.b+1, GFX_COLOR_BLACK);
	}
	for (uint8_t i=0; i<=p_bar.percent; i++)
	{
		current_x= p_bar.x + i*x_step;
		gfx_draw_filled_rect(current_x,p_bar.y	,2,p_bar.b-1, p_bar.fill_color);
	}
}


void draw_battery_bar(uint8_t battery_percent)
{
	gfx_draw_filled_rect(250,10,40,20, GFX_COLOR_WHITE);
	gfx_draw_rect		(250,10,40,20, GFX_COLOR_GREEN);
	gfx_draw_rect		(251,11,38,18, GFX_COLOR_GREEN);	
	gfx_draw_filled_rect(290,15,5,10, GFX_COLOR_GREEN);
	if (battery_percent>100)
	battery_percent=100;
	for(int8_t i=0; i<battery_percent/25;i++)
	{
		gfx_draw_filled_rect(256+i*8,13,4,14, GFX_COLOR_GREEN);		
	}
}


/*
example: drop down selector:
-> register as many touch objects as the number of drop down options.
-> initially enable only the drop down button.
-> in the drop down button call back, display the other options and enable the other buttons.
-> ISSUE: any earlier enabled button below the newly drop down region will be affected.
:: for now a drp down list should not over lap with any other object.
=> full screen drop down is the simplest option


-> after selection of the drop down, redraw the window with the updated values of all the objects


-> keyboard input will always be a new window
*/

#define MAX_DROPDOWN_OBJECTS 4
static struct ui_dropdown dropdown_object_array[MAX_DROPDOWN_OBJECTS];

static uint8_t dd_touch_no[20], dd_count=0, active_dd=0;


void dropdown_select_callback(int touch_uid)
{
	dropdown_object_array[active_dd].select_callback(touch_uid);
}

void dropdown_edit_callback(int touch_uid)
{
	active_dd = dd_touch_no[touch_uid];
	unregister_all_touch_objects();
	uint8_t i;
	for (i=0;i<dropdown_object_array[active_dd].num_options;i++)
	{
		dropdown_object_array[active_dd].option_b[i].touch.uid		=create_button(dropdown_object_array[active_dd].option_b[i]);
	}	
}

void unregister_all_dd(void)
{
	dd_count=0;
	for (uint8_t i=0; i<20 ;i++)
	{
		dd_touch_no[i] = 0;	
	}
}

int create_dropdown(struct ui_dropdown dropdown)
{
	struct ui_text_box dd_tb;
	dd_tb.x = dropdown.x;
	dd_tb.y = dropdown.y;
	dd_tb.l = dropdown.l;
	dd_tb.b = dropdown.b;
	dd_tb.bg_color=GFX_COLOR_WHITE;
	dd_tb.text_align= TEXT_ALIGN_LEFT;
	dd_tb.text_color=GFX_COLOR_BLACK;
	dd_tb.font= &bigfont;		
	dd_tb.text = dropdown.option_b[dropdown.selected_option].button_text;
	ui_draw_text_box(dd_tb);
	
	dropdown.select_b.x						=dropdown.x + dropdown.l;
	dropdown.select_b.y						=dropdown.y;
	dropdown.select_b.l						=25;
	dropdown.select_b.b						=dropdown.b;
	
	dropdown.select_b.touch.x				=dropdown.x + dropdown.l;
	dropdown.select_b.touch.y				=dropdown.y;
	dropdown.select_b.touch.l				=25;
	dropdown.select_b.touch.b				=dropdown.b;
	dropdown.select_b.touch.enable			=1;	
		
	dropdown.select_b.button_color			=dropdown.button_color;
	dropdown.select_b.button_text			="V";
	dropdown.select_b.text_color			=dropdown.text_color;
	dropdown.select_b.touch.press_callback	= dropdown_edit_callback;
	dropdown.select_b.touch.uid				=create_button(dropdown.select_b);
	dd_touch_no[dropdown.select_b.touch.uid] = dd_count;
	dd_count++;
	
	dropdown.option_b[0].touch.press_callback	= dropdown_select_callback;
	dropdown.option_b[1].touch.press_callback	= dropdown_select_callback;
	dropdown.option_b[2].touch.press_callback	= dropdown_select_callback;
				
	memcpy( &dropdown_object_array[dd_touch_no[dropdown.select_b.touch.uid]], &dropdown, sizeof(dropdown));
		return 0;
}


void gfx_draw_int(int number, gfx_coord_t x, gfx_coord_t y,
const struct font *font, const gfx_color_t bg_color,
const gfx_color_t text_color)
{
	char num_buffer[15];
	itoa(number, num_buffer,10);
	gfx_draw_string(num_buffer,x, y, font,bg_color, text_color);
}