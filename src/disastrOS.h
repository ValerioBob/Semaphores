#pragma once
#include "disastrOS_pcb.h"
#include "linked_list.h"

#ifdef _DISASTROS_DEBUG_
#include <stdio.h>

#define disastrOS_debug(...) printf(__VA_ARGS__)

#else //_DISASTROS_DEBUG_

#define disastrOS_debug(...) ;

#endif //_DISASTROS_DEBUG_

#define handle_error(error, msg)                                      \
    do                                                                \
    {                                                                 \
        fprintf(stderr, "[ERROR] error number %d, %s\n", error, msg); \
        running->syscall_retvalue = error;                            \
        disastrOS_exit(error);                                        \
        return;                                                       \
    } while (0);

#define handle_success(msg)                  \
    do                                       \
    {                                        \
        fprintf(stdout, "[DONE] %s\n", msg); \
        running->syscall_retvalue = 0;       \
        return;                              \
    } while (0);
// initializes the structures and spawns a fake init process
void disastrOS_start(void (*f)(void *), void *args, char *logfile);

// generic syscall
int disastrOS_syscall(int syscall_num, ...);

// classical process control
int disastrOS_getpid(); // this should be a syscall, but we have no memory separation, so we return just the running pid
int disastrOS_fork();
void disastrOS_exit(int exit_value);
int disastrOS_wait(int pid, int *retval);
void disastrOS_preempt();
void disastrOS_spawn(void (*f)(void *), void *args);
void disastrOS_shutdown();

// timers
void disastrOS_sleep(int);

// respurces (files)
int disastrOS_openResource(int resource_id, int type, int mode);
int disastrOS_closeResource(int fd);
int disastrOS_destroyResource(int resource_id);

// debug function, prints the state of the internal system
void disastrOS_printStatus();

//semafori
int disastrOS_semOpen(int id, int count);
int disastrOS_semClose(int desc);
int disastrOS_semWait(int desc);
int disastrOS_semPost(int desc);