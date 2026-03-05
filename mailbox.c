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

