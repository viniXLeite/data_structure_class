#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Use string management of struc_t in order to fix this algorithm

/* 
   -Then use this algorithm as referende to create a circular doubly linked list.
   -Firts Doubly and then circular
   -Pay attetion on adding a previous node pointer on Node Struct and connect it to other Node's objects.
   -Implement a file's write and read system.
   -Only need to use one type of adding function, choose the best kind among these.
   -Search for solution of the deallocating structs problem.
*/

typedef struct _node {
    char *data;
    struct _node *next;
} Node;

typedef struct _linkedList {
    Node *head;
    Node *tail;
    Node *current;
} LinkedList;

void initializeList(LinkedList *list) {
    list->head = NULL;
    list->current = NULL;
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
    // Modify this line to address the 'next' pointer to the list's head instead of null
    // Declare a function get previous to run a loop until node != list->tail and then asign this node to node->previous O(n) 
    // or see if it's possible to just use node->previous = list->tail and then list->tail = node 
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
    }
    else {
        list->tail->next = node;
    }
    list->tail = node;
}

Node *getNode(LinkedList *list, char *data) {
    Node *node = list->head;
    // pay attention on the difference between while and do while
    // while (node != Tail) and show the list->tail
    // use a function like this to get the previous and next of a certain node data
    while (node != NULL) {
        //Be careful when comparing strigs, for this pourpose use only the strcomp function
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

// X
void displayLinkedList(LinkedList *list) {
    printf("\nLinked List\n");
    Node *current = list->head;
    while (current != NULL){
        printf("data: %s\n", (current->data));
        current = current->next;
    }
}

void deallocateList(LinkedList *list) {
    free(list->current);
    free(list->head);
    free(list->tail);
}

int main() {
    LinkedList List;
    LinkedList *listPointer = &List;
    char value1[] = "Jose";
    char value2[] = "Maria";
    char value3[] = "Matheus";

    initializeList(listPointer);
    addHead(listPointer, value1);
    addHead(listPointer, value2);
    addHead(listPointer, value3);

    Node *node2 = getNode(listPointer, value2);
    delete(listPointer, node2);
    displayLinkedList(listPointer);

    //check is it's possible to deallocate the listpointer and the node2 pointer
    deallocateList(listPointer);
    return 0;
}