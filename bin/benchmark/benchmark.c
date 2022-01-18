/* $NetBSD: benchmark.c,v 1.57 2016/06/16 00:52:37 sevan Exp $	*/

/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of KTH.  All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/param.h>
#include <sys/stat.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/wait.h>

int run_command(char *);


int run_command(
	char * cmd
) {
  printf("%s \n", cmd);
  pid_t pid;			/* Pid of child.  */
  pid = fork ();		/* Run CMD as child process.  */
  if (pid < 0)
    printf ("cannot fork\n");
  else if (pid == 0)
    {	/* If child.  */
		char * arr[] = {cmd, NULL};
		execv(cmd, arr);	  
    } else {
		wait(NULL);
		return 1;
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	char *command_line;
  	command_line = argv[1];

    // to store the execution time of code
	struct timeval tv_before;
	struct timeval tv_after;
	clock_t clock_before;
	clock_t clock_after;
	double total_time_clock = 0;
	double total_time_date = 0;
	clock_before = clock();
	gettimeofday(&tv_before,NULL);

	for (int i = 0; i < 1000; i++) {
		int r = run_command(command_line);

		if (r == 1) {
			gettimeofday(&tv_after,NULL);
			clock_after = clock();

			double time_s = (double) tv_after.tv_sec - (double) tv_before.tv_sec;
			double time_us = (double) ((long) tv_after.tv_usec - (long) tv_before.tv_usec) / 1000000;
			time_s += time_us;
			total_time_date += time_s;

			double total_t = (double) (clock_after - clock_before) / CLOCKS_PER_SEC;
			total_time_clock += total_t;
		} else {
			return 0; // Child dies.
		}
	}

	printf("Seconds taken clock: %f \n", total_time_clock);
	printf("Seconds taken date: %f \n", total_time_date  );
	printf("Seconds taken clock avg: %f \n", total_time_clock / 1000);
	printf("Seconds taken date avg: %f \n", total_time_date / 1000);

	return 0;
}