# Progetto Sistemi Operativi

## Implementazione semafori in disastrOS

- [ ] **Gestione errori/success**
  - handle_error
  - handle_success
- [ ] **Funzione internal_semOpen()**
  - verifica dell'utilizzo del semaforo con gestione errore
  - verifica della corretta apertura del semaforo
  - verifica che il semaforo non esiste
  - verifica dell'allocazione del semaforo con gestione errore
  - inserimento in lista
  - creazione descriptor con gestione errore
  - inserimento del descriptor nel pcb
  - inserimento del puntatore del descriptor nella struct sem
  - set di return value
- [ ] **Funzione internal_semClose()**
- [ ] **Funzione internal_semPost()**
- [ ] **Funzione internal_semWait()**
- [ ] **Test**

`
