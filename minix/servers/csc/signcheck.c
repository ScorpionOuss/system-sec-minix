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
  // cp_grant_id_t grantId = cpf_grant_direct(PM_PROC_NR,
  // (vir_bytes) name, 16*sizeof(char), CPF_WRITE);
  
  /*****We call pm_getName()*****/
  int res = pm_getName((endpoint_t) m_ptr->mCscE, name);

  /*** Here we filter on the process name ***/
  if (strncmp(name, "demo01", 6) == 0){
    int grantId;
    //printf("*************Yesss! and endpoint is %d\n",  m_ptr->mCscE);
    int r = csc_VFS_grant((endpoint_t) m_ptr->mCscE, (vir_bytes) m_ptr->mCscV, &grantId);
    printf("We got the grantID back in our server %d, and we are happy%d\n", grantId, r);
  }

  /*We need one call to the vfs, we provide it with the end point, and it should
  create a magic grant for us, a read magic grant and*/

  /**
   * add a syscall to the vfs 
   * Then take the grantID
   * Use the grant ID 
   * Allocate a page and perform the safe copy.
   * sys_safecopyto(CSC_PROC_NR, m_in.mCscG, 0, (vir_bytes) rmp->mp_name, 16*sizeof(char))
   *  perform the signature
   */
  return(OK);
}