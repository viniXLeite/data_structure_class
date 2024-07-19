#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _node {
    int *data;
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

void addHead(LinkedList *list, int *data) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = data;
    if (list->head == NULL) {
        list->tail = node; 
        node->next = NULL;
    }
    else {
        node->next = list->head;
    }
    list->head = node;
}

void addTail(LinkedList *list, int *data) {
    Node *node = (Node*) malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node;
    }
    else {
        list->tail->next = node;
    }
    list->tail = node;
}

Node *getNode(LinkedList *list, int *data) {
    Node *node = list->head;
    while (node != NULL) {
        //Be careful when comparing strigs, for this pourpose use only the strcomp function
        if (node->data == data) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// Analise this function
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

void displayLinkedList(LinkedList *list) {
    printf("\nLinked List\n");
    Node *current = list->head;
    while (current != NULL){
        printf("data: %d\n", *(current->data));
        current = current->next;
    }
}

void disp() {
    printf("OK\n");
}

int main() {
    LinkedList List;
    LinkedList *listPointer = &List;
    int value1 = 23;
    int value2 = 34;
    int value3 = 45;
    int *pv1 = &value1;
    int *pv2 = &value2;
    int *pv3 = &value3;

    initializeList(listPointer);
    addHead(listPointer, pv1);
    addHead(listPointer, pv2);
    addHead(listPointer, pv3);

    Node *node2 = getNode(listPointer, pv2);
    delete(listPointer, node2);

    disp();
    displayLinkedList(listPointer);
}