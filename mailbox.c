#include "mailbox.h"
#include <string.h>

void mailbox_init(mailbox_t *mbox) {
    if (mbox == NULL){
        return;
    }

    mbox -> head = 0;
    mbox -> tail = 0;
}

int send_mailbox(mailbox_t *mbox, const void *msg, size_t len) {
    if (mbox == NULL || msg == NULL) {
        return MBOX_ERR_INVALID; // -1
    }

    if (len > MAILBOX_MAX_MSG) {
        return MBOX_ERR_TOO_LARGE; // -4
    }

    int next_tail = (mbox -> tail + 1) % MAILBOX_CAPACITY;

    if (next_tail == mbox->head) {
        return MBOX_ERR_FULL; // -2
    }

    mailbox_slot_t *slot = &mbox -> slots[mbox->tail];

    slot->len = len;
    memcpy(slot->data, msg, len);

    mbox -> tail = next_tail;

    return MBOX_SUCCESS; // 0
}

int receive_mailbox(mailbox_t *mbox, void *out_buf, size_t maxlen, size_t *out_len) {
    if (mbox == NULL || out_buf == NULL || out_len == NULL) {
        return MBOX_ERR_INVALID; // -1
    }

    if (mbox->head == mbox->tail) {
        return MBOX_ERR_EMPTY; // -3
    }

    mailbox_slot_t *slot = &mbox->slots[mbox->head];

    size_t copy_len = slot->len;

    if (copy_len > maxlen) {
        copy_len = maxlen;
    }

    memcpy(out_buf, slot->data, copy_len);

    *out_len = slot->len;

    mbox->head = (mbox->head + 1) % MAILBOX_CAPACITY;

    return MBOX_SUCCESS; // 0
}


