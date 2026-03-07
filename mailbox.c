// mailbox.c - Implementation of the mailbox functions for inter-process communication
#include "mailbox.h"
#include <string.h>

/// Initializes the mailbox structure in shared memory.
void mailbox_init(mailbox_t *mbox)
{
    // Validate the mailbox pointer:
    if (mbox == NULL) {
        fprintf(stderr, "Invalid mailbox\n");
        exit(1);
    }

    // Initialize head and tail indices to 0:
    mbox->head = 0;
    mbox->tail = 0;
}

// Sends a message to the mailbox. Returns 0 on success or a negative error code.
int send_mailbox(mailbox_t *mbox, const void *msg, size_t len)
{
    // Validate arguments:
    if (mbox == NULL || msg == NULL)
        return MBOX_ERR_INVALID;

    if (len > MAILBOX_MAX_MSG)
        return MBOX_ERR_TOO_LARGE;

    int next_tail = (mbox->tail + 1) % MAILBOX_CAPACITY;

    if (next_tail == mbox->head)
        return MBOX_ERR_FULL;

    // Write the message into the current tail slot:
    mailbox_slot_t *slot = &mbox->slots[mbox->tail];

    // Copy the message data into the slot and set the length:
    memcpy(slot->data, msg, len);
    slot->len = len;

    // Update the tail index to the next slot:
    mbox->tail = next_tail;

    return MBOX_SUCCESS;
}

// Receives a message from the mailbox. Returns 0 on success or a negative error code.
int receive_mailbox(mailbox_t *mbox, void *out_buf, size_t maxlen, size_t *out_len)
{
    // Validate arguments:
    if (mbox == NULL || out_buf == NULL || out_len == NULL)
        return MBOX_ERR_INVALID;

    if (mbox->head == mbox->tail)
        return MBOX_ERR_EMPTY;

    // Read the message from the current head slot:
    mailbox_slot_t *slot = &mbox->slots[mbox->head];

    // Copy the message data into the output buffer (up to maxlen) and set the output length:
    size_t copy_len = (slot->len < maxlen) ? slot->len : maxlen;

    memcpy(out_buf, slot->data, copy_len);

    *out_len = slot->len;

    // Update the head index to the next slot:
    mbox->head = (mbox->head + 1) % MAILBOX_CAPACITY;

    return MBOX_SUCCESS;
}

// Unmaps the mailbox from shared memory. Returns 0 on success or a negative error code.
// Only the parent process should call this function after the child has finished using the mailbox.
int close_mailbox(mailbox_t *mbox)
{
    // Validate the mailbox pointer:
    if (mbox == NULL)
        return MBOX_ERR_INVALID;

    // Unmap the mailbox from shared memory:
    if (munmap(mbox, sizeof(mailbox_t)) == -1) {
        perror("munmap");
        return MBOX_ERR_INVALID;
    }

    return MBOX_SUCCESS;
}