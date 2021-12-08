#include <minix/ds.h>
#include <minix/csc.h>
#include <string.h>

#include "syslib.h"


static int do_invoke_getName(message *m, int type, char *name)
{
	int r;
	r = _taskcall(PM_PROC_NR, type, m);
	printf("Here we got the name: %s\n", m->mCscN);
	strncpy(name, m->mCscN, 16);
	return r;
}

int pm_getName(endpoint_t endP, char *name)
{
	message m;
	memset(&m, 0, sizeof(m));
	m.mCscE = endP;
	return do_invoke_getName(&m, PM_GETNAME, name);
}