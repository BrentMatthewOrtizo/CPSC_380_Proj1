#include "mailbox.h"
#include <string.h>

void mailbox_init(mailbox_t *mbox) {
    if (mbox == NULL)
        return;

    mbox -> head = 0;
    mbox -> tail = 0;
}

