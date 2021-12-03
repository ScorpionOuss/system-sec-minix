#ifndef _CSC_PROTO_H
#define _CSC_PROTO_H

/* Function prototypes. */

/* main.c */
int main(int argc, char **argv);

/* myserver.c */
int do_codecheck(message *m_ptr);
int sef_cb_init_fresh(int type, sef_init_info_t *info);

#endif