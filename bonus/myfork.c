#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <string.h>

struct Node {
    int index;
    char filename[200];
    pid_t my_pid;
    pid_t child_pid;
    struct Node *nxt_node;
};

struct StatusNode {
    int code;
    struct StatusNode *nxt_StatusNode;
};

void print_node(struct Node *first_node);

void execute_file(struct Node *node);

void fork_node(struct Node *parent_node);

void fork_same(struct Node *last_node);

void status_info(int status);

void process_tree(struct Node *first_node);

void add_status(int status);

void add_laststatus(int status);

void print_status();

struct Node *first_node;
struct StatusNode *first_statusNode;
struct StatusNode *current_statusNode;

int main(int argc, char *argv[]) {
    first_statusNode = calloc(200, sizeof(int));
    current_statusNode = calloc(200, sizeof(int));
    first_statusNode->code = -1;
    current_statusNode = first_statusNode;

//    struct Node *first_node = calloc(100, sizeof(char));
    first_node = calloc(600, sizeof(char));
    first_node->index = 0;
    strcpy(first_node->filename, "Null_node");
    struct Node *last_node = first_node;


    /* Implement the functions here */
    for (int i = 0; i < argc - 1; i++) {

        struct Node *current_node = calloc(600, sizeof(char));
        current_node->index = i + 1;
        strcpy(current_node->filename, argv[argc -1 - i]);
//        printf("file is called %s\n", argv[argc -1 - i]);
        last_node->nxt_node = current_node;
        last_node = current_node;
    }

    last_node->nxt_node = NULL;
    first_node->my_pid = getpid();
    fork_node(first_node->nxt_node);

    return 0;
}

void fork_node(struct Node *parent_node) {
    struct Node *child_node = calloc(200, sizeof(char));
    child_node = parent_node->nxt_node;
    if (child_node == NULL){
        parent_node->my_pid = getpid();
        fork_same(parent_node);
        exit(0);
    }

    int status;
    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else {
        if (pid == 0) { // child process
            /* execute self */
            if (strcmp(parent_node->filename, "Null_node") != 0){
                execute_file(parent_node);
            }
//            exit(SIGCHLD);
        }
        else { // parent process
            parent_node->my_pid = pid;
            /* wait for child process terminates */
            waitpid(-1, &status, WUNTRACED);
            printf("my shit is %d\n", status);
            add_status(status);
            fork_node(child_node);

            /* check child process'  termination status */
//            if (WIFEXITED(status)) { // normal exit
//                printf("Normal termination with EXIT STATUS = %d\n", WEXITSTATUS(status));
//            }
//            else if (WIFSIGNALED(status)) { // abnormal exit
//                int num = WTERMSIG(status);
//                status_info(num);
//            }
//            else {
//                printf("CHILD PROCESS CONTINUED\n");
//            }

        }

    }
}

void fork_same(struct Node *last_node) {
    int status;
    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(1);
    } else {
        if (pid == 0) { // child process
                execute_file(last_node);
        }
//            exit(SIGCHLD);
        else { // parent process
            last_node->my_pid = pid;
            /* wait for child process terminates */
            waitpid(-1, &status, WUNTRACED);
            printf("my shit is %d\n", status);
            add_laststatus(status);
            print_status();
            process_tree(first_node);

            /* check child process'  termination status */
            if (WIFEXITED(status)) { // normal exit
                printf("Normal termination with EXIT STATUS = %d\n", WEXITSTATUS(status));
            }
            else if (WIFSIGNALED(status)) { // abnormal exit
                int num = WTERMSIG(status);
                status_info(num);
            }
            else {
                printf("CHILD PROCESS CONTINUED\n");
            }

        }

    }
}

void process_tree(struct Node *first_node){
    printf("Process Tree:\nthe process tree: ");
    struct Node *current_node = first_node;
    while (current_node->nxt_node != NULL){
        printf("%d->", current_node->my_pid);
        current_node = current_node->nxt_node;
    }
    printf("%d\n\n", current_node->my_pid);
}

void execute_file(struct Node *node) {
    char *filename = calloc(500, sizeof(char));
    strcpy(filename, node->filename);
    char *current_path = calloc(500, sizeof(char));
    strcpy(current_path,"/Users/zhongkaining/OneDrive/College/2019-Term 1/CSC3150/Project/CSC3150_Assignment_1/source/bonus/");
//    strcpy(current_path,"/mnt/hgfs/CSC3150/Project/CSC3150_Assignment_1/source/bonus/");
    strcat(current_path, filename);
//    printf("this shit is called %s\n", current_path);
    char *const *args = calloc(1, sizeof(char));
    execve(current_path, args, NULL);
}

void print_node(struct Node *first_node) {
    struct Node *current_node = first_node;
    while (current_node->nxt_node != NULL) {
        printf("%s\n", current_node->filename);
        current_node = current_node->nxt_node;
    }
}

void status_info(int status){
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
}

void add_status(int status){
    struct StatusNode *nxt_statusNode = calloc(200, sizeof(int));
    nxt_statusNode->code = status;
    current_statusNode->nxt_StatusNode = nxt_statusNode;
    current_statusNode = current_statusNode->nxt_StatusNode;
}

void add_laststatus(int status){
    struct StatusNode *nxt_statusNode = calloc(200, sizeof(int));
    nxt_statusNode->code = status;
    nxt_statusNode->nxt_StatusNode = NULL;
    current_statusNode->nxt_StatusNode = nxt_statusNode;
    current_statusNode = current_statusNode->nxt_StatusNode;
}

void print_status(){
    struct StatusNode *this_statusNode = calloc(200, sizeof(int));
    this_statusNode = first_statusNode;
    while (this_statusNode->nxt_StatusNode != NULL){
        printf("My dick is like %d\n", this_statusNode->code);
        this_statusNode = this_statusNode->nxt_StatusNode;
    }
    printf("My dick is like %d\n", this_statusNode->code);
}