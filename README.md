# PS Operating System Templates
## Pipes
- unidirectional (read and write-end)\
- file-like interface
- daten können nur einmal gelesen werden

### Anonymous / Unnamed Pipe

- keinen Namen (anonym) und kann daher nur mit fork() erzeugt werden
  - without forking the pipe connects process to itself
  - after forking, parent and child connected via pipe
- pipefd[0] -> refers to read-end of the pipe
- pipefd[1] -> refers to write-end of the pipe

Code Example: [Unnamed Pipe](/pipes/unnamed_pipe.c)

### Named Pipe (FIFO)

- ähnlich zu normalen files
  -Daten können nur einmal gelesen werden
- können von jedem Prozess geöffnet werden
- müssen an beiden Enden geöffnet werden bevord Daten ausgetauscht werden können

Code Example: [Named Pipe](/pipes/named_pipe.c) 

## Message Queue
- stellen packet semantics zur Verfügung
- Pakete fester Größe können ohne zusätzliche Logik gesendet und empfangen werden
- Pakete werden in der gleichen Reihenfolge zugestellt, wie sie gesendet wurden
- erlaubt verschiedene Prioritäten für messages
  - higher priority message werden zuerst gesendet

Code Example: [Scheduler](/message_queue/scheduler.c) [Service](/message_queue/service.c)

## Shared Memory
- jeder prozess hat eigenen Speicher, welcher nicht geshared wird
- shared memory objects sind über ihren Namen identifizierbar, aber nit Teil vom Filesystem
  - mehrere Prozesse können Zugriff auf den shared memory beantragen
- Zugriff muss synchronisiert werden

Code Example: [Shared Memory](/shared_memory/shared_memory.c)

## Synchronisation Methods
### Atomics
- benötigt hardware support
- atomic operations passieren nur ganz oder gar nicht
- kann verwendet, um einem anderen Prozess über einen gemeinsam genutzten Speicher ein sicheres Signal zu geben
- primitive atomic types können wir normale Variablen deklariert und verwendet werden
- Zugriff muss via spezielle API Functionen stattfinden (atomic_store, atomic_load, ...)

### Mutex
- werden auch locks genannt
- haben zwei Zustände
  - locked / held / owned
  - unlocked / free / released
- nur ein Thread kann einen Mutex zum Zeitpunkt "halten"
---

- genutzt um kritische Bereiche im Code zu beschützen
- es kann nur der Thread, der die Mutex haltet, den kritischen Bereich betreten
- ein Thread sollte die Mutex nicht mehr als einmal locken, sonst droht undefiniertes Verhalten

Code Example: [Mutex](/pthread_mutex_cond/mutex.c)

### Condition Variables
- wird verwendet, um zu blockieren/warten, bis eine spezielle condition erfüllt wird
- die condition variable wird dann signalisiert, wenn die condition erfüllt wird
---

- Achtung! Spurious wakeups -> condition variable wurde signalisiert, obohl keine gesetzte condition erfüllt wurde
- Condition variables müssen in Verbindung mit einem Mutex verwendet werden

Code Example: [Condition Variables](/pthread_mutex_cond/mutex_cond.c)

### Semaphores
- ziemlich gleich zu Mutexes
- besitzen counting semantic (semaphore wird als number repräsentiert und mit 0 initialisiert)
- post / release: Increments number by 1 -> semantically unlocks the semaphore
- wait / aquire: Decrements number by 1 -> semantically locks the semaphore
  - kann nur dekrementiert werden, falls der Wert größer als 0 ist
---

- eine Semaphore, welche nur die Werte 0 und 1 hat, verhält sich ca. gleich wie ein Mutex

Code Example: [Semaphore](/semaphore/semaphore.c), [Named Semaphore](/semaphore/named_semaphore.c)

### Barriers 
- synchronisiert alle beteiligten Threads
- blockiert, bis alle Threads an Barrier angelangt sind
- ein Thread wird als PTHREAD_BARRIER_SERIAL_THREAD zurückgegeben
  - kann benutzt werden, um etwas auszuführen, was nur einmal ausgeführt oder nur von einem Thread ausgeführt werden soll -> alle anderen geben den Wert 0 zurück

Code Example: [Barrier](/pthread_barrier/pthread_barrier.c)

| Kriterium                | Atomics                                           | Mutex                                                              | Condition Variables                                               | Semaphores                                              | Barriers                                         |
|--------------------------|---------------------------------------------------|--------------------------------------------------------------------|-------------------------------------------------------------------|---------------------------------------------------------|--------------------------------------------------|
| Definition               | Operationen, die atomar (ununterbrechbar) sind    | Sperrmechanismus zum Schutz kritischer Abschnitte                  | Mechanismus, um Threads zu blockieren un auf ein Signal zu warten | Zählermechanismus zum Verwalten von Ressourcenzugriffen | Synchronisationspunkt für Threads                |
| Zweck                    | Vermeidung von Race Conditions bei Variablen      | Vermeidung von Race Conditions durch Sperren                       | Wartende Threads wecken, wenn eine Bedingung erfüllt is           | Kontrolle über begrenzte Ressourcen                     | Synchronisation von Threads                      |
| Typische Funktionen in C | atomic_load, atomic_store, atomic_fetch_add, etc. | pthread_mutex_init, pthread_mutex_lock, pthread_mutex_unlock, etc. | pthread_cond_init, pthread_cond_wait, pthread_cond_signal, etc.   | sem_init, sem_wait, sem_post, etc.                      | pthread_barrier_init, pthread_barrier_wait, etc. |
| Blockierung              | Nein                                              | Ja                                                                 | Ja                                                                | Ja                                                      | Ja                                               |

## Linking and Libraries
### Dynamic Linking
- Laden von Code, der zur Kompilierzeit nicht verfügbar war (Plugins)
- Lazy loading von Code -> laden nur bei tatsächlichem Bedarf

![image](/images/dyn_linking.png)

### Static Shared Libraries
- eine systemweite Vorabzuweisung innerhalb des Adressraums erforderlich
- Linker verknüpft das Programm mit der Bibliothek, enthält aber nicht den eigentlichen Code
- der Loader markiert das Segment der gemeinsam genutzten Bibliothek als "unmapped"
- bei Zugriff -> segfault: Linker lädt die entsprechende Bibliothek und mappt sie in seinen Speicherbereich
---

- Verschiedene Programme können gleichen Code sharen

### Dynamic Shared Libraries
- jede library kann sich an jeder beliebigen Adresse im Adressraum befinden
---

- Position Independent Code -> Code muss an jeder virtuellen Adresse ausführbar sein