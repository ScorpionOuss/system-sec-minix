#include <lib.h>
#include <minix/csc.h>>
//endpoint_t end, vir_bytes addr

int cscCall(){
    message m;
    if (_syscall(CSC_PROC_NR, CSC_CODECHECK, &m) <0){
        return -1;
    }
    return 0;
}