#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include <string.h>


struct Node{
    int index;
    char filename[200];
    pid_t my_pid;
    pid_t child_pid;
    struct Node *nxt_node;
};

void print_node(struct Node *first_node);

int main(int argc,char *argv[]){
    struct Node *first_node = calloc(10, sizeof(char));
    struct Node *last_node =  first_node;
    last_node->index = 0;
    strcpy(last_node->filename, "Null_node");

	/* Implement the functions here */
	for (int i = 0; i<argc-1; i++) {

	    struct Node *current_node = calloc(1,100);
	    current_node->index = i+1;
	    strcpy(current_node->filename, argv[i+1]);
	    last_node->nxt_node = current_node;
	    last_node = current_node;
	}

	last_node->nxt_node = NULL;

	print_node(first_node);

	return 0;
}

void execute_file(char *filename) {
    char *current_path = calloc(200, sizeof(char));
    strcpy(current_path, "/Users/zhongkaining/OneDrive/College/2019-Term 1/CSC3150/Project/CSC3150_Assignment_1/source/bonus/");
    strcat(current_path, filename);
    execve(current_path, NULL, NULL);
    perror("fuck this shit");
}


void print_node(struct Node *first_node){
    struct Node *current_node = first_node;
    while (current_node->nxt_node != NULL){
        printf("%s\n",current_node->filename);
        current_node = current_node->nxt_node;
    }
}