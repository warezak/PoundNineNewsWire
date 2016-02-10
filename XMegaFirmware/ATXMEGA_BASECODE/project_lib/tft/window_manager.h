/*
 * window_manager.h
 *
 * Created: 6/13/2015 3:33:40 PM
 *  Author: ssenthil
 */ 


#ifndef WINDOW_MANAGER_H_
#define WINDOW_MANAGER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "ui_objects.h"

/*
struct touch_object_t {
	int uid; // a unique number for an object only within each window. should never exceed the MAX_touch objects per window
	int enable; // shall be used to temporarily disable a touch object 
	// touch coordinates / Area
	int16_t x;
	int16_t y;
	int16_t l;
	int16_t b;
	void (*press_callback) ();
};
*/


void WM_touch_callback(Point_t t_point);
uint8_t register_touch_object(struct touch_object_t t_object);
uint8_t unregister_touch_object(struct touch_object_t t_object);
void default_touch_callback(void);
void enable_touch_object(struct touch_object_t t_object) /* object should already be registered in current window */;
void disable_touch_object(struct touch_object_t t_object) /* object should already be registered in current window */;
void create_keypad(void);
uint8_t unregister_all_touch_objects(void);

#ifdef __cplusplus
}
#endif
#endif /* WINDOW_MANAGER_H_ */
