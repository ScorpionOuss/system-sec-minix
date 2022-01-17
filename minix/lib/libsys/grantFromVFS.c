#include <minix/ds.h>
#include <minix/csc.h>
#include <string.h>

#include "syslib.h"


static int do_invoke_vfsGrantCall(message *m, int type, int *grantID)
{
	int r;
	r = _taskcall(VFS_PROC_NR, type, m);
	//printf("We are back\n");
	*grantID = m->mCscG;
	//printf("We are here!\n");
	return r;
}

int csc_VFS_grant(endpoint_t endP, vir_bytes addr, int *grantID)
{
	/*****We call the VFS to create magic grant*****/
	message m;
	memset(&m, 0, sizeof(m));
	m.mCscE = endP;
    m.mCscV = addr;
	int r = do_invoke_vfsGrantCall(&m, VFS_CREATEMG, grantID);
	//printf("Back again back again!\n");
	return r;
}