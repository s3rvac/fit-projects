/*
 * Project:  POS 2009 (1)
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Encoding: iso8859-2
 */

#define _XOPEN_SOURCE
#define _XOPEN_SOURCE_EXTENDED 1 /* XPG 4.2 - needed for WCOREDUMP() */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* MY_WCOREDUMP - WCOREDUMP may not be available on all systems */
#ifdef WCOREDUMP
	#define MY_WCOREDUMP(status) (WCOREDUMP(status) ? "with core dump " : "")
#else
	#define MY_WCOREDUMP(status) ("")
#endif

/*
 * Process labels
 */
static const char *PROCESS_1_LABEL = "grandparent";
static const char *PROCESS_2_LABEL = "parent";
static const char *PROCESS_3_LABEL = "child";

/*
 * Prints process information to standard output.
 */
static void print_process_info(const char *process_label) {
	printf("%s identification: \n", process_label);
	printf("pid = %d,ppid = %d,pgrp = %d\n", (int)getpid(), (int)getppid(),
		(int)getpgrp());
	printf("uid = %d,gid = %d\n", (int)getuid(), (int)getgid());
	printf("euid = %d,egid = %d\n", (int)geteuid(), (int)getegid());
}

/*
 * Waits until a child process ends and returns its status.
 * If there is a problem, it exists with EXIT_FAILURE.
 */
static int wait_until_child_ends(pid_t child_pid) {
	int status;

	errno = 0;
	if (waitpid(child_pid, &status, 0) == -1) {
		perror("waitpid() failed");
		exit(EXIT_FAILURE);
	}

	return status;
}

/*
 * Prints the child end reason.
 */
static void print_child_end_reason(pid_t child_pid, const char *child_label,
		int end_status) {
	printf("%s exit (pid = %d):", child_label, (int)child_pid);

	if (WIFSIGNALED(end_status)) {
		printf("signal termination %s(signal = %d)\n",
			MY_WCOREDUMP(end_status), WTERMSIG(end_status));
	} else if (WIFEXITED(end_status)) {
		printf("normal termination (exit code = %d)\n",
			WEXITSTATUS(end_status));
	} else {
		printf("unknown type of termination\n");
	}
}

/* ARGSUSED */
int main(int argc, char **argv) {
	int status; /* Process status */
	pid_t pid; /* PID used while fork()ing */

	/* Get rid of the unused parameter 'argc' warning */
	(void) argc;

    /* Disable buffering on stdout */
    setbuf(stdout, NULL);

	print_process_info(PROCESS_1_LABEL);

	/* Spawn off a child */
	errno = 0;
	switch (pid = fork()) {
		case -1:
			/* fork() failed */
			perror("fork() failed");
			exit(EXIT_FAILURE);
			break;

		case 0:
			/* Child process */
			print_process_info(PROCESS_2_LABEL);

			/* Spawn off another child */
			errno = 0;
			switch (pid = fork()) {
				case -1:
					/* fork() failed */
					perror("fork() failed");
					exit(EXIT_FAILURE);
					break;

				case 0:
					/* Child process */
					print_process_info(PROCESS_3_LABEL);

					/* Replace the process with the given program and
						its arguments */
					errno = 0;
					/* Note: The second argument must be &argv[1], not &argv[2],
					   because the execv() function requires (as standard C programs)
					   that the argument list starts with the name of the file
					   that will be executed */
					/* If argc < 2, then this call to execv will fail
					   (according to webnews, the program should behave this way).
					   Note that argv[1] will contain a null pointer. */
					if (execv(argv[1], &argv[1]) == -1) {
						perror("execv() failed");
						exit(EXIT_FAILURE);
					}
					break;

				default:
					/* Parent process */
					status = wait_until_child_ends(pid);
					print_child_end_reason(pid, PROCESS_3_LABEL, status);
					break;
			}
			break;

		default:
			/* Parent process */
			status = wait_until_child_ends(pid);
			print_child_end_reason(pid, PROCESS_2_LABEL, status);
			break;
	}

	return EXIT_SUCCESS;
}

/* End of file */
