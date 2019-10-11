#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semPost()
{
    int sem_id = running->syscall_args[0];

    SemDescriptor *descr = SemDescriptorList_byFd(&running->sem_descriptors, sem_id);
    if (!descr)
    {
        handle_error(DSOS_ESEMDESCR, "sem descriptor not found");
        return;
    }

    Semaphore *sem = descr->semaphore;
    if (!sem)
    {
        handle_error(DSOS_ESEMPOST, "sem post failed");
        return;
    }
    sem->count++;

    if (sem->count <= 0)
    {
        List_insert(&ready_list, ready_list.last, (ListItem *)running);

        SemDescriptorPtr *descr_ptr = (SemDescriptorPtr *)List_detach(&sem->waiting_descriptors, (ListItem *)sem->waiting_descriptors.first);

        List_insert(&sem->descriptors, sem->descriptors.last, (ListItem *)descr_ptr);

        List_detach(&waiting_list, (ListItem *)descr_ptr->descriptor->pcb);

        running->status = Ready;
        running = descr_ptr->descriptor->pcb;
        handle_success("semPost success");
        return;
    }
