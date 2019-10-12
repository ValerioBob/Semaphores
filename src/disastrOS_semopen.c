#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include "disastrOS.h"
#include "disastrOS_syscalls.h"
#include "disastrOS_semaphore.h"
#include "disastrOS_semdescriptor.h"

void internal_semOpen()
{
    int sem_id = running->syscall_args[0];
    int sem_count = running->syscall_args[1];

    if (sem_count < 0) //verifico il valore
    {
        handle_error(DSOS_ESEMBUSY, "semaphore value negative \n");
        return;
    }

    Semaphore *sem = SemaphoreList_byId(&semaphores_list, sem_id);
    //controllo l'apertura del semaforo
    if (!sem)
    {
        //caso in cui il semaforo non esiste
        sem = Semaphore_alloc(sem_id, sem_count); //alloco il semaforo
        //devo controllare se l'allocazione e' avvenuta con successo
        if (!sem)
        {
            handle_error(DSOS_ESEMOPEN, "can't allocate semaphore");
            return;
        }

        //inserimento del semaforo in lista
        List_insert(&semaphores_list, semaphores_list.last, (ListItem *)sem);
    }

    //creo descriptor
    SemDescriptor *desc = SemDescriptor_alloc(running->last_sem_fd, sem, running);
    //caso errore
    if (!desc)
    {
        handle_error(DSOS_SEMDESCALLOC, "can't allocate sem descriptor");
        return;
    }

    running->last_sem_fd++;

    //inserisco descriptor nel PCB
    List_insert(&running->sem_descriptors, running->sem_descriptors.last, (ListItem *)desc);

    SemDescriptorPtr *desc_ptr = SemDescriptorPtr_alloc(desc);

    desc->ptr = desc_ptr; //inserisco il puntatore nella struct

    //inserisco il ptr del desc nella struct sem
    List_insert(&sem->descriptors, sem->descriptors.last, (ListItem *)desc_ptr);

    //setto retval
    running->syscall_retvalue = desc->fd;
    fprintf(stdout, "[DONE] sem open success\n");
}