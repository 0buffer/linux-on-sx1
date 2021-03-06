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

/* Handle IPC message
 * This message only tells indicator that its value is changed
 * Actual value is stored in sharedmem
*/
int ipc_handle(int fd)
{
	int ret;
	struct tbus_message msg;

	DBGMSG("ipc_handle\n");

	ret = tbus_get_message(&msg);
	if (ret < 0)
		return -1;
	switch (msg.type) {
		case TBUS_MSG_SIGNAL:
		/* we received a signal */
//                      ipc_signal(&msg);
		break;
	}

	tbus_msg_free(&msg);
	return 0;
}
