# SP26 CPSC 380 Programming Assignment 1 – Asynchronous Mailbox IPC

## Contributors
Brent Matthew Ortizo  
Student ID: 2452997  
Email: ortizo@chapman.edu  

Kayode Binitie  
Student ID: 2461327  
Email: binitie@chapman.edu  

---

## Description
This project implements an asynchronous mailbox using POSIX shared memory to support indirect inter-process communication (IPC) between one sender process and one receiver process. The mailbox acts as a bounded circular FIFO buffer that allows messages to be exchanged between processes created using `fork()`.

---

## Design Approach
The mailbox is stored in POSIX shared memory. Shared memory is created using `shm_open()`, sized using `ftruncate()`, and mapped into the process address space using `mmap()`.

After calling `mailbox_init()`, the program uses `fork()` to create two processes:
- The **parent process acts as the sender**
- The **child process acts as the receiver**

The sender repeatedly calls `send_mailbox()` to place messages into the mailbox, while the receiver calls `receive_mailbox()` to retrieve them. Both functions are asynchronous and return immediately with a status code.

The mailbox is implemented as a circular buffer using two indices:
- `head` – index of the next message to read  
- `tail` – index of the next slot to write  

This design preserves FIFO ordering and supports bounded buffering.

---

## Detecting Full and Empty Conditions

Mailbox **empty condition**

```
head == tail
```

Mailbox **full condition**

```
(tail + 1) % MAILBOX_CAPACITY == head
```

One slot is intentionally left unused so the implementation can distinguish between full and empty states. With a capacity of 8 slots, the mailbox can store up to 7 messages at once.

---

## Assumptions
- Exactly **one sender process and one receiver process** are used.
- Because only one process writes and one reads, additional synchronization mechanisms such as mutexes or semaphores are not required.
- If the mailbox becomes full or empty, the test program briefly waits using `usleep()` before retrying to avoid busy waiting.
- The header file includes a declaration for `close_mailbox()`. This function was not implemented because cleanup is handled in the test program using `munmap()` and `shm_unlink()`.
- Termination behavior was clarified when Kayode Binitie contacted the professor. The program terminates after sending and receiving a fixed number of messages provided via the command line.

---

## Compilation

```
gcc mboxtest.c mailbox.c -o mboxtest
```

---

## Execution

```
./mboxtest <number_of_messages>
```

Example:

```
./mboxtest 20
```

---

## Collaboration and References
This project was developed collaboratively using **GitHub** for version control.

Kayode Binitie contacted the professor via email to clarify termination behavior and bounded buffer semantics.

The **man7 Linux manual pages** were used as references for POSIX system calls such as:
- `shm_open()`
- `mmap()`
- `fork()`
- `ftruncate()`
- `shm_unlink()`