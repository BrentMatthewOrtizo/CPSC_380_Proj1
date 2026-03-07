# SP26 CPSC 380 Programming Assignment 1: Mailbox

## Contributors

- Name: Kayode Binitie
- Student ID: 2461327
- Email: [binitie@chapman.edu](mailto:binitie@chapman.edu)

## Description

This repository contains the implementation for an asynchronous mailbox.

## Design Approach

- Passed mapped shared memory to a pointer and which is then passed as parameters to prevent processes from having separate copies.

- Ensured that all parameters receive sanity checks and that only one process is responsible for unlinking the shared memory

- Had the parent wait on the child process to prevent zombificaion.

## How You Detect Full and Empty Conditions

- The mailbox is full if the next write position is the same as the current read position(referenced as next_tail and head respectively in the code)

- The mailbox is empty if the current read position is the same as the current write position(referenced as head and tail respectively inthe code)

## Assumptions

- Only one sender and receiver process.

- Message length is always known

## Instructions

### Compilation

```bash
        gcc mboxtest.c mailbox.c -o mboxtest
```

### Execution

```bash
        ./mboxtest <number of messages to send or 10 by default>

```
