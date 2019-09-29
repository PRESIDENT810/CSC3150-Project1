#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[]) {

    /* fork a child process */
    int status;
    pid_t pid;
    printf("Process start to fork\n");
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    else {
        if (pid == 0) { // child process
            int i;
            char *arg[argc];

            for (i = 0; i < argc - 1; i++) {
                arg[i] = argv[i + 1];
            }
            arg[argc - 1] = NULL;

            /* execute test program */
            printf("I'm the child process, my pid = %d\n", getpid());
            printf("Child process start to execute test program:\n");
            // start execute the program
            execve(arg[0], arg, NULL);

            printf("Continue to run original child process!\n");

            perror("execve");
            exit(SIGCHLD);
        }


        else { // parent process
            printf("I'm the parent process, my pid = %d\n", getpid());

            /* wait for child process terminates */
            waitpid(-1, &status, WUNTRACED);

            printf("Parent process receives the SIGCHLD signal\n");
//            printf("fuck %d\n", WTERMSIG(status));

            /* check child process'  termination status */
            if(WIFEXITED(status)){ // normal exit
                printf("Normal termination with EXIT STATUS = %d\n",WEXITSTATUS(status));
            }

            else if(WIFSIGNALED(status)){ // abnormal exit
                int num = WTERMSIG(status);
                switch (num){
                    case 6: // SIGABRT
                        printf("child process get SIGABRT signal\n");
                        printf("child process is abort by abort signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 14: // SIGALRM
                        printf("child process get SIGALRM signal\n");
                        printf("child process is abort by alarm signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 7: // SIGBUS
                        printf("child process get SIGBUS signal\n");
                        printf("child process is abort by bus error signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 8: // SIGFPE
                        printf("child process get SIGFPE signal\n");
                        printf("child process is abort by floating error signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 1: // SIGHUP
                        printf("child process get SIGHUP signal\n");
                        printf("child process is abort by hung up signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 4: // SIGILL
                        printf("child process get SIGILL signal\n");
                        printf("child process is abort by illegal signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 2: // SIGINT
                        printf("child process get SIGINT signal\n");
                        printf("child process is abort by keyboard interrupt signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 9: // SIGKILL
                        printf("child process get SIGKILL signal\n");
                        printf("child process is abort by kill signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 13: // SIGPIPE
                        printf("child process get SIGPIPE signal\n");
                        printf("child process is abort by broken pipe signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 3: // SIGQUIT
                        printf("child process get SIGQUIT signal\n");
                        printf("child process is abort by quit signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 11: // SIGSEGV
                        printf("child process get SIGSEGV signal\n");
                        printf("child process is abort by quit signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 15: // SIGTERM
                        printf("child process get SIGTERM signal\n");
                        printf("child process is abort by terminate signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                    case 5: // SIGTRAP
                        printf("child process get SIGTRAP signal\n");
                        printf("child process is abort by trap signal\n");
                        printf("CHILD EXECUTION FAILED!!\n");
                        break;
                }

            }
            else if(WIFSTOPPED(status)){ // stop signal
                printf("child process get SIGSTOP signal\n");
                printf("child process stopped\n");
                printf("CHILD EXECUTION STOPPED\n");            }
            else{
                printf("CHILD PROCESS CONTINUED\n");
            }
            exit(0);
        }

    }





}
