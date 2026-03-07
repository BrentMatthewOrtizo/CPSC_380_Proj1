#include "mailbox.h"
#include <sys/wait.h>

#define SHM_NAME "/mailbox"

int main(int argc, char *argv[]) {

    int fd;
    int count;
    mailbox_t *mbox;
    pid_t pid;

    // program expects number of messages to send/receive
    if (argc != 2) {
        printf("Usage: %s <num_messages>\n", argv[0]);
        return -1;
    }

    count = atoi(argv[1]);

    if (count <= 0) {
        printf("Number of messages must be positive\n");
        return -1;
    }

    // create/open POSIX shared memory object
    fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("shm_open");
        return -1;
    }

    // set size of shared memory region
    if (ftruncate(fd, sizeof(mailbox_t)) == -1) {
        perror("ftruncate");
        return -1;
    }

    // map shared memory into process address space
    mbox = mmap(NULL, sizeof(mailbox_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mbox == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    // file descriptor no longer needed after mmap
    close(fd);

    // initialize mailbox before fork
    mailbox_init(mbox);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }

    // CHILD = receiver
    if (pid == 0) {

        char buffer[MAILBOX_MAX_MSG + 1];
        size_t len;
        int received = 0;
        int ret;

        while (received < count) {

            ret = receive_mailbox(mbox, buffer, MAILBOX_MAX_MSG, &len);

            if (ret == MBOX_SUCCESS) {
                printf("Received: %s\n", buffer);
                received++;
            }

            else if (ret == MBOX_ERR_EMPTY) {
                printf("Mailbox empty\n");
                usleep(100000);
            }
        }

        munmap(mbox, sizeof(mailbox_t));
    }

    // PARENT = sender
    else {

        char msg[MAILBOX_MAX_MSG];
        int sent = 0;
        int ret;

        while (sent < count) {

            sprintf(msg, "Message %d", sent);

            ret = send_mailbox(mbox, msg, strlen(msg) + 1);

            if (ret == MBOX_SUCCESS) {
                printf("Sent: %s\n", msg);
                sent++;
            }

            else if (ret == MBOX_ERR_FULL) {
                printf("Mailbox full\n");
                usleep(100000);
            }
        }

        wait(NULL);

        munmap(mbox, sizeof(mailbox_t));
        shm_unlink(SHM_NAME);
    }
    return 0;
}