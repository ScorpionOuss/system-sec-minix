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
   cp_grant_id_t magGrant = cpf_grant_magic(CSC_PROC_NR, endP, virtAddr, 4096, CPF_READ);
   message m_out;
   m_out.mCscG = magGrant;
   int r = ipc_sendnb(CSC_PROC_NR, &m_out);
   return r;
}