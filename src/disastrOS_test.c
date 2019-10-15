#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "disastrOS.h"
#include "disastrOS_constants.h"

#define SEMAPHORE_PRODUCER_VALUE 10
#define SEMAPHORE_CONSUMER_VALUE 0

//aggiunte funzioni consumer/producer
void producer(int producer, int consumer)
{
    for (int i = 0; i < 5; i++)
    {
        disastrOS_semWait(producer);
        printf("-------------------------------------I'm the producer\n\n\n\n");
        disastrOS_semPost(consumer);
    }
}

void consumer(int producer, int consumer)
{
    for (int i = 0; i < 5; i++)
    {
        disastrOS_semWait(consumer);
        printf("-------------------------------------I'm the consumer\n\n\n\n");
        disastrOS_semPost(producer);
    }
}

// we need this to handle the sleep state
void sleeperFunction(void *args)
{
    printf("Hello, I am the sleeper, and I sleep %d\n", disastrOS_getpid());
    while (1)
    {
        getc(stdin);
        disastrOS_printStatus();
    }
}

void childFunction(void *args)
{
    printf("Hello, I am the child function %d\n", disastrOS_getpid());
    // int type = 0;
    // int mode = 0;
    // int fd = disastrOS_openResource(disastrOS_getpid(), type, mode);

    printf("opening semaphores\n");

    int prod_id_1 = disastrOS_semOpen(1, SEMAPHORE_PRODUCER_VALUE); //semaforo 1 = producer valore 10
    int cons_id_1 = disastrOS_semOpen(2, SEMAPHORE_CONSUMER_VALUE); // semaforo 2 = consumer valore 0

    // printf("PID :%d", disastrOS_getpid());

    if (disastrOS_getpid() % 2 == 0)
    {
        producer(prod_id_1, cons_id_1);
    }
    else
    {
        consumer(prod_id_1, cons_id_1);
    }
    printf("pid=%d is terminating\n", disastrOS_getpid());

    printf("closing semaphores!\n");
    disastrOS_semClose(prod_id_1);
    disastrOS_semClose(cons_id_1);
    disastrOS_exit(disastrOS_getpid() + 1);
}

void initFunction(void *args)
{
    disastrOS_printStatus();
    printf("Hello, I am init and I just started\n");
    disastrOS_spawn(sleeperFunction, 0);
    printf("I feel like to spawn 10 nice threads\n");
    int alive_children = 0;

    for (int i = 0; i < 3; ++i)
    {
        int type = 0;
        int mode = DSOS_CREATE;
        // printf("mode: %d\n", mode);
        // printf("opening resource (and creating if necessary)\n");
        int fd = disastrOS_openResource(i, type, mode);
        printf("fd=%d\n", fd);
        disastrOS_spawn(childFunction, 0);
        alive_children++;
    }

    disastrOS_printStatus();
    int retval;
    int pid;
    while (alive_children > 0 && (pid = disastrOS_wait(0, &retval)) >= 0)
    {
        disastrOS_printStatus();
        printf("initFunction, child: %d terminated, retval:%d, alive: %d \n",
               pid, retval, alive_children);
        --alive_children;
    }
    disastrOS_printStatus();
    printf("***SHUTDOWN!***\n");
    disastrOS_shutdown();
}

int main(int argc, char **argv)
{
    char *logfilename = 0;
    if (argc > 1)
    {
        logfilename = argv[1];
    }
    // we create the init process processes
    // the first is in the running variable
    // the others are in the ready queue
    printf("the function pointer is: %p", childFunction);
    // spawn an init process
    printf("start\n");
    disastrOS_start(initFunction, 0, logfilename);
    return 0;
}