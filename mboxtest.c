#include "mailbox.h"

int main(){

	pid_t pid;

    mailbox_t mbox;
    // Create the mailbox:
    mailbox_init(mailbox_t *mbox);

    // Create asynchronous processes to send 
    // and recieve messages
	pid = fork();
	
    if(pid == -1){
        fprintf(stderr, "%s%d\n%s%s\n", "Error number: ", errno, "Error Message: ", strerror(errno)); 
    }
    else if (pid == 0) { /* child process */
        while(true)
            if( send_mailbox(mailbox_t *mbox, const void *msg, size_t len) != MBOX_SUCCESS) {
                printf("Error writing to mailbox");
            return 1 
        };
		return 0;
	}
	else if (pid > 0) { /* parent process */

        while(true)
            if(receive_mailbox(mailbox_t *mbox, void *out_buf, size_t maxlen, size_t *out_len) != MBOX_SUCCESS) {
                printf("Error writing to mailbox");
            return 1 
        };
		return 0;
	}

}
