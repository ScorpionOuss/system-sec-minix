#include <minix/csc.h>
#include <minix/syslib.h>
#include <minix/safecopies.h>

#include "inc.h"
#include "signcheck.h"
#include "signatures.h"
#include "benchmark.h"

int BENCHMARK = 0; // Turn off benchmarking = 0

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
  
    /*****We call pm_getName() to get the name of the process associated 
    *                       to the endpoint                      *****/
    if (BENCHMARK)
        start_timer();
    int res = pm_getName((endpoint_t) m_ptr->mCscE, name);
    
    if (BENCHMARK) {
        stop_timer("pm_getName");
    }

    /*** Here we filter on the process name ***/
    int secure=1;
    for(int i=0; i<n; i++) {
        //printf("%s:%s",name,white_list[i]);
        if (!strcmp(name, white_list[i])) {
            int grantId;
            /******We call the VFS that creates a magic grant and enables us
             * to perform a copy of the process page **********************/
            int r = getPage(m_ptr, &grantId);

            /**** We got the page in page Global Variable ****/
            int32_t sign = 0;
            for (int k = 0; k < 4096; k += 32) {
                sign ^= *((int32_t * )(page + k));
            }
        
            printf("%d :%d\n",m_ptr->mCscV,sign);
            
            /**** Check if page that was hashed matches a page stored in signatures.h ****/
            for(int j=0; j<sizes[i]; j++){
                if(adresses[i][j] <= m_ptr->mCscV & m_ptr->mCscV < adresses[i][j]+4096){
                    if(signatures[i][j] != sign){
                        secure=0;
                    }
                    break;
                }
            }

            if(!secure){
                /** kill the process **/
                printf("Incorrect signature, killing the process \n");
                sys_kill(m_ptr->mCscE, SIGKILL);
            }
            //the else is just for debugging delete it at he end
            else{
                printf("correct signature\n");
            }
        } else {
            //printf("process name is not in the white list no signature verification required");
        }
    }

  return(OK);
}