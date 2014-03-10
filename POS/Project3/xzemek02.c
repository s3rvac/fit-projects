/*
 * Project:  POS 2009 (3)
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Encoding: iso8859-2
 */

#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 199506L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Maximal length of an input command */
#define MAX_INPUT_LENGTH 512

/* Shell exit command */
static const char *EXIT_COMMAND = "exit";

/*
 * Parsed command.
 */
typedef struct parsed_command {
	/* Program name */
	char *prog_name;
	/* Program arguments (like argv, e.g. 0 - program name, 1 - first arg, ...),
	  last item is NULL */
	char **prog_argv;
	/* 1 if the program should be run on background, 0 otherwise */
	int run_on_bg;
	/* Path to a file from which the input should be taken (can be NULL) */
	char *input_file_path;
	/* Path to a file to which the output should be stored (can be NULL) */
	char *output_file_path;
} parsed_command_t;

/*
 * Information about a running process on background.
 */
typedef struct process_info {
	/* Process PID */
	pid_t pid;
	/* Process name (yes, the size is overkill, but this way I don't have
	   to manually allocated/deallocate memory for the process name) */
	char name[MAX_INPUT_LENGTH + 1];
	/* Has the process ended? (1 means yes, 0 means no) */
	int ended;
	/* Process exit status */
	int exit_status;
	/* Pointer to the next item in the list */
	struct process_info *next;
} process_info_t;

/*
 * List (single linked) of information about running processes on background.
 */
typedef struct process_info_list {
	/* List head */
	process_info_t *head;
} process_info_list_t;

/* Command buffer; +1 for \0 (replaces \n) and +1 to detect a long input */
static char command_buffer[MAX_INPUT_LENGTH + 2] = {'\0'};
/* 1 - command was read and don't read a new command,
   0 - command was not read, so read a new command */
static int command_read = 0;
/* Mutex for command handling */
static pthread_mutex_t command_mutex = PTHREAD_MUTEX_INITIALIZER;
/* Command was read condition */
static pthread_cond_t command_read_cond = PTHREAD_COND_INITIALIZER;
/* Command was processed condition */
static pthread_cond_t command_processed_cond = PTHREAD_COND_INITIALIZER;
/* List of processes running on background. */
static process_info_list_t proc_info_list;
/* Should the process running on foreground should be killed? */
static volatile int end_fg_process = 0;
/* PID of the currently running process on foreground (if none, this is 0) */
static volatile pid_t current_fg_process_pid = 0;

/*
 * Prints program help to the selected stream.
 */
static void print_help(FILE *stream, const char *prog_name) {
	fprintf(stream, "usage: %s\n", prog_name);
}

/*
 * Print information about a process that just ended.
 */
static void print_process_exit_info(process_info_t *proc) {
	printf("\"%s\" [%u] done (status: %d)\n", proc->name, (unsigned) proc->pid,
		proc->exit_status);
}

/*
 * Skips whitespaces in the selected string (str and i must be not NULL).
 */
static void skip_whitespaces(const char *str, size_t *i) {
	const size_t STR_LENGTH = strlen(str);

	while (*i < STR_LENGTH && isspace((int) str[*i])) {
		++*i;
	}
}

/*
 * Parses a string from the selected string starting at index i.
 * str and i must be not NULL.
 * Returns file path allocated by malloc on success, NULL on failure.
 */
char *parse_string(const char *str, size_t *i) {
	const size_t STR_LENGTH = strlen(str);
	char delim;
	size_t j = 0;
	char *parsed_str = NULL;
	char *parsed_str_reallocked = NULL;

	/* Make sure there will be enough space for storing the string */
	parsed_str = malloc(STR_LENGTH * sizeof(char) + 1);
	if (parsed_str == NULL) {
		return NULL;
	}

	/* Parse the string */
	skip_whitespaces(str, i);
	if (str[*i] == '\'' || str[*i] == '"') {
		/* String in quotes */
		delim = str[*i];
		++*i;
		while (*i < STR_LENGTH && (str[*i] != delim || str[*i - 1] == '\\')) {
			parsed_str[j] = str[*i];
			++j;
			++*i;
		}
		if (str[*i] != delim) {
			/* Unmatched quotes */
			free(parsed_str);
			return NULL;
		}
		++*i;
	} else {
		while (*i < STR_LENGTH) {
			if (isspace((int) str[*i]) || str[*i] == '<' || str[*i] == '>' || str[*i] == '&') {
				break;
			}

			parsed_str[j] = str[*i];
			++j;
			++*i;
		}
	}
	skip_whitespaces(str, i);
	parsed_str[j] = '\0';

	/* Use only needed space */
	parsed_str_reallocked = realloc(parsed_str, strlen(parsed_str) + 1);
	if (parsed_str_reallocked == NULL) {
		return parsed_str;
	}

	return parsed_str_reallocked;
}

/*
 * Parses program arguments from the selected string starting at index i.
 * str, i and prog_name must be not NULL.
 * Returns program arguments allocated by malloc on success, NULL on failure.
 */
char **parse_program_argv(const char *str, size_t *i, const char *prog_name) {
	const size_t STR_LENGTH = strlen(str);
	size_t j = 0;
	size_t argc = 0;
	char *arg = NULL;
	char **argv = NULL;
	char **argv_reallocked = NULL;

	/* Yes, the allocated size is overkill, but I just want to make sure
	   it will be enough (I will realloc it later)
	   +1 is for the program name and +1 is for the last NULL pointer */
	argv = malloc((STR_LENGTH + 2) * sizeof(char *));
	if (argv == NULL) {
		return NULL;
	}
	for (j = 0; j < (STR_LENGTH + 2); ++j) {
		argv[j] = NULL;
	}

	/* Set the first argument to the program name */
	argv[0] = malloc(strlen(prog_name) + 1);
	if (argv[0] == NULL) {
		free(argv);
		return NULL;
	}
	strcpy(argv[0], prog_name);
	++argc;

	/* Parse the arguments, one by one */
	while (*i < STR_LENGTH) {
		arg = parse_string(str, i);
		if (arg == NULL) {
			/* Error - free the allocated memory */
			j = 0;
			while (argv[j] != NULL) {
				free(argv[j]);
				++j;
			}

			return NULL;
		} else if (arg[0] == '\0') {
			/* No more arguments */
			free(arg);
			break;
		}

		argv[argc] = arg;
		++argc;
	}
	argv[argc] = NULL;

	/* Use only needed space */
	argv_reallocked = malloc((argc + 1) * sizeof(char *));
	if (argv_reallocked == NULL) {
		return argv;
	}
	j = 0;
	while (argv[j] != NULL) {
		argv_reallocked[j] = argv[j];
		++j;
	}
	argv_reallocked[j] = NULL;
	free(argv);

	return argv_reallocked;
}

/*
 * Parses the selected command into pc.
 *
 * command - command to be parsed; must be not NULL
 * pc - into this structure the parsed command will be stored; must be not NULL
 *
 * Returns 0 on success (pc is correctly set), else -1 is returned and the content
 * of pc is undefined (system error or invalid command/path syntax).
 * pc should be (only on success) then destroyed by calling destroy_command().
 */
static int parse_command(const char *command, parsed_command_t *pc) {
	size_t i = 0;
	const size_t COMMAND_LENGTH = strlen(command);

	assert(command != NULL);
	assert(pc != NULL);

	/* Initialization */
	pc->prog_name = NULL;
	pc->prog_argv = NULL;
	pc->run_on_bg = 0;
	pc->input_file_path = NULL;
	pc->output_file_path = NULL;

	/* Parse the command */
	i = 0;
	/* Program name */
	pc->prog_name = parse_string(command, &i);
	if (pc->prog_name == NULL) {
		return -1;
	}

	/* Program arguments */
	pc->prog_argv = parse_program_argv(command, &i, pc->prog_name);
	if (pc->prog_argv == NULL) {
		return -1;
	}

	if (pc->prog_name[0] == '\0') {
		/* No program path was set, but that's ok */
		return 0;
	}

	/* Parse the rest of the string */
	skip_whitespaces(command, &i);
	while (i < COMMAND_LENGTH) {
		if (command[i] == '&') {
			/* Run on background */
			if (pc->run_on_bg) {
				/* Second occurence of this symbol -> error */
				return -1;
			}

			pc->run_on_bg = 1;
			++i;
		} else if (command[i] == '<') {
			/* Input file path */
			++i;
			pc->input_file_path = parse_string(command, &i);
			if (pc->input_file_path == NULL || (pc->input_file_path)[0] == '\0') {
				return -1;
			}
		} else if (command[i] == '>') {
			/* Output file path */
			++i;
			pc->output_file_path = parse_string(command, &i);
			if (pc->output_file_path == NULL || (pc->output_file_path)[0] == '\0') {
				return -1;
			}
		} else {
			/* Syntax error */
			return -1;
		}

		skip_whitespaces(command, &i);
	}

	return 0;
}

/*
 * Destroys the selected parsed command (must be not NULL).
 */
static void destroy_command(parsed_command_t *pc) {
	char **p;

	assert(pc != NULL);

	free(pc->prog_name);
	pc->prog_name = NULL;
	if (pc->prog_argv != NULL) {
		p = pc->prog_argv;
		while (*p != NULL) {
			free(*p);
			p++;
		}
	}
	free(pc->prog_argv);
	pc->prog_argv = NULL;
	free(pc->input_file_path);
	pc->input_file_path = NULL;
	free(pc->output_file_path);
	pc->output_file_path = NULL;
}

/*
 * Function for the 2nd thread.
 * It reads input from the command line and puts it into the buffer.
 * Does not accept any arguments (anything can be passed). Returns always NULL.
 */
static void *read_commands(void *args) {
	/* Temporary character used to read the rest of a line in case of a long input
	   Note that it can be char and not int because I use read() to read the
	   input, not getchar()! */
	char c = '\0';
	/* Number of read bytes */
	ssize_t read_bytes = 0;
	/* Return code */
	int rc = 0;
	/* Signal blocking mask */
	sigset_t sig_block_mask;

	/* Get rid of the unused parameter 'args' warning */
	(void) args;

	/* Block all signals in this thread (it is not supposed to handle any signals) */
	sigfillset(&sig_block_mask);
    pthread_sigmask(SIG_SETMASK, &sig_block_mask, NULL);

	for (;;) {
		printf("$ ");

		/* Since this thread blocks all signals, we don't need to care
		   about EINTR */
		read_bytes = read(STDIN_FILENO, command_buffer, sizeof(command_buffer));
		if (read_bytes == -1) {
			/* There was an error */
			fprintf(stderr, "read() error: %d\n", errno);

			continue;
		} else if (read_bytes == 0) {
			/* "End of file", e.g. Ctrl+D - user wants to exit the shell */
			strncpy(command_buffer, EXIT_COMMAND, sizeof(command_buffer));
			read_bytes = strlen(EXIT_COMMAND) + 1;
		} else if (read_bytes == sizeof(command_buffer)) {
			/* Too long input */
			fprintf(stderr, "input too long - ignoring\n");
			/* Read the rest of the line (ignore errors) */
			c = command_buffer[read_bytes - 1];
			while (c != '\n') {
				read(STDIN_FILENO, &c, 1);
			}

			continue;
		}
		/* Remove the '\n' character and correctly end the buffer */
		command_buffer[read_bytes - 1] = '\0';

		/* Input was read correctly and has correct length, so signal
		   it to the main thread */
		rc = pthread_mutex_lock(&command_mutex);
		if (rc != 0) {
			fprintf(stderr, "pthread_mutex_lock() error: %d\n", rc);
			exit(EXIT_FAILURE);
		}
		command_read = 1;
		rc = pthread_cond_signal(&command_read_cond);
		if (rc != 0) {
			fprintf(stderr, "pthread_cond_signal() error: %d\n", rc);
			exit(EXIT_FAILURE);
		}

		/* Wait until we can read another input */
		while (command_read) {
			rc = pthread_cond_wait(&command_processed_cond, &command_mutex);
			if (rc != 0) {
				fprintf(stderr, "pthread_cond_wait() error: %d\n", rc);
				exit(EXIT_FAILURE);
			}
		}
		rc = pthread_mutex_unlock(&command_mutex);
		if (rc != 0) {
			fprintf(stderr, "pthread_mutex_unlock() error: %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}

	pthread_exit(NULL);
}

/*
 * Signal handler.
 */
static void signal_handler(int signum) {
	/* Process info list pointer */
	process_info_t *proc_info_list_item;
	/* Process PID */
	pid_t pid;
	/* Process exit status */
	int exit_status;

	if (signum == SIGCHLD) {
		/* Get the pid of the ended process */
		pid = wait(&exit_status);

		/* If there is a running process on the foreground, check whether
		   it was that process that sent the SIGCHLD signal */
		if (current_fg_process_pid == pid) {
			/* It was the process on the foreground */
			current_fg_process_pid = 0;
		} else {
			/* It was a process on the background */
			proc_info_list_item = proc_info_list.head;
			while (proc_info_list_item != NULL) {
				if (proc_info_list_item->pid == pid) {
					proc_info_list_item->ended = 1;
					proc_info_list_item->exit_status = exit_status;
					break;
				}
				proc_info_list_item = proc_info_list_item->next;
			}
		}
	} else if (signum == SIGINT && current_fg_process_pid != 0) {
		/* The running process on the foreground should be ended */
		end_fg_process = 1;
		current_fg_process_pid = 0;
	}
}

/*
 * Function for the main thread.
 * It reads commands from the buffer and executes them.
 * Returns what the main program should return.
 */
static int execute_commands() {
	/* Parsed command from the command buffer */
	parsed_command_t pc;
	/* New file descriptors for input and output */
	int new_stdin_fd = 0;
	int new_stdout_fd = 0;
	/* Signal actions */
	struct sigaction sig_action;
	/* Signal blocking masks */
	sigset_t sig_block_all_mask;
	sigset_t sig_allow_all_mask;
	/* Process info */
	process_info_t *proc_info;
	/* Process info list pointers */
	process_info_t *proc_info_list_item;
	process_info_t *proc_info_list_prev_item;
	process_info_t *tmp_proc_info_list_item;
	/* PID used while fork()ing */
	pid_t pid;
	/* Return code */
	int rc = 0;

	/* Set signal blocking masks */
	sigfillset(&sig_block_all_mask);
	sigemptyset(&sig_allow_all_mask);

	/* Set signal handler */
	sigfillset(&sig_action.sa_mask);
	sig_action.sa_handler = signal_handler;
	sig_action.sa_flags = 0;
	sigaction(SIGCHLD, &sig_action, NULL);
	sigaction(SIGINT, &sig_action, NULL);

	/* Initialize the process info list */
	proc_info_list.head = NULL;

	for (;;) {
		/* Wait until a new command is read */
		rc = pthread_mutex_lock(&command_mutex);
		if (rc != 0) {
			fprintf(stderr, "pthread_mutex_lock() error: %d\n", rc);
			exit(EXIT_FAILURE);
		}
		while (!command_read) {
			rc = pthread_cond_wait(&command_read_cond, &command_mutex);
			if (rc != 0) {
				fprintf(stderr, "pthread_cond_wait() error: %d\n", rc);
				exit(EXIT_FAILURE);
			}
		}
		rc = pthread_mutex_unlock(&command_mutex);
		if (rc != 0) {
			fprintf(stderr, "pthread_mutex_unlock() error: %d\n", rc);
			exit(EXIT_FAILURE);
		}

		/* Block all signals */
		pthread_sigmask(SIG_SETMASK, &sig_block_all_mask, NULL);

		/* Process the command (block all signals until the command is processed) */
		do { /* while(0) trick */
			if (parse_command(command_buffer, &pc) == -1) {
				fprintf(stderr, "selected command could not be parsed "
					"(invalid syntax or system error)\n");
				break;
			}

			if (pc.prog_name[0] == '\0') {
				/* No command */
				destroy_command(&pc);
				break;
			} else if (strcmp(pc.prog_name, EXIT_COMMAND) == 0) {
				/* User wants to exit the shell */

				/* Terminate all the processes running on the background */
				proc_info_list_item = proc_info_list.head;
				while (proc_info_list_item != NULL) {
					if (proc_info_list_item->ended == 1) {
						print_process_exit_info(proc_info_list_item);
					} else {
						kill(proc_info_list_item->pid, SIGTERM);
					}
					tmp_proc_info_list_item = proc_info_list_item;
					proc_info_list_item = tmp_proc_info_list_item->next;
					free(tmp_proc_info_list_item);
				}

				destroy_command(&pc);
				return EXIT_SUCCESS;
			}

			/* Run the selected program */
			switch (pid = fork()) {
				case -1:
					fprintf(stderr, "fork() failed: %d\n", errno);
					break;

				case 0:
					/* Child process */

					/* Unblock all signals */
					sigprocmask(SIG_SETMASK, &sig_allow_all_mask, NULL);

					/* If the process will be running on background,
					   block SIGINT to avoid premature process exit if SIGINT
					   is sent to the process on foreground */
					if (pc.run_on_bg) {
						sigaddset(&sig_allow_all_mask, SIGINT);
						sigprocmask(SIG_SETMASK, &sig_allow_all_mask, NULL);
					}

					/* Set the signal handler for SIGCHLD and SIGINT back
					   to the default one */
					sig_action.sa_handler = SIG_DFL;
					sigaction(SIGCHLD, &sig_action, NULL);
					sigaction(SIGINT, &sig_action, NULL);

					/* Prepare standard input for that process */
					if (pc.input_file_path != NULL) {
						new_stdin_fd = open(pc.input_file_path, O_RDONLY);
						if (new_stdin_fd == -1) {
							fprintf(stderr, "open() failed: %d\n", errno);
							exit(EXIT_FAILURE);
						}
						if (dup2(new_stdin_fd, STDIN_FILENO) == -1) {
							fprintf(stderr, "dup2() failed: %d\n", errno);
							exit(EXIT_FAILURE);
						}
					}
					/* Prepare standard output for that process */
					if (pc.output_file_path != NULL) {
						new_stdout_fd = open(pc.output_file_path,
							O_CREAT | O_WRONLY | O_TRUNC,
							/* rw-rw-rw, will be modified by umask */
							S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
						if (new_stdout_fd == -1) {
							fprintf(stderr, "open() failed: %d\n", errno);
							exit(EXIT_FAILURE);
						}
						if (dup2(new_stdout_fd, STDOUT_FILENO) == -1) {
							fprintf(stderr, "dup2() failed: %d\n", errno);
							exit(EXIT_FAILURE);
						}
					}

					/* execvp() should take $PATH into account */
					if (execvp(pc.prog_name, pc.prog_argv) == -1) {
						fprintf(stderr, "execvp() failed: %d\n", errno);
						if (new_stdin_fd != 0) {
							close(new_stdin_fd);
						}
						if (new_stdout_fd != 0) {
							close(new_stdout_fd);
						}
						exit(EXIT_FAILURE);
					}
					break;

				default:
					/* Parent process */

					/* We don't need the arguments, so destroy them */
					destroy_command(&pc);

					if (pc.run_on_bg) {
						/* Process is ran on the background, so add it
						   to the list of processes that runs on the background */
						proc_info = malloc(sizeof(process_info_t));
						if (proc_info == NULL) {
							fprintf(stderr, "out of memory\n");
							exit(EXIT_FAILURE);
						}
						proc_info->pid = pid;
						proc_info->ended = 0;
						strncpy(proc_info->name, command_buffer, MAX_INPUT_LENGTH + 1);

						/* Put the process into the list */
						proc_info->next = NULL;
						if (proc_info_list.head == NULL) {
							/* This process will be the first one in the list */
							proc_info_list.head = proc_info;
						} else {
							/* There are already some processes, so put
							   the process at the end of the process list */
							proc_info_list_item = proc_info_list.head;
							/* Since there is more than one process, it is
							   guaranteed that the first item is not NULL */
							while (proc_info_list_item->next != NULL) {
								proc_info_list_item = proc_info_list_item->next;
							}
							proc_info_list_item->next = proc_info;
						}
					} else {
						/* Process is ran on the foreground, so wait until
						   the child exits (SIGCHLD handling is done in
						   the signal handler) or until there is a request
						   to end this process */
						current_fg_process_pid = pid;
						while (current_fg_process_pid != 0) {
							sigsuspend(&sig_allow_all_mask);
						}
						if (end_fg_process) {
							end_fg_process = 0;
							kill(pid, SIGINT);
						}
					}

					break;
			}
		} while (0);

		/* Check whether there is any process that just ended (all signals
		   (except SIGTERM and non-blockable signals) are blockedat this moment) */
		proc_info_list_item = proc_info_list.head;
		proc_info_list_prev_item = NULL;
		while (proc_info_list_item != NULL) {
			if (proc_info_list_item->ended == 1) {
				print_process_exit_info(proc_info_list_item);

				/* Remove the process from the list */
				if (proc_info_list.head == proc_info_list_item || proc_info_list_prev_item == NULL) {
					/* The process is the first one in the list */
					proc_info_list.head = proc_info_list_item->next;
				} else {
					/* The process is not the first one in the list */
					proc_info_list_prev_item->next = proc_info_list_item->next;
				}

				/* Free the memory of that item */
				tmp_proc_info_list_item = proc_info_list_item;
				proc_info_list_item = proc_info_list_item->next;
				free(tmp_proc_info_list_item);
			} else {
				proc_info_list_prev_item = proc_info_list_item;
				proc_info_list_item = proc_info_list_item->next;
			}
		}

		/* Unblock all signals */
		pthread_sigmask(SIG_SETMASK, &sig_allow_all_mask, NULL);

		/* Signal to the reading thread that it can read another input */
		command_read = 0;
		rc = pthread_cond_signal(&command_processed_cond);
		if (rc != 0) {
			fprintf(stderr, "pthread_cond_signal() error: %d\n", rc);
			exit(EXIT_FAILURE);
		}
	}

	return EXIT_SUCCESS;
}

/*
 * Main.
 */
int main(int argc, char **argv) {
	/* Thread used for reading commands */
	pthread_t read_commands_thread;
	/* Return code */
	int rc = 0;

	/* Check program arguments */
	if (argc > 1) {
		/* Check whether there was a help request */
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			print_help(stdout, argv[0]);
			exit(EXIT_SUCCESS);
		} else {
			/* There should be no arguments */
			print_help(stderr, argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	/* Disable buffering on stdout */
    setbuf(stdout, NULL);

	/* Create a thread for command readings */
	rc = pthread_create(&read_commands_thread, NULL, read_commands, NULL);
	if (rc != 0) {
		fprintf(stderr, "pthread_create() error: %d\n", rc);
		exit(EXIT_FAILURE);
	}

	/* The thread can be detached (I don't need it to be joinable) */
	rc = pthread_detach(read_commands_thread);
	if (rc != 0) {
		fprintf(stderr, "pthread_detach() error: %d\n", rc);
		exit(EXIT_FAILURE);
	}

	/* Enter the main loop */
	return execute_commands();
}

/* End of file */
