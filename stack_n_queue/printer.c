#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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
   int number_pages;
   struct _queueNode *next;
   struct _queueNode *previous;
} QueueNode;


typedef struct _queue {
   QueueNode *head;
   QueueNode *current;
   QueueNode *tail;
} Queue;


typedef struct _printer {
   char *printerName;
   unsigned short position;
} Printer;


typedef struct _printing_document {
   char docName[51];
   int number_pages;
} Printing;






void initialize_Stack(Stack *stackList) {
   stackList->head = NULL;
   stackList->tail = NULL;
}


void initialize_Queue(Queue *queueList) {
   queueList->head = NULL;
   queueList->tail = NULL;
}


void initialize_printersSlot(int printersSlot[], int number_printers, Queue* docQueue, Printing *printing) {
   for(int i=0; i <= number_printers-1; i++) {
       printersSlot[i] = docQueue->current->number_pages;
       strcpy(printing[i].docName, docQueue->current->docName);
       printing[i].number_pages = docQueue->current->number_pages;
       docQueue->current = docQueue->current->next;
   }
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


void addQueue(Queue *queueList, char *name, int number_pages) {
   QueueNode *queueNode = (QueueNode*) malloc(sizeof(QueueNode));
   queueNode->docName = (char*) malloc(sizeof(name));
   strcpy(queueNode->docName, name);
   queueNode->number_pages = number_pages;


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


void traverseQueue(Queue *queue) {
   QueueNode *node = queue->head;
   while(node != queue->tail) {
       printf("%s\n", node->docName);
       node = node->next;
   }
   printf("%s\n", queue->tail->docName);
}




void docDistributuion(int printersSlot[], int number_printers, Queue* docQueue, Printing* printing) {


   for(int i = 0; i <= number_printers-1; i++) {
       if(printersSlot[i] == 0) {
           printf("//printed %s => %d pages\n", printing[i].docName, printing[i].number_pages);


           if(docQueue->current == docQueue->tail) {
               printersSlot[i] = docQueue->current->number_pages;
               strcpy(printing[i].docName, docQueue->current->docName);
               printing[i].number_pages = docQueue->current->number_pages;
               return;
           }
           else {
               printersSlot[i] = docQueue->current->number_pages;
               strcpy(printing[i].docName, docQueue->current->docName);
               printing[i].number_pages = docQueue->current->number_pages;
               docQueue->current = docQueue->current->next;
           }
       }
   }
}


int lowestArrayNumber(int printersSlot[], int number_printers) {
   int lowest = printersSlot[0];
   for(int i=0; i <= number_printers-1; i++) {
       if(printersSlot[i] <= lowest)
           lowest = printersSlot[i];
   }
   // Subtract lowest
   return lowest;
}


void subtract_number_array(int printersSlot[], int number_printers, int lowestArrayNumber) {
   for(int i=0; i <= number_printers-1; i++)
       printersSlot[i] = printersSlot[i]-lowestArrayNumber;
}


int main() {
   int printedPages = 0;


   // Use the file system to save the printers's names to an array of printers
   // Save the documents on a queue and dequeue every time the document is sent to a printer
   // While (queue->head != NULL) {take the documents, choose an available printer, print(every time the printer attribute printing is 0, choose another file)
   // save it to a stack, increment the pages' counter, show the printer1s name ++ printed document-documentPages}
   // Show the pages's counter++p, and the printed files++documentPages


   Stack printedPapersStack;
   Stack *printedPapersStack_Pointer = &printedPapersStack;
   initialize_Stack(printedPapersStack_Pointer);


   Queue docNameQueue;
   Queue *docNameQueue_Pointer = &docNameQueue;
   initialize_Queue(docNameQueue_Pointer);


   int number_printers = 5;
   int printersSlot[number_printers];


   Printing printed_Documents[number_printers];


   addQueue(docNameQueue_Pointer, "doc1", 7);
   addQueue(docNameQueue_Pointer, "doc2", 5);
   addQueue(docNameQueue_Pointer, "doc3", 12);
   addQueue(docNameQueue_Pointer, "doc4", 25);
   addQueue(docNameQueue_Pointer, "doc5", 6);
   addQueue(docNameQueue_Pointer, "doc6", 9);
   addQueue(docNameQueue_Pointer, "doc7", 7);
   addQueue(docNameQueue_Pointer, "doc8", 12);
   addQueue(docNameQueue_Pointer, "doc9", 5);
   addQueue(docNameQueue_Pointer, "doc10", 9);
   addQueue(docNameQueue_Pointer, "doc12", 4);
   addQueue(docNameQueue_Pointer, "doc13", 5);
   addQueue(docNameQueue_Pointer, "doc14", 2);




   docNameQueue_Pointer->current = docNameQueue_Pointer->head;
   initialize_printersSlot(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents);


   int lowestNumber;
   QueueNode* node = docNameQueue_Pointer->head;


   printf("%s, %d\n", docNameQueue_Pointer->tail->docName, docNameQueue_Pointer->tail->number_pages);


   for(int i=0; i <= number_printers-1; i++) {
       printf("%d ", printersSlot[i]);
   }


   while(1) {
       lowestNumber = lowestArrayNumber(printersSlot, number_printers);
       subtract_number_array(printersSlot, number_printers, lowestNumber);


       //printf("%s => %d\n\n", node->docName, node->number_pages);


       printf("\n");
       for(int i=0; i <= number_printers-1; i++) {
           printf("%d ", printersSlot[i]);
       }


       docDistributuion(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents);


       printf("\n");
       for(int i=0; i <= number_printers-1; i++) {
           printf("%d ", printersSlot[i]);
       }


       if(docNameQueue_Pointer->current == docNameQueue_Pointer->tail) {
           lowestNumber = lowestArrayNumber(printersSlot, number_printers);
           subtract_number_array(printersSlot, number_printers, lowestNumber);


           printf("\n");
           for(int i=0; i <= number_printers-1; i++) {
               printf("%d ", printersSlot[i]);
           }


           docDistributuion(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents);


           printf("\n");
           for(int i=0; i <= number_printers-1; i++) {
               printf("%d ", printersSlot[i]);
           }


           lowestNumber = lowestArrayNumber(printersSlot, number_printers);
           subtract_number_array(printersSlot, number_printers, lowestNumber);


           printf("\n");
           for(int i=0; i <= number_printers-1; i++) {
               printf("%d ", printersSlot[i]);
           }


           break;
       }
   // Find out why the iteration is not working properly, it makes the last distrubuition n keep sustituing the tail->number_doc
   // Im distribuiting with queue->current and iterating with node
   // implement a funtion to sum all the doc->numberpages
   }


   return 0;
}

