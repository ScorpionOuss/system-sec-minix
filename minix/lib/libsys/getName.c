
#include <minix/ds.h>
#include <minix/csc.h>
#include <string.h>

#include "syslib.h"


static int do_invoke_getName(message *m, int type)
{
	int r;

	r = _taskcall(PM_PROC_NR, type, m);
	return r;
}

int pm_getName(endpoint_t endP, cp_grant_id_t grantId)
{
	message m;
	memset(&m, 0, sizeof(m));
	m.mCscE = endP;
    m.mCscG = grantId;
	return do_invoke_getName(&m, PM_GETNAME);
}