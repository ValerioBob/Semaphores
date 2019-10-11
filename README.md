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
- [ ] **Test**

`
