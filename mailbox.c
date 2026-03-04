#include "mailbox.h"

/* ---------------- API Functions ---------------- */
/*
        * Initialize a mailbox structure.
        * Must be called exactly once before fork().
*/
void mailbox_init(mailbox_t *mbox){
        mbox->head = 0;
        mbox->tail = 0;
        for (int i = 0; i < MAILBOX_CAPACITY; i++) {
                mbox->slots[i].len = 0;
        }

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
                    const void *msg, size_t len){
        if (len > MAILBOX_MAX_MSG) {
                return MBOX_ERR_TOO_LARGE;      
        }
        else if (len == 0 || msg == NULL || mbox == NULL) {
                return MBOX_ERR_INVALID;
        }
        int next_tail = (mbox->tail + 1) % MAILBOX_CAPACITY;
        if (next_tail == mbox->head) {
                return MBOX_ERR_FULL;
        }
        // Copy message into the mailbox slot
        mailbox_slot_t *slot = &mbox->slots[mbox->tail];
        memcpy(mbox->slots[mbox->tail].data, msg, len);
        mbox->slots[mbox->tail].len = len;
        mbox->tail += len;
        return MBOX_SUCCESS;    

}
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
int receive_mailbox(mailbox_t *mbox, void *out_buf, size_t maxlen, size_t *out_len){
        if (mbox == NULL || out_buf == NULL || out_len == NULL) {
                return MBOX_ERR_INVALID;
        }
        if (mbox->head == mbox->tail) {
                return MBOX_ERR_EMPTY;
        }
        mailbox_slot_t *slot = &mbox->slots[mbox->head];
        size_t copy_len = slot->len < maxlen ? slot->len : maxlen;
        memcpy(out_buf, slot->data, copy_len);
        *out_len = copy_len;
        mbox->head = (mbox->head + 1) % MAILBOX_CAPACITY;
        return MBOX_SUCCESS;
}
