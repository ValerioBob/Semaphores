# Progetto Sistemi Operativi

## Implementazione semafori in disastrOS

### handle_error e handle_success

Sono state definite due funzioni per la gestione degli errori.

handle_error stampa sullo stderr un messaggio di errore contenente il codice dell'errore (intero negativo) e un breve messaggio. Dopo la stampa setta syscall_retvalue contenuto nella struct running con valore l'errore e chiama la funzione disastrOS_exit() per terminare l'esecuzione.
handle_success invece stampa sullo stdout un messaggio contenente la syscall che ha avuto successo e successivamente setta syscall_retvalue a 0.

### internal_semOpen()

Questa funzione crea un semaforo identificato da un id e un valore.

La funzione verifica inizialmente se il valore passato. Nel caso di valore negativo la funzione ritorna un messaggio di errore. Subito dopo verifica l'esistenza di un semaforo con quell'id utilizzando la funzione SemaphoreList_byId. Passate queste verifiche la funzione puo' allocare il semaforo con id e valore e successivamente inserirla nella lista dei semafori, dopodiche' crea un descrittore al quale inserisce il puntatore al semaforo appena creato. Viene settato il return value della syscall con l'id del semaforo appena creato

### internal_semClose()

Questa funzione chiude il semaforo identificato dall'id passato.

Ovviamente la funzione verifica se il semaforo passato sia esistente prendendo il suo descriptor e verificando non sia nullo. Successivamente viene il suo descrittore viene rimosso dalla lista dei sem_descriptors del processo corrente e viene rimosso anche il suo puntatore dalla lista dei descriptors. Successivamente puo' essere rimosso il semaforo dalla lista dei semafori e infine liberata la memoria utilizzata.

### internal_semPost()

Questa funzione incrementa il valore del semaforo.

La funzione verifica l'esistenza del semaforo verificando l'esistenza del suo descriptor tramite la funzione SemDescriptorList_byFd. In caso non esiste viene stampato un messaggio di errore. Successivamente viene incrementato il valore del semaforo. Se il valore di count e' minore o uguale a 0 la fuznione inserisce il processo corrente nella ready_list in ultima posizione e setta il suo stato a ready. Il puntatore del al suo descrittore viene rimosso dalla waiting_descriptors e viene inserito nella lista dei descriptor. Successivamente viene preso e rimosso il primo processo dalla coda di waiting, viene settato lo status di running a ready e solo dopo il processo puo' essere mandato in esecuzione.

### internal_semWait()

Questa funzione decrementa il valore del semaforo.

La funzione verifica l'esistenza del semaforo verificando l'esistenza del suo descriptor tramite la funzione SemDescriptorList_byFd. In caso non esiste viene stampato un messaggio di errore. Successivamente viene decrementato il valore del semaforo. Se il valore del semaforo e' minore a 0 il suo descrittore viene rimosso dalla lista dei descrittori e viene inserito nella lista dei waiting_descriptors. Lo status del processo viene settato a Waiting. Successivamente viene preso il primo processo dalla coda di ready e viene mandato in esecuzione.
