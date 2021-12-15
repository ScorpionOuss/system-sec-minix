#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include <minix/endpoint.h>
#include <minix/syslib.h>
#include <minix/safecopies.h>

#include "fs.h"



int create_grant(){
    /**
    Vfs creates a grant on demand to the Csc
    */
   endpoint_t endP = m_in.mCscE;
   vir_bytes virtAddr = m_in.mCscV;
   cp_grant_id_t magGrant = -1;
   magGrant = cpf_grant_magic(CSC_PROC_NR, endP, virtAddr, 4096, CPF_READ);
   //printf("We got the endP %d and the virtAddr %lu and we create a magic grant %d \n", endP, virtAddr, magGrant);
   job_m_out.mCscG = magGrant;
   if(magGrant == -1){
         return 1;
   }
   return 0;
}