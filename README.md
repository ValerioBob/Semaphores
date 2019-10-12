# Progetto Sistemi Operativi

## Implementazione semafori in disastrOS

- [x] **Gestione errori/success**
  - handle_error
  - handle_success
- [x] **Funzione internal_semOpen()**
  - verifica dell'utilizzo del semaforo con gestione errore
  - verifica della corretta apertura del semaforo
  - verifica che il semaforo non esiste
  - verifica dell'allocazione del semaforo con gestione errore
  - inserimento in lista
  - creazione descriptor con gestione errore
  - inserimento del descriptor nel pcb
  - inserimento del puntatore del descriptor nella struct sem
  - set di return value
- [x] **Funzione internal_semClose()**
  - id del semaforo da chiudere e il rispettivo sem descriptor con gestione errori
  - rimozione del sem descriptor dalla lista dei descriptor con gestione errori
  - puntatore al semaforo da passare alla list_detach() con gestione errori
  - pulizia di tutto tra cui rimozione del puntatore al fd e le varie free
- [x] **Funzione internal_semPost()**

  - id del semaforo da postare e il rispettivo sem descriptor con gestione errori
  - puntatore al semaforo da incrementare
  - controllo del valore del semaforo
  - metto il processo in coda di ready
  - rimuovo il descriptor dalla lista di waiting e lo metto nella lista dei descriptor
  - rimuovo il processo dalla lista di waiting
  - setto running->status a ready
  - setto running al pcb per far partire il processo

- [x] **Funzione internal_semWait()**
  - id del semaforo da postare e il rispettivo sem descriptor con gestione errori
  - puntatore al semaforo da decrementare
  - controllo del valore del semaforo
  - rimuovo il descrittore del processo dalla lista di ready
  - inserisco il processo nella lista di waiting
  - prendo il primo processo nella coda di ready
  - setto running= pcb per far partire il processo
- [x] **Test**

  - funzioni producer/ consumer
  - modifica di child function
  - aggiunta Semaphore_init() e semDescriptor_init() in disastros start
  - aggiunta di chiamate a syscall semafori in disastros.c e dichiarazione in disastros.h

### handle_error e handle_success

Sono state definite due funzioni per la gestione degli errori. 

handle_error stampa sullo stderr un messaggio di errore contenente il codice dell'errore (intero negativo) e un breve messaggio. Dopo la stampa setta syscall_retvalue contenuto nella struct running con valore l'errore.

handle_success invece stampa sullo stdout un messaggio contenente la syscall che ha avuto successo e successivamente  setta syscall_retvalue a 0.



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