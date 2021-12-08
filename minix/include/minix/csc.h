/* Prototypes and definitions for MYSERVER interface. */

#ifndef _MINIX_CSC_H
#define _MINIX_CSC_H

#include <minix/endpoint.h>

/* myserver.c */

/* U32 */
int csc_codecheck(endpoint_t endP);
int pm_getName(endpoint_t endP, char *name);
int csc_VFS_grant(endpoint_t endP, vir_bytes addr);

#endif /* _MINIX_MYSERVER_H */