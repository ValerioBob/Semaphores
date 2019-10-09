#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semClose()
{
    int sem_id = running->syscall_args[0];
    SemDescriptor *descr = SemDescriptorList_byFd(&running->sem_descriptors, sem_id); //prendo il descriptor del sem tramite il suo id

    if (!descr)
    {
        handle_error(DSOS_ESEMDESCR, "semaphore descriptor not found");
        return;
    }

    descr = (SemDescriptor *)List_detach(&running->sem_descriptors, (ListItem *)descr); //detach del descr

    Semaphore *sem = descr->semaphore; //puntatore al semaforo da passare al list_detach()

    if (!sem)
    {
        handle_error(DSOS_ESEMCLOSE, "error in semaphore pointer");
        return;
    }

    SemDescriptorPtr *descr_ptr = (SemDescriptorPtr *)List_detach(&sem->descriptors, (ListItem *)(descr->ptr));
    if (!descr_ptr)
    {
        handle_error(DSOS_ESEMDESCRDETACH, "can't detach sem descriptor pointer");
        return;
    }
    List_detach(&semaphores_list, (ListItem *)sem); //rimosso il semaforo
    //free di tutto
    SemDescriptor_free(descr);
    SemDescriptorPtr_free(descr_ptr);
    handle_success("semaphore closed");
    running->syscall_retvalue = 0;

    return;
}
