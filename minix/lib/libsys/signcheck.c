
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

int csc_codecheck(endpoint_t endP, vir_bytes vaddr)
{
	message m;
	memset(&m, 0, sizeof(m));
	m.mCscE = endP;
	m.mCscV = vaddr;
	return do_invoke_myserver(&m, CSC_CODECHECK);
}