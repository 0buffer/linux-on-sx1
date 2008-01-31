/* ipc.c
*
*  Common IPC functions for all daemons
*
* Copyright 2007 by Vladimir Ananiev (Vovan888 at gmail com )
*
*/

#include <flphone_config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>

#include "alarmd_internal.h"

static int client_fd = 0;
static int val = 0;

/* Handle IPC message
 * This message only tells indicator that its value is changed
 * Actual value is stored in sharedmem
*/
int ipc_handle(int fd)
{
	int ack = 0, size = 64;
	unsigned short src = 0;
	unsigned char msg_buf[64];

	DBGMSG("alarmserver: ipc_handle\n");

	if ((ack = ClGetMessage(&msg_buf, &size, &src)) < 0)
		return ack;

	if (ack == CL_CLIENT_BROADCAST) {
		/* handle broadcast message */
	}

	/*      if (IS_GROUP_MSG(src))
	   ipc_group_message(src, msg_buf); */
}
