#include "mailbox.h"

/* ---------------- API Functions ---------------- */
/*
        * Initialize a mailbox structure.
        * Must be called exactly once before fork().
*/
void mailbox_init(mailbox_t *mbox){
}
/*
        * Asynchronous send.
        *
        * Returns:
        * MBOX_SUCCESS on success
        * MBOX_ERR_FULL if mailbox is full
        * MBOX_ERR_TOO_LARGE if len > MAILBOX_MAX_MSG
        * MBOX_ERR_INVALID on invalid arguments
*/
int send_mailbox(mailbox_t *mbox,
                    const void *msg, size_t len);
/*
        * Asynchronous receive.
        *
        * Copies at most maxlen bytes into out_buf.
        *
        * On success:
        * - returns MBOX_SUCCESS
        * - writes actual message length to *out_len
        *
        * Returns:
        * MBOX_ERR_EMPTY if mailbox is empty
        * MBOX_ERR_INVALID on invalid arguments
*/
int receive_mailbox(mailbox_t *mbox, void *out_buf, size_t maxlen, size_t *out_len);
