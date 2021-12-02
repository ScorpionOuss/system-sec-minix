
#include <minix/ds.h>
#include <minix/csc.h>
#include <string.h>

#include "syslib.h"

static int do_invoke_myserver(message *m, int type)
{
	int r;

	r = _taskcall(CSC_PROC_NR, type, m);

	return r;
}

int myserver_sys1(void)
{
	message m;

	memset(&m, 0, sizeof(m));
	return do_invoke_myserver(&m, CSC_SYS1);
}