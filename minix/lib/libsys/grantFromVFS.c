#include <minix/ds.h>
#include <minix/csc.h>
#include <string.h>

#include "syslib.h"


static int do_invoke_vfsGrantCall(message *m, int type)
{
	int r;
	r = _taskcall(VFS_PROC_NR, type, m);
	return r;
}

int csc_VFS_grant(endpoint_t endP, vir_bytes addr)
{
	message m;
	memset(&m, 0, sizeof(m));
	m.mCscE = endP;
    m.mCscV = addr;
	return do_invoke_vfsGrantCall(&m, VFS_CREATEMG);
}