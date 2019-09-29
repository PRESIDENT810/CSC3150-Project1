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

int convert2signal(int exit_code);

struct Node *first_node;
struct StatusNode *first_statusNode;
struct StatusNode *current_statusNode;
int arg_count;

int main(int argc, char *argv[]) {
    arg_count = argc;
    first_statusNode = calloc(600, sizeof(int));
    current_statusNode = calloc(600, sizeof(int));
    first_statusNode->code = -1;
    current_statusNode = first_statusNode;

    first_node = calloc(600, sizeof(char));
    first_node->index = 0;
    strcpy(first_node->filename, "Null_node");
    struct Node *last_node = first_node;


    /* Implement the functions here */
    for (int i = 0; i < argc - 1; i++) {

        struct Node *current_node = calloc(600, sizeof(char));
        current_node->index = i + 1;
        strcpy(current_node->filename, argv[argc -1 - i]);
        last_node->nxt_node = current_node;
        last_node = current_node;
    }

    last_node->nxt_node = NULL;
    first_node->my_pid = getpid();
    fork_node(first_node->nxt_node);

    return 0;
}

void fork_node(struct Node *parent_node) {
    struct Node *child_node = calloc(600, sizeof(char));
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
        }
        else { // parent process
            parent_node->my_pid = pid;
            /* wait for child process terminates */
            waitpid(-1, &status, WUNTRACED);
            add_status(status);
            fork_node(child_node);
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
        else { // parent process
            last_node->my_pid = pid;

            /* wait for child process terminates */
            waitpid(-1, &status, WUNTRACED);
            add_laststatus(status);
            process_tree(first_node);
            print_status();
        }

    }
}

void relation_info(int idx){
    int i = idx;
    int last_pid = -1;
    struct Node *result_node = first_node;
    struct StatusNode *result_statusNode = first_statusNode;
    while (i != 0){
        result_statusNode = result_statusNode->nxt_StatusNode;
        i--;
    }
    i = idx;
    while (arg_count-i != 0){
        last_pid = result_node->my_pid;
        result_node = result_node->nxt_node;
        i++;
    }
    int parent_pid = last_pid;
    int child_pid = result_node->my_pid;
    int signal = convert2signal(result_statusNode->code);
    if (signal != 0) {
        printf("The child process (pid=%d) of parent process (pid=%d) is terminated by signal %d\n", child_pid, parent_pid, signal);
        printf("The signal number = %d\n", signal);
    }
    else {
        printf("The child process (pid=%d) of parent process (pid=%d) has normal execution %d\n", child_pid, parent_pid,
               signal);
    }

}

void process_tree(struct Node *First_node){
    printf("Process Tree:\nthe process tree: ");
    struct Node *current_node = First_node;
    while (current_node->nxt_node != NULL){
        printf("%d->", current_node->my_pid);
        current_node = current_node->nxt_node;
    }
    printf("%d\n\n", current_node->my_pid);
}

void execute_file(struct Node *node) {
    char *filename = calloc(600, sizeof(char));
    strcpy(filename, node->filename);
    char *current_path = calloc(600, sizeof(char));
//    strcpy(current_path,"/Users/zhongkaining/OneDrive/College/2019-Term 1/CSC3150/Project/CSC3150_Assignment_1/source/bonus/");
    strcpy(current_path,"/mnt/hgfs/CSC3150/Project/CSC3150_Assignment_1/source/bonus/");
    strcat(current_path, filename);
//    printf("this shit is called %s\n", current_path);
    char *const *args = calloc(600, sizeof(char));
    execve(current_path, args, NULL);
}

void status_info(int status){
    if(WIFEXITED(status)){ // normal exit
        printf("Its exit status = 0\n");
    }

    else if(WIFSIGNALED(status)){ // abnormal exit
        int num = WTERMSIG(status);
        switch (num){
            case 6: // SIGABRT
                printf("child process get SIGABRT signal\n");
                printf("child process is abort by abort signal\n");
                break;
            case 14: // SIGALRM
                printf("child process get SIGALRM signal\n");
                printf("child process is abort by alarm signal\n");
                break;
            case 7: // SIGBUS
                printf("child process get SIGBUS signal\n");
                printf("child process is abort by bus error signal\n");
                break;
            case 8: // SIGFPE
                printf("child process get SIGFPE signal\n");
                printf("child process is abort by floating error signal\n");
                break;
            case 1: // SIGHUP
                printf("child process get SIGHUP signal\n");
                printf("child process is hung up\n");
                break;
            case 4: // SIGILL
                printf("child process get SIGILL signal\n");
                printf("child process is abort by illegal signal\n");
                break;
            case 2: // SIGINT
                printf("child process get SIGINT signal\n");
                printf("child process is abort by keyboard interrupt signal\n");
                break;
            case 9: // SIGKILL
                printf("child process get SIGKILL signal\n");
                printf("child process is abort by kill signal\n");
                break;
            case 13: // SIGPIPE
                printf("child process get SIGPIPE signal\n");
                printf("child process is abort by broken pipe signal\n");
                break;
            case 3: // SIGQUIT
                printf("child process get SIGQUIT signal\n");
                printf("child process is abort by quit signal\n");
                break;
            case 11: // SIGSEGV
                printf("child process get SIGSEGV signal\n");
                printf("child process is abort by quit signal\n");
                break;
            case 15: // SIGTERM
                printf("child process get SIGTERM signal\n");
                printf("child process is abort by terminate signal\n");
                break;
            case 5: // SIGTRAP
                printf("child process get SIGTRAP signal\n");
                printf("child process reach a breakpoing\n");
                break;
            case 0: //
                printf("Its exit status = 0\n");
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
    printf("\n");
}

void add_status(int status){
    struct StatusNode *nxt_statusNode = calloc(600, sizeof(char));
    nxt_statusNode->code = status;
    current_statusNode->nxt_StatusNode = nxt_statusNode;
    current_statusNode = current_statusNode->nxt_StatusNode;
}

void add_laststatus(int status){
    struct StatusNode *nxt_statusNode = calloc(600, sizeof(char));
    nxt_statusNode->code = status;
    nxt_statusNode->nxt_StatusNode = NULL;
    current_statusNode->nxt_StatusNode = nxt_statusNode;
    current_statusNode = current_statusNode->nxt_StatusNode;
}

void print_status(){
    int cnt = 1;
    struct StatusNode *this_statusNode = calloc(600, sizeof(char));
    this_statusNode = first_statusNode->nxt_StatusNode;
    while (this_statusNode->nxt_StatusNode != NULL){
//        printf("Fuck you bitch\n");
        relation_info(cnt);
        cnt++;
        status_info(convert2signal(this_statusNode->code));
        this_statusNode = this_statusNode->nxt_StatusNode;
    }

    relation_info(cnt);
    status_info(this_statusNode->code);
}

int convert2signal(int exit_code){
    switch (exit_code) {
        case 1:
            return 1;
        case 2:
            return 2;
        case 131:
            return 3;
        case 132:
            return 4;
        case 133:
            return 5;
        case 134:
            return 6;
        case 135:
            return 7;
        case 136:
            return 8;
        case 9:
            return 9;
        case 139:
            return 11;
        case 13:
            return 13;
        case 14:
            return 14;
        case 15:
            return 15;
        case 0:
            return 0;
        default:
            printf("I am so fuckin' sorry I didn't prepare for this exit code\n");
            return 0;
    }
}