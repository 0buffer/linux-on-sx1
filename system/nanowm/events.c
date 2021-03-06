/*
 * NanoWM - Window Manager for Nano-X
 *
 * Copyright (C) 2000, 2003 Greg Haerr <greg@censoft.com>
 * Copyright (C) 2000 Alex Holden <alex@linuxhacker.org>
 */
#include <stdio.h>
#include <stdlib.h>
#define MWINCLUDECOLORS
#include <nano-X.h>

#include "nanowm.h"
#include <ipc/tbus.h>

void do_exposure(GR_EVENT_EXPOSURE * event)
{
	win *window;

	Dprintf("do_exposure: wid %d, x %d, y %d, width %d, height %d\n",
		event->wid, event->x, event->y, event->width, event->height);

	/* shmem stuff */
	shdata->WM.top_active_window = event->wid;

	if (!(window = find_window(event->wid)))
		return;

	switch (window->type) {
	case WINDOW_TYPE_CONTAINER:
		container_exposure(window, event);
		break;
	default:
		Dprintf("Unhandled exposure on window %d " "(type %d)\n", window->wid, window->type);
		break;
	}

}

void do_button_down(GR_EVENT_BUTTON * event)
{
	win *window;

	Dprintf("do_button_down: wid %d, subwid %d, rootx %d, rooty %d, x %d, "
		"y %d, buttons %d, changebuttons %d, modifiers %d\n",
		event->wid, event->subwid, event->rootx, event->rooty, event->x,
		event->y, event->buttons, event->changebuttons, event->modifiers);

	if (!(window = find_window(event->wid)))
		return;

	switch (window->type) {
	case WINDOW_TYPE_CONTAINER:
		container_buttondown(window, event);
		break;
	default:
		Dprintf("Unhandled button down on window %d "
		       "(type %d)\n", window->wid, window->type);
		break;
	}
}

void do_button_up(GR_EVENT_BUTTON * event)
{
	win *window;

	Dprintf("do_button_up: wid %d, subwid %d, rootx %d, rooty %d, x %d, "
		"y %d, buttons %d, changebuttons %d, modifiers %d\n",
		event->wid, event->subwid, event->rootx, event->rooty, event->x,
		event->y, event->buttons, event->changebuttons, event->modifiers);

	if (!(window = find_window(event->wid)))
		return;

	switch (window->type) {
	case WINDOW_TYPE_CONTAINER:
		container_buttonup(window, event);
		break;
	default:
		Dprintf("Unhandled button up on window %d "
		       "(type %d)\n", window->wid, window->type);
		break;
	}
}

void do_mouse_enter(GR_EVENT_GENERAL * event)
{
	win *window;

	Dprintf("do_mouse_enter: wid %d\n", event->wid);

	if (!(window = find_window(event->wid)))
		return;

	switch (window->type) {
	default:
		Dprintf("Unhandled mouse enter from window %d "
		       "(type %d)\n", window->wid, window->type);
		break;
	}
}

void do_mouse_exit(GR_EVENT_GENERAL * event)
{
	win *window;

	Dprintf("do_mouse_exit: wid %d\n", event->wid);

	if (!(window = find_window(event->wid)))
		return;

	switch (window->type) {
	default:
		Dprintf("Unhandled mouse exit from window %d "
		       "(type %d)\n", window->wid, window->type);
		break;
	}
}

void do_mouse_moved(GR_EVENT_MOUSE * event)
{
	win *window;

/*	Dprintf("do_mouse_moved: wid %d, subwid %d, rootx %d, rooty %d, x %d, "
		"y %d, buttons %d, modifiers %d\n", event->wid, event->subwid,
		event->rootx, event->rooty, event->x, event->y, event->buttons,
		event->modifiers);
*/
	if (!(window = find_window(event->wid)))
		return;

	switch (window->type) {
	case WINDOW_TYPE_CONTAINER:
		container_mousemoved(window, event);
		break;
	default:
		Dprintf("Unhandled mouse movement in window %d "
		       "(type %d)\n", window->wid, window->type);
		break;
	}
}

void do_focus_in(GR_EVENT_GENERAL * event)
{
	win *window;

	Dprintf("do_focus_in: wid %d\n", event->wid);

	if (!(window = find_window(event->wid)))
		return;

	switch (window->type) {
	default:
		Dprintf("Unhandled focus in from window %d "
		       "(type %d)\n", window->wid, window->type);
		break;
	}
}

void do_key_down(GR_EVENT_KEYSTROKE * event)
{
	Dprintf("do_key_down: wid %d, subwid %d, rootx %d, rooty %d, x %d, "
		"y %d, buttons %d, modifiers %d, ch %u, scancode %d\n",
		event->wid, event->subwid, event->rootx,
		event->rooty, event->x, event->y, event->buttons,
		event->modifiers, event->ch, event->scancode);

	/* DEBUG keys */
/*	if (event->ch == Key_Menu) {
		int ret = 888;
		char *str = "Key_Menu";
		ret = tbus_emit_signal("debugkey", "is", &ret, &str);
	}*/
	/* Handle keyboard shortcuts */
	handle_key_up(event);
}

void do_key_up(GR_EVENT_KEYSTROKE * event)
{
	Dprintf("do_key_up: wid %d, subwid %d, rootx %d, rooty %d, x %d, "
		"y %d, buttons %d, modifiers %d, ch %u, scancode %d\n",
		event->wid, event->subwid, event->rootx,
		event->rooty, event->x, event->y, event->buttons,
		event->modifiers, event->ch, event->scancode);

}

void do_update(GR_EVENT_UPDATE * event)
{
	win *window, *zwin;

	Dprintf("do_update: wid %d, subwid %d, x %d, y %d, width %d, height %d, "
		"utype %d\n", event->wid, event->subwid, event->x, event->y, event->width,
		event->height, event->utype);

	if (!(window = find_window(event->subwid))) {
		if (event->utype == GR_UPDATE_MAP)
			new_client_window(event->subwid);
		else if(event->utype == GR_UPDATE_DESTROY) {
			/* some other client destroyed
			 raise top level window */
			zwin = get_top_window();
			if (zwin != NULL)
				container_show(zwin);

		}
		return;
	}

	if (window->type == WINDOW_TYPE_CONTAINER) {
		if (event->utype == GR_UPDATE_ACTIVATE)
			redraw_ncarea(window);
		return;
	} else
	if (window->type == WINDOW_TYPE_CLIENT) {
		if (event->utype == GR_UPDATE_MAP)
			client_window_remap(window);
		if (event->utype == GR_UPDATE_DESTROY)
			client_window_destroy(window);
		if (event->utype == GR_UPDATE_UNMAP)
			client_window_unmap(window);
		if (event->utype == GR_UPDATE_SIZE)
			client_window_resize(window);
	} else
	if (window->type == WINDOW_TYPE_APP) {
		if (event->utype == GR_UPDATE_MAP)
			remap_app_window(window);
		if (event->utype == GR_UPDATE_DESTROY)
			destroy_app_window(window);
		if (event->utype == GR_UPDATE_UNMAP)
			unmap_app_window(window);
// 		if (event->utype == GR_UPDATE_SIZE)
// 			resize_app_window(window);
	}/* else
	if (window->type == WINDOW_TYPE_MENU) {
		if (event->utype == GR_UPDATE_MAP)
			remap_menu_window(window);
		if (event->utype == GR_UPDATE_DESTROY)
			destroy_menu_window(window);
		if (event->utype == GR_UPDATE_UNMAP)
			unmap_menu_window(window);
		if (event->utype == GR_UPDATE_SIZE)
			resize_menu_window(window);
	}*/
}
