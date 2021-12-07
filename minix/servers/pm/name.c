/* File that implments getName syscall*/


#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include <minix/endpoint.h>
#include <minix/syslib.h>

#include "pm.h"
#include "mproc.h"

int findName(endpoint_t endP){
    struct mproc *rmp, *csc;
    int proc_n;
    /******A loop to find the process whose endpoint is endP*****/
    for (int i = 0; i < NR_PROCS; i++){
        rmp = &mproc[i];
        if (rmp->mp_endpoint == endP){
            endpoint_t proc_e= m_in.m_lexec_pm_exec_new.endpt;
	        if (pm_isokendpt(proc_e, &proc_n) != OK) {
		        panic("det_name: got bad endpoint: %d", proc_e);
            }
            printf("Found it ****************: %s *****************!\n", rmp->mp_name);
            csc = &mproc[proc_n];
            /****We write in the grant****/
            sys_safecopyto(CSC_PROC_NR, m_in.mCscG, 0, (vir_bytes) rmp->mp_name, 16*sizeof(char));
            break;
        } 
    }
    printf("I am figuring out the name!\n");
    return (OK);
}

int get_name(void)
{
    endpoint_t endP = m_in.mCscE;
    return findName(endP);
}