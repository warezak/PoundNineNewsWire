/*
 * window_manager.c
 *
 * Created: 6/13/2015 3:29:46 PM
 *  Author: ssenthil
 */ 

/*
This file takes care of both the display and touch function.
Touch callback will come to this file and will be used here based on the current window

All display outputs will go from this file
*/

#include <asf.h>
#include "conf_ads7843.h"
#include "board.h"
#include "ioport.h"
#include "tft/common/services/resistive_touch/rtouch.h"
#include <util/delay.h>
#include "window_manager.h"
#include "ui_objects.h"
#include "screens.h"


#define MAX_TOUCH_OBJECTS 80
static struct touch_object_t touch_object_array[MAX_TOUCH_OBJECTS];

static uint8_t registerd_touch_objects=0;

void WM_touch_callback(Point_t t_point)
{
	uint8_t i;
	t_point.x = 320 - t_point.x;
	t_point.y = 240 - t_point.y;

// touch point will be checked will all active touch object registered in a window
		for (i=0;i<MAX_TOUCH_OBJECTS && i<registerd_touch_objects; i++)
		{
			if (touch_object_array[i].enable)
			{
				if ((t_point.x>touch_object_array[i].x) && (t_point.x < (touch_object_array[i].x + touch_object_array[i].l)) && (t_point.y>touch_object_array[i].y) && (t_point.y < (touch_object_array[i].y + touch_object_array[i].b)) )
				{
					touch_object_array[i].press_callback(touch_object_array[i].uid);
					break;
				}
			}
		}
}


uint8_t register_touch_object(struct touch_object_t t_object)
{
	if (registerd_touch_objects < MAX_TOUCH_OBJECTS -1)
	{
//			t_object.uid									=registerd_touch_objects;
			touch_object_array[registerd_touch_objects].enable			=t_object.enable;
			touch_object_array[registerd_touch_objects].press_callback	=t_object.press_callback;
			touch_object_array[registerd_touch_objects].x				=t_object.x;
			touch_object_array[registerd_touch_objects].y				=t_object.y;
			touch_object_array[registerd_touch_objects].l				=t_object.l;
			touch_object_array[registerd_touch_objects].b				=t_object.b;
			touch_object_array[registerd_touch_objects].uid				=registerd_touch_objects;
			registerd_touch_objects++; // this will be cleared while starting a new window
/*
						itoa(registerd_touch_objects, buffer,10);
						gfx_draw_string("   ",90, 10, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
						gfx_draw_string(buffer,90, 10, &sysfont,GFX_COLOR_WHITE, GFX_COLOR_RED);
						_delay_ms(300);
*/
			return(registerd_touch_objects-1);
	}
	else
	return(0);
	
}

uint8_t unregister_touch_object(struct touch_object_t t_object)
{
		touch_object_array[t_object.uid].enable			=0;
		touch_object_array[t_object.uid].press_callback	=default_touch_callback;
		touch_object_array[t_object.uid].x				=NULL;
		touch_object_array[t_object.uid].y				=NULL;
		touch_object_array[t_object.uid].l				=NULL;
		touch_object_array[t_object.uid].b				=NULL;
		return(1);
}

uint8_t unregister_all_touch_objects()
{
	registerd_touch_objects=0;
	touch_object_array[0].enable			=0;
	touch_object_array[0].press_callback	=default_touch_callback;
	touch_object_array[0].x					=NULL;
	touch_object_array[0].y					=NULL;
	touch_object_array[0].l					=NULL;
	touch_object_array[0].b					=NULL;
	return(1);
}

void default_touch_callback(void)
{
	//nop
}

void enable_touch_object(struct touch_object_t t_object) // object should already be registered in current window
{
	touch_object_array[t_object.uid].enable=1;
}
void disable_touch_object(struct touch_object_t t_object) // object should already be registered in current window
{
	touch_object_array[t_object.uid].enable=0;
}


