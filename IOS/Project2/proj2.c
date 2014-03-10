/*
 * Encoding: UTF-8
 * File:     proj2.c
 * Project:  IOS - Project 2
 * Date:     1.5.2006
 * Authors:  Petr Zemek, xzemek02@stud.fit.vutbr.cz 
 *           Michal Ingeli, xingel00@stud.fit.vutbr.cz 
 *           Ivan Vogel, xvogel01@stud.fit.vutbr.cz              
 * Description: Process synchronisation - 3 processes are forked off
 *              and they are changing number in shared memory segment.
 *              Then they write their status and after short period 
 *              (depends on 'NUM_MAX' constant), they all terminate.
 *              Synchronisation is done by signal sendings.
 */
 
#include <stdio.h>                  /* Standard I/O functions                */
#include <stdlib.h>                 /* General purpose standard C library    */
#include <stdarg.h>                 /* Functions with variabel param. count  */
#include <signal.h>                 /* Signals handling                      */
#include <sys/types.h>              /* Various type definitions              */
#include <sys/ipc.h>                /* General IPC structures                */
#include <sys/shm.h>                /* Shared memory functions and structs   */
#include <unistd.h>                 /* Standard symbolic constants and types */

/*
 * Constants, symbols and types
 ******************************************************************************
 */
const int NUM_MIN = 0;                  /* Starting number                   */
const int NUM_MAX = 9;                  /* Ending number                     */
const int INCREMENT_1 = 10;             /* Child1 incrementation rate        */
const int INCREMENT_2 = 100;            /* Child2 incrementation rate        */    
const size_t SEG_SIZE = sizeof(int);    /* Size of shared memory segment     */
 
/* Structure containing info about shared memory segment */  
typedef struct {
    int id;                            /* Share memory ID                    */
    int *addr;                         /* Share memory adress                */
} shm_info_t; 
 
/*
 * Global variables
 ******************************************************************************
 */
volatile sig_atomic_t action;          /* Signal number (set in handler)     */
 
/*
 * Function prototypes - processes
 ******************************************************************************
 */ 
void parent(pid_t child1_pid, shm_info_t shm_seg);
void child1(shm_info_t shm_seg);
void child2(shm_info_t shm_seg);
 
/*
 * Atexit functions - print status 'finished' when process ends
 ******************************************************************************
 */ 
void atexit_parent(void) { printf("parent: %d: finished\n", getpid()); }
void atexit_child1(void) { printf("child1: %d: finished\n", getpid()); }
void atexit_child2(void) { printf("child2: %d: finished\n", getpid()); }

/*
 * Signal handling
 ******************************************************************************
 */
void set_action(int signum) {   
    // set 'action' according to 'signum'
    action = signum;     
}

/*
 * Process functions, main() and other
 ******************************************************************************
 */

/*
 * De-attach the given memory segment.
 */
inline void free_shmem(shm_info_t *shm_seg) {
    shmdt(shm_seg->addr);          
    shmctl(shm_seg->id, IPC_RMID, NULL);
}

/*
 * Print format string 'fmt' to standard error. 
 * Function has variable argument count (like printf).
 */
void error(const char *fmt, ...) {  
    va_list args;   
  
    va_start(args, fmt);  
    fprintf(stderr, "Proj2: ");    
    vfprintf(stderr, fmt, args);  
    va_end(args);    
}

/*
 * Parent process
 */
void parent(pid_t child1_pid, shm_info_t shm_seg) {
    // blocking mask
    sigset_t block_mask;
 
    // set up blocking mask 
    sigfillset(&block_mask);   
    sigdelset(&block_mask, SIGUSR1);
    sigdelset(&block_mask, SIGCHLD);
    sigdelset(&block_mask, SIGINT);     
    
    // set atexit() function
    atexit(atexit_parent);
    
    // loop - generate numbers (0..9)
    for (int num = NUM_MIN; num <= NUM_MAX; ++num) {       
        // write number to shared memory        
        *shm_seg.addr = num;       
        
        // pass control to child1                 
        kill(child1_pid, SIGUSR1);
                
        // wait for a signal
        sigsuspend(&block_mask); 
                                           
        // do action according to 'action' variable
        switch (action) {
            case SIGUSR1:
                // print status
                printf("parent: %d: %d\n", getpid(), *shm_seg.addr);
                break;
            default:
                break;            
        }                                  
    }   
    
    // send SIGTERM to child1
    kill(child1_pid, SIGTERM); 
    
    // and wait until child terminates itself
    sigsuspend(&block_mask);   
} 

/*
 * Child1 process
 */
void child1(shm_info_t shm_seg) {
    // fork off child process
    pid_t pid = fork();
    
    switch (pid) {
        // fork failed
        case -1: 
            error("fork: child1: Cannot create new process!\n");
            kill(getppid(), SIGKILL);                                                    
            exit(2);
        // child process
        case 0:       
            child2(shm_seg);   
        // parent process
        default:
            break;                         
    }       
    
    // blocking mask
    sigset_t block_mask;
           
    // set up blocking mask
    sigfillset(&block_mask);
    sigdelset(&block_mask, SIGUSR1);
    sigdelset(&block_mask, SIGUSR2);
    sigdelset(&block_mask, SIGCHLD); 
    sigdelset(&block_mask, SIGTERM);
    sigdelset(&block_mask, SIGINT);   
                
    // set atexit() function
    atexit(atexit_child1);           
     
    // save pid
    pid_t child2_pid = pid;        
           
    for (;;) {
        // wait for signal
        sigsuspend(&block_mask);       
        
        // do action according to 'action' variable
        switch (action) {
            case SIGUSR1:
                // print status
                printf("child1: %d: %d\n", getpid(), *shm_seg.addr);
    
                // increase number in shared memory
                *shm_seg.addr += INCREMENT_1;
                break;            
            case SIGTERM:
                // send SIGTERM to child2 
                kill(child2_pid, SIGTERM); 
      
                // and wait until child terminate itself
                sigsuspend(&block_mask);
    
                // terminate process
                exit(0);               
            default:
                break;            
        }
        
        // pass control to child2             
        kill(child2_pid, SIGUSR1); 
              
        // wait for signal
        // this is needed because if child2 sends SIGUSR2, nothing will
        // be printed, child1 will just send SIGUSR1 to parent process 
        sigsuspend(&block_mask);
        
        // send SIGUSR1 to parent
        kill(getppid(), SIGUSR1);                      
    }
} 

/*
 * Child2 process
 */
void child2(shm_info_t shm_seg) {    
    // blocking mask
    sigset_t block_mask;
    
    // set up blocking mask 
    sigfillset(&block_mask);   
    sigdelset(&block_mask, SIGUSR1);    
    sigdelset(&block_mask, SIGTERM);
    sigdelset(&block_mask, SIGINT);    
    
    // set atexit() function
    atexit(atexit_child2);
    
    // save child1 pid
    pid_t child1_pid = getppid();
        
    for (;;) {        
        // wait for signal
        sigsuspend(&block_mask);
        
        // do action according to 'action' variable
        switch (action) {
            case SIGUSR1:
                // print status
                printf("child2: %d: %d\n", getpid(), *shm_seg.addr);
    
                // increase number in shared memory
                *shm_seg.addr += INCREMENT_2;
        
                // pass control to parent (child1)        
                kill(child1_pid, SIGUSR2);
                break; 
            case SIGTERM:        
                // terminate process
                exit(0);   
            default:
                break;            
        }     
    }       
} 

/*
 * Main 
 */
int main(int argc, char *argv[]) {        
    // only to get rid of GCC warning message
    (void)argc;
    
    // no buffering
    setbuf(stdout, NULL);
               
    // create shared memory info structure
    shm_info_t shm_seg;      
               
    // create key for shared memory segment based on program filename  
    key_t key = ftok(argv[0], 'a');     
    if (key == -1) {
        error("ftok: Cannot create key!\n");
        exit(1);
    }
    
    // shuffle key according to process pid to avoid situatuion, 
    // when two or more processes that are running simultaneously
    // have the same key
    key %= getpid();
    
    // create shared memory segment (accessible only to the current user)
    shm_seg.id = shmget(key, SEG_SIZE, IPC_CREAT | 0600);  
    if (shm_seg.id == -1) {
        error("shmget: Cannot create shared memory segment!\n");
        exit(1);
    }    
    
    // attach the given shared memory segment (OS'll allocate it)
    shm_seg.addr = shmat(shm_seg.id, NULL, 0);
    if (shm_seg.addr == NULL) {
        error("shmat: Cannot attach shared memory segment\n");
        exit(1);
    }
                 
    // structures to specify new actions
    struct sigaction act;
    sigset_t block_all;
    
    // set up blocking mask for all processes
    // signals will be delivered only in sigsuspend()      
    sigfillset(&block_all);
    sigdelset(&block_all, SIGINT); 
    sigprocmask(SIG_BLOCK, &block_all, NULL);
           
    // establish signal handlers 
    act.sa_handler = set_action;
    act.sa_mask = block_all;
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
    sigaction(SIGCHLD, &act, NULL);
    sigaction(SIGTERM, &act, NULL);         
             
    // fork off child process
    pid_t pid = fork();
    
    switch (pid) {
        // fork failed
        case -1: 
            error("fork: Cannot create new process!\n");            
            free_shmem(&shm_seg); 
            exit(2);                        
        // child process
        case 0:
            child1(shm_seg);   
        // parent process
        default:
            parent(pid, shm_seg);
    }  
        
    // de-attach the given memory segment
    free_shmem(&shm_seg);   
        
    return 0;
}

/* End of file proj2.c */
