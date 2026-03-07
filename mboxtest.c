// mboxtest.c - Test program for the mailbox implementation
#include "mailbox.h"
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    // Validate command-line arguments:
    if (argc > 2) {
        printf("Usage: %s [num_messages]\n", argv[0]);
        return 1;
    }

    // Default to 10 messages if not specified:
    int num_messages = (argc == 2) ? atoi(argv[1]) : 10;

    // Create shared mailbox:
    mailbox_t *mbox = mmap(NULL,
                           sizeof(mailbox_t),
                           PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS,
                           -1,
                           0);

    // Check for mmap failure:
    if (mbox == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Initialize mailbox:
    mailbox_init(mbox);

    // Start asynchronous sender and receiver processes:
    pid_t pid = fork();

    // Check for fork failure:
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    /* CHILD: sender */
    if (pid == 0) {

        // Send the user-specified number of messages:
        for (int i = 0; i < num_messages; i++) {

            char msg[MAILBOX_MAX_MSG];
            sprintf(msg, "Message %d", i + 1);

            // Keep trying to send until successful (handle full mailbox):
            while (send_mailbox(mbox, msg, strlen(msg)) == MBOX_ERR_FULL);

        }

        exit(0);
    }

    /* PARENT: receiver */
    else {

        // Receive the expected number of messages:
        for (int i = 0; i < num_messages; i++) {

            char buffer[MAILBOX_MAX_MSG];
            size_t len;

            // Keep trying to receive until successful (handle empty mailbox):
            while (receive_mailbox(mbox, buffer, sizeof(buffer), &len) == MBOX_ERR_EMPTY);

            // Null-terminate the received message for printing:
            buffer[len] = '\0';

            // Print the received message:
            printf("Received: %s\n", buffer);
        }

        // Wait for sender to finish and clean up:
        wait(NULL);

        // Unmap the shared mailbox:
        close_mailbox(mbox);
    }

    return 0;
}