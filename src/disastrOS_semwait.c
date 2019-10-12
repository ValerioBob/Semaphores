#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semWait()
{
    int sem_id = running->syscall_args[0];

    SemDescriptor *descr = SemDescriptorList_byFd(&running->sem_descriptors, sem_id);

    if (!descr)
    {
        handle_error(DSOS_ESEMWAIT, "sem descriptor not found");
        return;
    }

    Semaphore *sem = descr->semaphore;
    SemDescriptorPtr *descr_ptr = descr->ptr;

    sem->count--;

    if (sem->count < 0)
    {
        List_detach(&sem->descriptors, (ListItem *)descr_ptr);                                         //rimuovo il desc del proc dalla lista dei desc
        List_insert(&sem->waiting_descriptors, sem->waiting_descriptors.last, (ListItem *)descr->ptr); //inserisco il proc nella coda di attesa

        running->status = Waiting;

        List_insert(&waiting_list, waiting_list.last, (ListItem *)running); //inserisco il processo in esecuzione nella wait list

        PCB *pcb = (PCB *)List_detach(&ready_list, (ListItem *)ready_list.first); //prendo il primo processo ready

        running = (PCB *)pcb;
    }
    handle_success("sem_wait success!");
    return;
}
