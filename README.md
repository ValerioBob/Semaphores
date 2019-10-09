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
- [ ] **Funzione internal_semPost()**
- [ ] **Funzione internal_semWait()**
- [ ] **Test**

`
