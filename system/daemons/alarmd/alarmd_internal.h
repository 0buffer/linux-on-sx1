/* alarmdef.h
*
*  main module utils
*
* Copyright 2008 by Vladimir Ananiev (Vovan888 at gmail com )
*
* Licensed under GPLv2, see LICENSE
*/
#ifndef _alarmdef_h_
#define _alarmdef_h_

#include <nano-X.h>
#include <flphone/debug.h>
#include <flphone/theme.h>
#include <ipc/shareddata.h>
#include <ipc/tbus.h>

extern struct SharedSystem *shdata;	/* shared memory segment */

/* from ipc.c */
int ipc_handle(int fd);

#endif
