<!-- TOC start (generated with https://github.com/derlin/bitdowntoc) -->

- [PS Operating System Templates](#ps-operating-system-templates)
  * [Pipes](#pipes)
    + [Anonymous / Unnamed Pipe](#anonymous-unnamed-pipe)
    + [Named Pipe (FIFO)](#named-pipe-fifo)
  * [Message Queue](#message-queue)
  * [Shared Memory](#shared-memory)
  * [Synchronisation Methods](#synchronisation-methods)
    + [Atomics](#atomics)
    + [Mutex](#mutex)
    + [Condition Variables](#condition-variables)
    + [Semaphores](#semaphores)
    + [Barriers ](#barriers)
  * [Linking and Libraries](#linking-and-libraries)
    + [Dynamic Linking](#dynamic-linking)
    + [Static Shared Libraries](#static-shared-libraries)
    + [Dynamic Shared Libraries](#dynamic-shared-libraries)
  * [Miscellaneus](#miscellaneus)

<!-- TOC end -->

<!-- TOC --><a name="ps-operating-system-templates"></a>
# PS Operating System Templates
<!-- TOC --><a name="pipes"></a>
## Pipes
- unidirectional (read and write-end)\
- file-like interface
- daten können nur einmal gelesen werden

<!-- TOC --><a name="anonymous-unnamed-pipe"></a>
### Anonymous / Unnamed Pipe

- keinen Namen (anonym) und kann daher nur mit fork() erzeugt werden
  - without forking the pipe connects process to itself
  - after forking, parent and child connected via pipe
- pipefd[0] -> refers to read-end of the pipe
- pipefd[1] -> refers to write-end of the pipe

Code Example: [Unnamed Pipe](/pipes/unnamed_pipe.c)

<!-- TOC --><a name="named-pipe-fifo"></a>
### Named Pipe (FIFO)

- ähnlich zu normalen files
  -Daten können nur einmal gelesen werden
- können von jedem Prozess geöffnet werden
- müssen an beiden Enden geöffnet werden bevord Daten ausgetauscht werden können

Code Example: [Named Pipe](/pipes/named_pipe.c), [Functions](/pipes/fifo_functions.h)

<!-- TOC --><a name="message-queue"></a>
## Message Queue
- stellen packet semantics zur Verfügung
- Pakete fester Größe können ohne zusätzliche Logik gesendet und empfangen werden
- Pakete werden in der gleichen Reihenfolge zugestellt, wie sie gesendet wurden
- erlaubt verschiedene Prioritäten für messages
  - higher priority message werden zuerst gesendet

Code Example: [Scheduler](/message_queue/scheduler.c), [Service](/message_queue/service.c), [Functions](/message_queue/mq_functions.h)

<!-- TOC --><a name="shared-memory"></a>
## Shared Memory
- jeder prozess hat eigenen Speicher, welcher nicht geshared wird
- shared memory objects sind über ihren Namen identifizierbar, aber nit Teil vom Filesystem
  - mehrere Prozesse können Zugriff auf den shared memory beantragen
- Zugriff muss synchronisiert werden

Code Example: [Shared Memory](/shared_memory/shared_memory.c)

<!-- TOC --><a name="synchronisation-methods"></a>
## Synchronisation Methods
<!-- TOC --><a name="atomics"></a>
### Atomics
- benötigt hardware support
- atomic operations passieren nur ganz oder gar nicht
- kann verwendet, um einem anderen Prozess über einen gemeinsam genutzten Speicher ein sicheres Signal zu geben
- primitive atomic types können wir normale Variablen deklariert und verwendet werden
- Zugriff muss via spezielle API Functionen stattfinden (atomic_store, atomic_load, ...)

<!-- TOC --><a name="mutex"></a>
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

<!-- TOC --><a name="condition-variables"></a>
### Condition Variables
- wird verwendet, um zu blockieren/warten, bis eine spezielle condition erfüllt wird
- die condition variable wird dann signalisiert, wenn die condition erfüllt wird
---

- Achtung! Spurious wakeups -> condition variable wurde signalisiert, obohl keine gesetzte condition erfüllt wurde
- Condition variables müssen in Verbindung mit einem Mutex verwendet werden

Code Example: [Condition Variables](/pthread_mutex_cond/mutex_cond.c)

<!-- TOC --><a name="semaphores"></a>
### Semaphores
- ziemlich gleich zu Mutexes
- besitzen counting semantic (semaphore wird als number repräsentiert und mit 0 initialisiert)
- post / release: Increments number by 1 -> semantically unlocks the semaphore
- wait / aquire: Decrements number by 1 -> semantically locks the semaphore
  - kann nur dekrementiert werden, falls der Wert größer als 0 ist
---

- eine Semaphore, welche nur die Werte 0 und 1 hat, verhält sich ca. gleich wie ein Mutex

Code Example: [Semaphore](/semaphore/semaphore.c), [Named Semaphore](/semaphore/named_semaphore.c)

<!-- TOC --><a name="barriers"></a>
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

<!-- TOC --><a name="linking-and-libraries"></a>
## Linking and Libraries
<!-- TOC --><a name="dynamic-linking"></a>
### Dynamic Linking
- Laden von Code, der zur Kompilierzeit nicht verfügbar war (Plugins)
- Lazy loading von Code -> laden nur bei tatsächlichem Bedarf

![image](/images/dyn_linking.png)

<!-- TOC --><a name="static-shared-libraries"></a>
### Static Shared Libraries
- eine systemweite Vorabzuweisung innerhalb des Adressraums erforderlich
- Linker verknüpft das Programm mit der Bibliothek, enthält aber nicht den eigentlichen Code
- der Loader markiert das Segment der gemeinsam genutzten Bibliothek als "unmapped"
- bei Zugriff -> segfault: Linker lädt die entsprechende Bibliothek und mappt sie in seinen Speicherbereich
---

- Verschiedene Programme können gleichen Code sharen

<!-- TOC --><a name="dynamic-shared-libraries"></a>
### Dynamic Shared Libraries
- jede library kann sich an jeder beliebigen Adresse im Adressraum befinden
---

- Position Independent Code -> Code muss an jeder virtuellen Adresse ausführbar sein

<!-- TOC --><a name="miscellaneus"></a>
## Miscellaneus
[Thread Handling](/thread_handling/pthread_handling.c)</br>
[Signal Handling](/signal_handling/sigaction.c)</br>
[Fork](/fork/fork.c)</br>
[Argument Validation](/argument_validation/argument_validation.c)</br>
[Queue](/datastructures/myqueue.h), [Queue with Pair](/datastructures/myqueue_pair.h)</br>
[Thread Pool](/thread_pool/thread_pool.c)</br>
[Client Server](/client_server), [Socket Functions](/client_server/socket_functions.h)</br>
[Makefiles](/makefile)