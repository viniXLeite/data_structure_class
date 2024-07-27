#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The documents firstly must be organized in a queue structure
// The file and printers' names may have letters and numbers
// The number of pages related to each file will set a limit for some iteration
// There must be a stack to organize the printed papers
// Output [file_name] [numb of pages]
// Allocate the printer's name in a vector of strings
// Before the next iteration and after the first check if each printers is available 
// [printer1, printer2, printer3]

typedef struct _stackNode {
    char *docName;
    struct _stackNode *next; 
    struct _stackNode *previous;
} StackNode;

typedef struct _stack {
    StackNode *head;
    StackNode *tail;
} Stack;

// Add a int printing attribute
typedef struct _queueNode {
    char *docName;
    struct _queueNode *next; 
    struct _queueNode *previous;
} QueueNode;

typedef struct _queue {
    QueueNode *head;
    QueueNode *tail;
} Queue;



void initialize_Stack(Stack *stackList) {
    stackList->head = NULL;
    stackList->tail = NULL;
}

void initialize_Queue(Queue *queueList) {
    queueList->head = NULL;
    queueList->tail = NULL;
}


void addStack(Stack *stackList, char *name) {
    StackNode *stackNode = (StackNode*) malloc(sizeof(StackNode));
    stackNode->docName = (char*) malloc(sizeof(name));
    strcpy(stackNode->docName, name);

    if(stackList->head == NULL) {
        stackList->head = stackNode;
        stackNode->previous = NULL;
    }
    else {
        stackList->tail->next = stackNode;
        stackNode->previous = stackList->tail;
    }

    stackNode->next = NULL;
    stackList->tail = stackNode;
}

void addQueue(Queue *queueList, char *name) {
    QueueNode *queueNode = (QueueNode*) malloc(sizeof(QueueNode));
    queueNode->docName = (char*) malloc(sizeof(name));
    strcpy(queueNode->docName, name);

    if (queueList->head == NULL) {
        queueList->head = queueNode;
        queueNode->previous = NULL;
    }
    else {
        queueList->tail->next = queueNode;
        queueNode->previous = queueList->tail;
    }

    queueNode->next = NULL;
    queueList->tail = queueNode;
}


// Funtion that removes stack's tail
// See this function is really necessaty maybe just need to implement a pop stack one
void removeStack(Stack *stackList) {
    if (stackList->head == NULL) {
        return;
    }
    else {
        StackNode *node = stackList->tail;
        stackList->tail->previous->next = NULL;
        stackList->tail = stackList->tail->previous;
        // See if it's possible to assign node->previous to NULL
        free(node);
    }
}

// Funtion that removes queue's head
// Add a case when tries to remove a one-element queue, should it assign list-head to NUlLL?
// Maybe count the number of nodes added to the queue
void removeQueue(Queue *queueList) {
    if (queueList->head == NULL) {
        return;
    }
    else {
        QueueNode *node = queueList->head;
        queueList->head->next->previous = NULL;
        queueList->head = queueList->head->next;
        // See if it's possible to assign node->next to NULL
        free(node);
    }
}

// Implement the read and write system
// It must read the first line and use it as a iteration limit to catch the printers names
// New line, read the number and use the same logic to get the docNames 
int main() {
    int printedPages = 0;

    // Use the file system to save the printers's names to an array of printers
    // Save the documents on a queue and dequeue every time the document is sent to a printer
    // While (queue->head != NULL) {take the documents, choose an available printer, print(every time the printer attribute printing is 0, choose another file)
    // save it to a stack, increment the pages' counter, show the printer1s name ++ printed document-documentPages}
    // Show the pages's counter++p, and the printed files++documentPages

    Stack printedPapersStack;
    Stack *printedPapersStack_Pointer = &printedPapersStack;

    Queue docNameQueue;
    Queue *docNameQueue_Pointer = &docNameQueue;

    addQueue(docNameQueue_Pointer, "doc1");
    addQueue(docNameQueue_Pointer, "doc2");
    addQueue(docNameQueue_Pointer, "doc3");
    addQueue(docNameQueue_Pointer, "doc4");

    return 0;
}