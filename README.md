# bs_templates
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


## misc:

- [argument validation](/argument_validation/argument_validation.c) for {strtod, strtof, strtol, strtold, strtoll, strtoul, strtoull}

- [Client/Server](/client_server/) using TCP sockets

- [Makefile](/makefile/)

- pipes [named](/pipes/named_pipe.c) [unnamed](/pipes/unnamed_pipe.c) using fork

- [signal](/signal_handling/sigaction.c)

- [thread_handling](/thread_handling/pthread_handling.c)

## syncronisation:

- semaphore [unnamed](/semaphore/semaphore.c) [named](/semaphore/named_semaphore.c)

- [mutex](/pthread_mutex_cond/mutex.c) [mutex+cond](/pthread_mutex_cond/mutex_cond.c)
