
#include "inc.h"
#include "signcheck.h"


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
  return(OK);
} 