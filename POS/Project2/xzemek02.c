/*
 * Project:  POS 2009 (2)
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Encoding: iso8859-2
 */

#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 199506L

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

/* Maximal thread waiting time (in nanoseconds) */
static const long MAX_WAITING_TIME_NSECS = 500000000L; /* 0.5 s */

/* How many times should the critical section be passed */
static int crit_sect_pass_count = 0;
/* Last ticket number */
static int last_ticket = 0;
/* Ticket that can enter the critical section */
static int ticket_to_crit_sect = 0;
/* Mutex for the getticket() function */
static pthread_mutex_t getticket_mutex = PTHREAD_MUTEX_INITIALIZER;
/* Mutex for r/w the ticket_to_crit_sect and threads_should_terminate variables */
static pthread_mutex_t crit_sect_mutex = PTHREAD_MUTEX_INITIALIZER;
/* Thread can check whether its turn to enter the critical section */
static pthread_cond_t thread_can_check_its_turn_cond = PTHREAD_COND_INITIALIZER;

/*
 * Prints program help to the standard error.
 */
static void print_help(const char *prog_name) {
	fprintf(stderr, "usage: %s N M\n", prog_name);
	fprintf(stderr, "N - number of threads to be created (1 <= N <= %d)\n",
		INT_MAX);
	fprintf(stderr, "M - how many times should the critical section "
		"be passed (0 <= M <= %d)\n", INT_MAX);
}

/*
 * Converts the selected string to int and stores it in result.
 * Returns 1 if the conversion went ok, 0 otherwise (result is left untouched).
 */
static int str2int(const char *str, int *result) {
	long tmp_result = 0;
	char *tail = NULL;

	/* Preconditions */
	assert(str != NULL);
	assert(result != NULL);

	/* Conversion */
	errno = 0;
	tmp_result = strtol(str, &tail, 10);
	if (errno != 0 || *tail != '\0' || tmp_result < INT_MIN ||
			tmp_result > INT_MAX) {
		return 0;
	}

	*result = tmp_result;
	return 1;
}

/*
 * Returns 1 if the selected number is in the selected interval, 0 otherwise.
 */
static int in_interval(int number, int left_bound, int right_bound) {
	return (number >= left_bound && number <= right_bound) ? 1 : 0;
}

/*
 * Returns a new ticket number.
 */
static int getticket(void) {
	int new_ticket = 0;
	int rc = 0;

	rc = pthread_mutex_lock(&getticket_mutex);
	if (rc != 0) {
		fprintf(stderr, "pthread_mutex_lock() error: %d\n", rc);
		exit(EXIT_FAILURE); /* As specified in a discussion in WIS, exit the program */
	}

	new_ticket = last_ticket++;

	rc = pthread_mutex_unlock(&getticket_mutex);
	if (rc != 0) {
		fprintf(stderr, "pthread_mutex_unlock() error: %d\n", rc);
		exit(EXIT_FAILURE); /* As specified in a discussion in WIS, exit the program */
	}

	return new_ticket;
}

/*
 * Enters the critical section.
 * aenter is a ticket number returned by getticket().
 */
static void await(int aenter) {
	int rc = 0;

	/* Check whether this thread can enter the critical section */
	rc = pthread_mutex_lock(&crit_sect_mutex);
	if (rc != 0) {
		fprintf(stderr, "pthread_mutex_lock() error: %d\n", rc);
		exit(EXIT_FAILURE); /* As specified in a discussion in WIS, exit the program */
	}

	while (ticket_to_crit_sect != aenter) {
		/* It's not our turn, so wait until we can check whether is it our turn again */
		rc = pthread_cond_wait(&thread_can_check_its_turn_cond, &crit_sect_mutex);
		if (rc != 0) {
			fprintf(stderr, "pthread_cond_wait() error: %d\n", rc);
			exit(EXIT_FAILURE); /* As specified in a discussion in WIS, exit the program */
		}

		if (ticket_to_crit_sect == crit_sect_pass_count) {
			/* The desired total number of passes has been accomplished */
			pthread_mutex_unlock(&crit_sect_mutex);
			pthread_exit(NULL);
		}
	}
}

/*
 * Leaves the critical section.
 */
static void advance(void) {
	int rc = 0;

	/* Only increase the ticket number if there are more passes to be made */
	if (ticket_to_crit_sect < crit_sect_pass_count) {
		ticket_to_crit_sect++;
	}

	/* Let all threads know that they can check whether it's their turn to enter
	   the critical section (or that they should quit) */
	rc = pthread_cond_broadcast(&thread_can_check_its_turn_cond);
	if (rc != 0) {
		fprintf(stderr, "pthread_cond_broadcast() error: %d\n", rc);
		exit(EXIT_FAILURE); /* As specified in a discussion in WIS, exit the program */
	}

	rc = pthread_mutex_unlock(&crit_sect_mutex);
	if (rc != 0) {
		fprintf(stderr, "pthread_mutex_unlock() error: %d\n", rc);
		exit(EXIT_FAILURE); /* As specified in a discussion in WIS, exit the program */
	}

}

/*
 * Returns thread waiting time in a struct timespec structure.
 */
static struct timespec get_thread_waiting_time(unsigned *seed) {
	struct timespec sleep_spec;

	/* Genereate a number between 0.0 and MAX_WAITING_TIME_NSECS nanoseconds */
	sleep_spec.tv_sec = 0;
	sleep_spec.tv_nsec = (long) (MAX_WAITING_TIME_NSECS * (rand_r(seed) / (RAND_MAX + 1.0)));

	return sleep_spec;
}

/*
 * Thread loop (each thread will enter this loop).
 * arg must be of the type int * and must contain the thread ID.
 * Exists with a NULL pointer status.
 */
static void *thread_loop(void *arg) {
	int id = *((int *) arg);
	int ticket = 0;
	struct timespec sleep_spec;
	unsigned rand_r_seed = getpid() + id;

	while ((ticket = getticket()) < crit_sect_pass_count) {
		/* Wait a few miliseconds */
		sleep_spec = get_thread_waiting_time(&rand_r_seed);
		nanosleep(&sleep_spec, NULL);

		await(ticket);
		printf("%d\t(%d)\n", ticket, id);
		advance();

		/* Wait a few miliseconds */
		sleep_spec = get_thread_waiting_time(&rand_r_seed);
		nanosleep(&sleep_spec, NULL);
	}

	return NULL;
}

/*
 * Main.
 */
int main(int argc, char **argv) {
	int thread_count = 0;        /* Number of threads that will run */
	int tc_parsing_result = 0;   /* thread_count parsing result */
	int cspc_parsing_result = 0; /* crit_sect_pass_count parsing result */
	pthread_t *threads = 0;      /* List of created threads */
	int *thread_ids = 0;         /* List of created threads IDs */
	pthread_attr_t thread_attrs; /* Thread attributes */
	int i = 0;                   /* For loop counter */
	int rc = 0;                  /* Return code */
    /* Disable buffering on stdout */
    setbuf(stdout, NULL);

	/* Program arguments check and parsing */
	if (argc != 3) {
		/* Invalid number of arguments */
		print_help(argv[0]);
		return EXIT_FAILURE;
	}

	/* There are 3 arguments, so parse them... */
	tc_parsing_result = str2int(argv[1], &thread_count);
	cspc_parsing_result = str2int(argv[2], &crit_sect_pass_count);

	/* ...and check whether they are valid */
	if (0 == tc_parsing_result || 0 == cspc_parsing_result ||
			!in_interval(thread_count, 1, INT_MAX) ||
			!in_interval(crit_sect_pass_count, 0, INT_MAX)) {
		/* Invalid arguments */
		print_help(argv[0]);
		return EXIT_FAILURE;
	}

	/* Allocate dynamic memory for the desired number of threads */
	threads = (pthread_t *) malloc(thread_count * sizeof(pthread_t));
	thread_ids = (int *) malloc(thread_count * sizeof(int));
	if (threads == NULL || thread_ids == NULL) {
		fprintf(stderr, "insufficient memory\n");
		return EXIT_FAILURE;
	}

	/* Create attributes for all threads (to make all threads joinable) */
	rc = pthread_attr_init(&thread_attrs);
	if (rc != 0) {
		/* Cleanup */
		free(threads);
		free(thread_ids);
		fprintf(stderr, "pthread_attr_init() error: %d\n", rc);
		return EXIT_FAILURE;
	}
	rc = pthread_attr_setdetachstate(&thread_attrs, PTHREAD_CREATE_JOINABLE);
	if (rc != 0) {
		/* Cleanup */
		free(threads);
		free(thread_ids);
		pthread_attr_destroy(&thread_attrs);
		fprintf(stderr, "pthread_attr_setdetachstate() error: %d\n", rc);
		return EXIT_FAILURE;
	}

	/* Create the selected number of threads */
	for (i = 0; i < thread_count; ++i) {
		thread_ids[i] = i + 1;

		rc = pthread_create(&threads[i], &thread_attrs, thread_loop, &thread_ids[i]);
		if (rc != 0) {
			pthread_attr_destroy(&thread_attrs);
			fprintf(stderr, "pthread_create() error: %d\n", rc);
			return EXIT_FAILURE;
		}
	}

	/* Thread attributes are not necessary anymore, so destroy them */
	pthread_attr_destroy(&thread_attrs);

	/* Wait until all threads terminates */
	for (i = 0; i < thread_count; ++i) {
		rc = pthread_join(threads[i], NULL);
		if (rc != 0) {
			fprintf(stderr, "pthread_join() error: %d\n", rc);
			return EXIT_FAILURE;
		}
	}

	/* Cleanup */
	free(threads);
	free(thread_ids);

	return EXIT_SUCCESS;
}

/* End of file */
