/* mainbattery.c
*
*  Display battery indicator on MainScreen
*
* Copyright 2007 by Vladimir Ananiev (Vovan888 at gmail com )
*
* Licensed under GPLv2, see LICENSE
*/
#include <flphone_config.h>

#include <stdio.h>
#include <stdarg.h>

#include <flphone/theme.h>

#include <nxcolors.h>
#include "indicators.h"

#define CHARGING_ANIMATE_PERIOD	  1500

static GR_IMAGE_ID battery_image;
static int battery_frame_width;
static int battery_frame_height;
static int battery_current = 0;
 /*FIXME*/ static int xcoord, ycoord;	/* XY coordinates of indicator */
#define MAINBATTERY_NUMFRAMES	6
static GR_TIMER_ID timerid;

static int mainbattery_show(int frame);

static void mainbattery_changed_callback(int new_value)
{
	int chargelevel = shdata->Battery.ChargeLevel;
	DBGMSG("%d\n", chargelevel);

	if (battery_current != chargelevel) {
		battery_current = chargelevel;
		mainbattery_show(battery_current);
	}
}

static void mainbattery_event_callback(GR_WINDOW_ID window, GR_EVENT * event)
{
	DBGMSG("\n");
	battery_current = shdata->Battery.ChargeLevel;

	mainbattery_show(battery_current);
}

void mainbattery_charging_timercallback(void)
{
	battery_current++;
	if (battery_current > 5)
		battery_current = 0;

	mainbattery_show(battery_current);
}

/* enable/disable charging state indication */
void mainbattery_charging(int state)
{
	static int current_state = 0;

	if (current_state == state)
		return;
	current_state = state;

	if(state) {
		/* start charging indication */
		timerid = GrCreateTimer(GR_ROOT_WINDOW_ID, CHARGING_ANIMATE_PERIOD);
	} else {
		/* stop indication */
		GrDestroyTimer(timerid);

		battery_current = shdata->Battery.ChargeLevel;
		mainbattery_show(battery_current);
	}
}

/* create mainbattery indicator */
int mainbattery_create(struct indicator *ind)
{
	GR_IMAGE_INFO iinfo;

	/* Get the image from theme */
	int ret = theme_get_image(THEME_GROUP_MAINSCREEN, THEME_MAINBATTERY,
				  &xcoord, &ycoord, &battery_image);

	if (ret == -1)
		return -1;

	if (ret != -2) {
		GrGetImageInfo(battery_image, &iinfo);
		battery_frame_width = iinfo.width / MAINBATTERY_NUMFRAMES;
		battery_frame_height = iinfo.height;
	}
	/* show indicator */
	mainbattery_show(1);
	/* set the callback */
	ind->callback = &mainbattery_event_callback;
	ind->changed = &mainbattery_changed_callback;
	ind->wind_id = GR_ROOT_WINDOW_ID;

	return 0;
}

/* show current battery state */
static int mainbattery_show(int frame)
{
	/*TODO - check if the root window is displayed, or not */

	if (frame >= MAINBATTERY_NUMFRAMES)
		frame = MAINBATTERY_NUMFRAMES - 1;
	else if (frame < 0)
		frame = 0;

	/* draw frame (part) of the image */
	/* clear the area under the indicator to background pixmap */
	GrClearArea(GR_ROOT_WINDOW_ID, xcoord, ycoord,
		    battery_frame_width, battery_frame_height, 0);
	/* Draw the indicator */
	GrDrawImagePartToFit(GR_ROOT_WINDOW_ID, gc, xcoord, ycoord,
			     battery_frame_width, battery_frame_height,
			     battery_frame_width * frame, 0,
			     battery_frame_width, battery_frame_height, battery_image);
	return 0;
}
