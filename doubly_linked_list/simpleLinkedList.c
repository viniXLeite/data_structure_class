#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* 
   -Then use this algorithm as referende to create a circular doubly linked list.
   -Firts Doubly and then circular
   -Pay attetion on adding a previous node pointer on Node Struct and connect it to other Node's objects.
   -Implement a file's write and read system.
   -Only need to use one type of adding function, choose the best kind among these.
   -Change all the function and variables names and the logic of the conditionals
*/

typedef struct _node {
    char *data;
    struct _node *next;
} Node;

typedef struct _linkedList {
    Node *head;
    Node *tail;
} LinkedList;

void initializeList(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
}

// X
void addHead(LinkedList *list, char *data) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = (char*) malloc(sizeof(data)+1);
    strcpy(node->data, data);
    if (list->head == NULL) {
        list->tail = node; 
        node->next = NULL;
    }
    else {
        node->next = list->head;
    }
    list->head = node;
}

void addTail(LinkedList *list, char *data) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = (char*) malloc(sizeof(data)+1);
    strcpy(node->data, data);
    // Modify this line to address the node 'next' pointer to the list's head instead of Null
    // In order to assign the previous pointer of the node to the previous node just do node->previous = list->tail and after that list->tail=node
    // Then write node->next = list->head and list->head->previous = list->tail to turn it into a circular doubly linked list
    // list->tail->next=node, node->previous = list->tail, node->next= list->head, list->head->previous=node, list->tail=node

    if (list->head == NULL) {
        list->head = node;
    }
    else {
        list->tail->next = node;
    }
    node->next = NULL;
    list->tail = node;
}

// X
Node *getNode(LinkedList *list, char *data) {
    Node *node = list->head;
    while (node != NULL) {
        //Be careful when comparing strigs, for this pourpose only use the strcomp function
        if (strcmp(node->data, data) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// Analyse this function
void delete(LinkedList *list, Node *node) {
    if (node == list->head) {
        if (list->head->next == NULL) {
            list->head = list->tail = NULL;
        }
        else {
            list->head = list->head->next;
        }
    }
    else {
        Node *tmp = list->head;
        while (tmp != NULL && tmp->next != node) {
            tmp = tmp->next;
        }
        if (tmp != NULL) {
            tmp->next = node->next;
        }
    }
    free(node);
}

// Modify this function to accept a new argument that is the person's name and exibit one's previous and curent only 
// use get node as reference and a return within the loop
// To exibit the friends of the current node use current->next->data and current->previous->data
void displayLinkedList(LinkedList *list) {
    printf("\nLinked List\n");
    Node *current = list->head;
    while (current != NULL){
        printf("data: %s\n", (current->data));
        current = current->next;
    }
}

// No need to deallocate the pointer to the struct at the end of the application, because the operating system already does this
int main() {
    LinkedList List;
    LinkedList *listPointer = &List;

    initializeList(listPointer);
    addHead(listPointer, "Jose");
    addHead(listPointer, "Maria");
    addHead(listPointer, "Matheus");

    Node *node2 = getNode(listPointer, "Maria");
    delete(listPointer, node2);
    displayLinkedList(listPointer);

    return 0;
}