#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* In this so called social network project, I've used two structs, one named Node that represents the people registed on 
poxim's social network and another one that represesents the social network itself (which have Node type pointers, 
one for the head and other for the tail of the linked list). In order execute the actions ADD, REMOVE and SHOW, I've declared functions
that take a LinkedList type pointer and a person's name as argument, treated them and returned formated strings, 
that later on were written to a file.*/


typedef struct _node {
    char name[51];
    struct _node *previous;
    struct _node *next;
} Node;

typedef struct _circularDoublyLinkedList {
    Node *head;
    Node *tail;
} LinkedList;


void initializeLinkedList(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
}

// return the node related to the person's name or NULL if this one is not registed
Node *traverseList(LinkedList *list, char *name) {
    if(strcmp(list->tail->name, name) == 0) {
        return list->tail;
    } 
    else {
        Node *current = list->head;
        while(current != list->tail) {
            if(strcmp(current->name, name) == 0)
                return current;
            current = current->next;
        }
        return NULL;
    }
}


char* add(LinkedList *list, char *name) {
    Node *node = (Node*) malloc(sizeof(Node));
    // This conditional treats the empty list case
    strcpy(node->name, name);
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        node->next = list->head;
        node->previous = list->tail;
    }
    else {

        if(traverseList(list, name) != NULL) {
            char *not_add = (char*) malloc(66);
            sprintf(not_add,"[ERROR] ADD %s", name);
            return(not_add);
        }

        list->tail->next = node;
        node->previous = list->tail;
        node->next = list->head;
        list->head->previous = node;
        list->tail = node;
    }

    // malloc sufficient space to set a string message output
    char *ok_add = (char*)malloc(66);
    sprintf(ok_add,"[ OK  ] ADD %s", name);
    return(ok_add);
}


char* show(LinkedList *list, char *name) {
    // This conditional treats the empty list case
    if (list->head == NULL) {
        char *error_show = (char*)malloc(72);
        sprintf(error_show,"[ERROR] ?<-%s->?", name);
        return(error_show);
    }

    // Check if the name is equal the list head's name
    else if (strcmp(list->tail->name, name) == 0) {
        char *ok_show = (char*)malloc(60+sizeof(list->tail->previous->name)+sizeof(list->tail->name)+sizeof(list->tail->next->name));
        sprintf(ok_show,"[ OK  ] %s<-%s->%s", list->tail->previous->name, list->tail->name, list->tail->next->name);
        return(ok_show);
    }

    else {
        Node *current = list->head;
        while(current != list->tail) {
            if(strcmp(current->name, name) == 0) {
                char *ok_show = (char*)malloc(60+sizeof(current->previous->name)+sizeof(current->name)+sizeof(current->next->name));
                sprintf(ok_show,"[ OK  ] %s<-%s->%s", current->previous->name, current->name, current->next->name);
                return(ok_show); 
            }
            current = current->next;
        }
    }

    // Return error if the name is not registed on the linked list
    char *error_show = (char*)malloc(72);
    sprintf(error_show,"[ERROR] ?<-%s->?", name);
    return(error_show);
}

char* remove_node(LinkedList *list, char *name) {
    // This conditional treats the empty list case
    if(list->head == NULL) {
        char *error_remove = (char*)malloc(72);
        sprintf(error_remove,"[ERROR] REMOVE %s", name);
        return(error_remove);
    }

    else {
        // check if the person is registed
        Node *node = traverseList(list, name);
        if(node != NULL) {
            
            if(node == list->head) {
                node->next->previous = list->tail;
                list->tail->next = node->next;
                list->head = node->next;
            }
            else if(node == list->tail) {
                node->previous->next = list->head;
                list->head->previous = node->previous;
                list->tail = node->previous;

            }
            else {
                node->previous->next = node->next;
                node->next->previous = node->previous;
            }
            node->next = NULL;
            node->previous = NULL;
            free(node);
            char *ok_remove = (char*)malloc(72);
            sprintf(ok_remove,"[ OK  ] REMOVE %s", name);
            return(ok_remove);
        }
        else {
            char *error_remove = (char*)malloc(72);
            sprintf(error_remove,"[ERROR] REMOVE %s", name);
            return(error_remove);
        }
    }
}


int main(int argc, char* argv[]) {
    LinkedList list;
    LinkedList *listPointer = &list;
    initializeLinkedList(listPointer);

	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    char commandLine[70];
    char *command, *name;

    while(fgets(commandLine, sizeof(commandLine), input)) {
        command = strtok(commandLine, " ");
        name = strtok(NULL, "");
        size_t len = strlen(name);
        if(name[len-1] == '\n')
		name[len-1] = '\0';

        if (commandLine[0] == 'A') {
            printf("%s\n", command);
            fprintf(output, "%s\n", add(listPointer, name));
        }
        else if (commandLine[0] == 'S') {
            fprintf(output, "%s\n", show(listPointer, name));
        }
        else {
            fprintf(output, "%s\n", remove_node(listPointer, name));
        }
    }
            
    fclose(input); 
    fclose(output);
    return 0;
}
