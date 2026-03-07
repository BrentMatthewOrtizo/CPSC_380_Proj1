# Programming Assignment 1 – Asynchronous Mailbox IPC Using Shared Memory

## Authors
Brent Matthew Ortizo  
Kayode Binitie

## Design Approach

This project implements indirect inter-process communication using a mailbox stored in POSIX shared memory. The mailbox acts as a bounded circular buffer that allows one sender process and one receiver process to exchange messages asynchronously.

Shared memory is created using `shm_open()` and sized using `ftruncate()`. The mailbox structure is then mapped into the process address space using `mmap()`. After the mailbox is initialized with `mailbox_init()`, the program calls `fork()` to create two processes. The parent process acts as the sender and the child process acts as the receiver.

The sender repeatedly calls `send_mailbox()` to place messages into the mailbox, while the receiver repeatedly calls `receive_mailbox()` to read messages. Both functions are implemented as non-blocking operations that immediately return a status code indicating success or failure.

The mailbox itself is implemented as a circular buffer using `head` and `tail` indices. The `tail` index represents the next slot where a message will be written, and the `head` index represents the next slot to be read.

## Detecting Full and Empty Conditions

Full and empty conditions are detected using the positions of the `head` and `tail` indices.

The mailbox is empty when: head == tail

This means there are no messages available for the receiver to read.

The mailbox is full when: (tail + 1) % MAILBOX_CAPACITY == head

This condition above ensures that one slot in the circular buffer remains unused so the program can distinguish between full and empty states. Because of this design, a mailbox with a capacity of 8 slots can store up to 7 messages at a time.

## Assumptions

This implementation assumes exactly one sender process and one receiver process, as specified in the assignment instructions. Because only one process writes to the mailbox and only one process reads from it, additional synchronization mechanisms such as mutexes or semaphores are not required.

If the mailbox becomes full or empty, the test program waits briefly using `usleep()` before retrying the operation. This prevents busy waiting while maintaining the required non-blocking behavior.

The provided header file contains a declaration for `close_mailbox()`. This function was not implemented because mailbox resource cleanup is handled in the test program using `munmap()` and `shm_unlink()` to properly release the shared memory object.

A clarification about program termination and mailbox reuse was obtained when Kayode Binitie contacted the professor. The professor confirmed that the sender and receiver should terminate after a fixed number of messages (provided as a command-line argument) and that mailbox slots are reused through the circular buffer implementation.

## Collaboration and References

The project was developed collaboratively using GitHub to manage version control and coordinate changes between both authors.

Kayode Binitie also contacted the professor via email to clarify how the mailbox should terminate and how the bounded buffer should behave when it becomes full or empty.

The man7 Linux manual pages were used as references for POSIX system calls such as `shm_open()`, `mmap()`, `fork()`, and other related functions.