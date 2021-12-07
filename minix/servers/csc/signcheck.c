
#include <minix/csc.h>
#include <minix/syslib.h>
#include <minix/safecopies.h>

#include "inc.h"
#include "signcheck.h"


char name[16];

/*===========================================================================*
 *		            sef_cb_init_fresh                                *
 *===========================================================================*/
int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *info)
{
  return(OK);
}

/*===========================================================================*
 *				do_publish				     *
 *===========================================================================*/

int do_codecheck(message *m_ptr)
{
  printf("invoked the syscall 01\n");
  /******Here we grant write permissions******/ 
  cp_grant_id_t grantId = cpf_grant_direct(PM_PROC_NR,
  (vir_bytes) name, 16*sizeof(char), CPF_WRITE);
  /*****We call pm_getName()*****/
  int res = pm_getName((endpoint_t) m_ptr->mCscE, grantId);
  printf("We got the name : %s \n", name);
  return(OK);
} 
