#include "mailbox.h"

int main(){

	pid_t pid;

    mailbox_t mbox;

    // Create the mailbox:
    mailbox_init(&mbox);

    // Create asynchronous processes to send 
    // and recieve messages
	pid = fork();
	
    // Fork failed:
    if(pid == -1){ 
        fprintf(stderr, "%s%d\n%s%s\n", "Error number: ", 
                errno, "Error Message: ", strerror(errno)); 
    }

    // Child process: 
    else if (pid == 0) { 
        while(1)
            if( send_mailbox(&mbox, "LaLaLa", strlen("LaLaLa")) != MBOX_SUCCESS) {
                printf("Error writing to mailbox");
            return 1;
        };
		return 0;
	}

    // Parent process:
	else if (pid > 0) {

        while(1)
            if(receive_mailbox(&mbox, NULL, 0, NULL) != MBOX_SUCCESS) {
                printf("Error writing to mailbox");
            return 1; 
        };
		return 0;
	}

}
