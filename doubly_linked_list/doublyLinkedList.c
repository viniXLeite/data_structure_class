#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node {
    char *name;
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
    node->name = (char*) malloc(sizeof(name)+1);
    strcpy(node->name, name);
    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        node->next = list->head;
        node->previous = list->tail;
    }
    else {
        // Test if this sequence works on adding a new node to a sigle node circular doubly linked list
        // Test if you can drop out node->previous = list->tail; and node->next = list->head; of both conditionals since they declared twice

        if(traverseList(list, name) != NULL) {
            char *not_add = (char*) malloc(15+sizeof(name));
            sprintf(not_add,"[ERROR] ADD %s", name);
            return(not_add);
        }

        list->tail->next = node;
        node->previous = list->tail;
        node->next = list->head;
        list->head->previous = node;
        list->tail = node;
    }

    char *ok_add = (char*)malloc(15+sizeof(name));
    sprintf(ok_add,"[ OK  ] ADD %s", name);
    return(ok_add);
}

// make this function return a char* see test.c
char* show(LinkedList *list, char *name) {

    if (strcmp(list->tail->name, name) == 0) {
        char *ok_show = (char*)malloc(15+sizeof(list->tail->previous->name)+sizeof(list->tail->name)+sizeof(list->tail->next->name));
        sprintf(ok_show,"[ OK  ] %s<-%s->%s", list->tail->previous->name, list->tail->name, list->tail->next->name);
        return(ok_show);
    }

    else {
        Node *current = list->head;
        while(current != list->tail) {
            if(strcmp(current->name, name) == 0) {
                char *ok_show = (char*)malloc(15+sizeof(current->previous->name)+sizeof(current->name)+sizeof(current->next->name));
                sprintf(ok_show,"[ OK  ] %s<-%s->%s", current->previous->name, current->name, current->next->name);
                return(ok_show); 
            }
            current = current->next;
        }
    }

    char *error_show = (char*)malloc(16+sizeof(name));
    sprintf(error_show,"[ERROR] ?<-%s->?", name);
    return(error_show);
}

void remove_node(LinkedList *list, char *name) {
    // if list->head == NULL: return;
    //else
        // if name in list
            // if node == list->head: node->next->previous = list->tail, list->tail->next = node->next, list->head = node->next, node->next = NULL, node->previous = NULL, free(node)
        
            // elif node == list->tail: node->previous->next = list->head, list->head->previous = node->previous, list->tail = node->previous, node->next = NULL, node->previous = NULL, free(node)
        
            // else: node->previous->next = node->next, node->next->previous = node->previous, node->previous = NULL, node->next NULL, free(node)
            
            //[ok] remove name

        // else
            // error remove name
    if(list->head == NULL)
        return;
    else {
        Node *node = traverseList(list, name);
        if(node != NULL) {
            if(node == list->head) {
                node->next->previous = list->tail;
                list->tail->next = node->next;
                list->head = node->next;
                //node->next = NULL;
                //node->previous = NULL;
                free(node);
            }
            else if(node == list->tail) {
                node->previous->next = list->head;
                list->head->previous = node->previous;
                list->tail = node->previous;
                free(node);
            }
            else {
                node->previous->next = node->next;
                node->next->previous = node->previous;
                free(node);
            }
            printf("%s deleted from list\n", name);
        }
        else {
            printf("%s not in list\n", name);
        }
    }
}

int main() {
    LinkedList list;
    LinkedList *listPointer = &list;

    initializeLinkedList(listPointer);
    printf("%s\n", add(listPointer, "Maria"));
    printf("%s\n", show(listPointer, "Maria"));
    printf("%s\n", add(listPointer, "Maria"));

    printf("%s\n", add(listPointer, "Gabriel"));
    printf("%s\n", show(listPointer, "Maria"));
    printf("%s\n", show(listPointer, "Gabriel"));
    printf("%s\n", add(listPointer, "Edward"));

    printf("%s\n", add(listPointer, "Gabriel"));
    printf("%s\n", add(listPointer, "Joseph"));
    printf("%s\n", add(listPointer, "Edward"));
    printf("%s\n", add(listPointer, "Guilherme"));

    printf("%s\n", show(listPointer, "Edward"));

    remove_node(listPointer, "Guilherme");

    printf("%s\n", show(listPointer, "Maria"));
    printf("%s\n", show(listPointer, "Gabriel"));
    printf("%s\n", show(listPointer, "Edward"));
    printf("%s\n", show(listPointer, "Guilherme"));
    printf("%s\n", show(listPointer, "Joseph"));

    return 0;
}