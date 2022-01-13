#include <minix/csc.h>
#include <minix/syslib.h>
#include <minix/safecopies.h>

#include "inc.h"
#include "signcheck.h"


char name[16];

char page[4096];

/*===========================================================================*
 *		            sef_cb_init_fresh                                *
 *===========================================================================*/
int sef_cb_init_fresh(int UNUSED(type), sef_init_info_t *info)
{
  return(OK);
}


/*==========================================================================*
 *             Get the process memory page                          *
 *==========================================================================*/
int getPage(message *m_ptr, int *grantId){

  /********We perform a syscall to the VFS to create a magic grant********/
  //printf("CSC: system call to the VFS to creat a magic grant to access the page-->");
  int r = csc_VFS_grant((endpoint_t) m_ptr->mCscE, (vir_bytes) m_ptr->mCscV, grantId);
  
  /**We copy the content of the page into our GV page **/
  sys_safecopyfrom(VFS_PROC_NR, *grantId, 0, (vir_bytes) page, 4096*sizeof(char));
  //printf("CSC:grant received grant_id= %d\n", *grantId);
  
  return r;
}

/*===========================================================================*
 *				do_codecheck				     *
 *===========================================================================*/

int do_codecheck(message *m_ptr)
{
    //printf("csc: checkcode-->");
  //printf("invoked the syscall 01\n");
  
  /*****We call pm_getName() to get the name of the process associated 
   *                       to the endpoint                      *****/
  int res = pm_getName((endpoint_t) m_ptr->mCscE, name);

  /*** Here we filter on the process name ***/

  if (strncmp(name, "demo01\0", 7) == 0){
      //printf("process name is in the white list starting signature verification process");
    int grantId;
    /******We call the VFS that creates a magic grant and enables us
     * to perform a copy of the process page **********************/
    int r = getPage(m_ptr, &grantId);
    /****We got the page in page Global Variable****/
    //printf("We access the first 32 bits of page text segements %d\n", *((int *) page));
    //printf("CSC:signing Page-->");
    int32_t sign = 0;
    for (int i=0; i<4096; i+=32){
        sign^= *((int32_t *) (page+i));
    }
      printf("CSC: page%d signature=%d \n",m_ptr->mCscV,sign);

    /********Now that we have the page we should verify if it's consistent******/
    //printf("CSC:signature checked expected value= %d signature correct= %d",,);
  }else{
      //printf("process name is not in the white list no signature verification required");
  }

  return(OK);
}