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

typedef struct _printing_document {
   char docName[51];
   int number_pages;
} Printing;

typedef struct _Printers {
    int linesNumber;
    char** linesArray;
    int *numberPrinters;

} Printers;

typedef struct _Document {
    int linesNumber;
    char** linesArray;
    int *number_document;

} Document;


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

void docDistributuion(int printersSlot[], int number_printers, Queue* docQueue, Printing* printing, char** printersName) {
    for(int i = 0; i <= number_printers-1; i++) {
        if(printersSlot[i] == 0) {
            printf("[%s] %s-%dp\n", printersName[i], printing[i].docName, printing[i].number_pages);
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

void last_subtract_number_array(int printersSlot[], int number_printers, int lowestArrayNumber) {
    for(int i=0; i <= number_printers-1; i++) {
        if (printersSlot != 0) {
            printersSlot[i] = printersSlot[i]-lowestArrayNumber;
        }
    }
}

void last_compare(int printersSlot[], int number_printers, int lowestArrayNumber, Printing *printing, char** printersName) {
    for(int i = 0; i <= number_printers-1; i++) {
        if(printersSlot[i] == 0) {
           printf("[%s] %s-%d", printersName[i], printing[i].docName, printing[i].number_pages);
        }
    }
}

int last_lowestArrayNumber(int printersSlot[], int number_printers) {
    int lowest = printersSlot[0];
    for(int i=0; i <= number_printers-1; i++) {
        if(printersSlot[i] <= lowest) {
            if(printersSlot[i] != 0)
                lowest = printersSlot[i];
        }
    }
    // Subtract lowest
    return lowest;
}

void replaceZeros(int printersSlot[], int number_ṕrinters) {
    for(int i = 0; i <= number_ṕrinters-1; i++) {
        if(printersSlot[i] == 0) 
            printersSlot[i] = 1000000;
    }
}

Printers getPrinters(FILE* input) {
    char commandLine[10];
    char nameLine[51];
    Printers Printers;

    if(fgets(commandLine, sizeof(commandLine), input)) {
        char charCommandsNumber = commandLine[0];
        int commandsNumber = charCommandsNumber - '0';

        Printers.linesNumber = commandsNumber;
        Printers.linesArray = malloc(commandsNumber*sizeof(char*)+1);

        for(int i = 0; i < commandsNumber; i++) {
            if (fgets(nameLine, sizeof(nameLine), input)) {
                Printers.linesArray[i] = malloc(sizeof(nameLine)+1);

                if(nameLine[strlen(nameLine)-1] == '\n') nameLine[strlen(nameLine)-1] = '\0';
                strcpy(Printers.linesArray[i], nameLine);
            }
        }
    }

    return Printers;
}

Document getDocuments(FILE* input) {
    char commandLine[10];
    char nameLine[51];
    Document Document;
    char *docName, *numberPagesStr;
    char numberPagesChar;
    int number_Pages;

    if(fgets(commandLine, sizeof(commandLine), input)) {
        char charCommandsNumber = commandLine[0];
        int commandsNumber = charCommandsNumber - '0';

        Document.linesNumber = commandsNumber;
        Document.linesArray = malloc(commandsNumber*sizeof(char*)+1);

        for(int i = 0; i < commandsNumber; i++) {
            if (fgets(nameLine, sizeof(nameLine), input)) {
                Document.linesArray[i] = malloc(sizeof(nameLine)+1);
                if(nameLine[strlen(nameLine)-1] == '\n') nameLine[strlen(nameLine)-1] = '\0';
                strcpy(Document.linesArray[i], nameLine);
                docName = strtok(Document.linesArray[i], " ");
                numberPagesStr = strtok(NULL, "");
                numberPagesChar = numberPagesStr[0];
                number_Pages = numberPagesChar - '0';
                Document.number_document[i] = number_Pages;

            }
        }
    }

    return Document;
}

int main(int argc, char* argv[]) {

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

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

    char *docName, *numberPagesStr;
    char numberPagesChar;
    int number_Pages;

    Printers printers = getPrinters(input);

    Printers documents = getPrinters(input);
    for(int i = 0; i <= documents.linesNumber-1; i++) {
        documents.numberPrinters = (int*) malloc(30);
        docName = strtok(documents.linesArray[i], " ");
        numberPagesStr = strtok(NULL, "");
        numberPagesChar = numberPagesStr[0];
        number_Pages = numberPagesChar - '0';
        documents.numberPrinters[i] = number_Pages;
        addQueue(docNameQueue_Pointer, documents.linesArray[i], documents.numberPrinters[i]);
    }


    int number_printers = printers.linesNumber;
    int printersSlot[number_printers];
    Printing printed_Documents[number_printers];

    char *printersName[51] = {"jatodetinta", "laser"};

    docNameQueue_Pointer->current = docNameQueue_Pointer->head;
    initialize_printersSlot(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents);

    int lowestNumber;
    QueueNode* node = docNameQueue_Pointer->head;

    while(1) {
        lowestNumber = lowestArrayNumber(printersSlot, number_printers);
        subtract_number_array(printersSlot, number_printers, lowestNumber);
        docDistributuion(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents, printersName);

        if(docNameQueue_Pointer->current == docNameQueue_Pointer->tail) {
            lowestNumber = lowestArrayNumber(printersSlot, number_printers);
            subtract_number_array(printersSlot, number_printers, lowestNumber);
            docDistributuion(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents, printersName);

            for(int i = 0; i <= number_printers-2; i++) {

                lowestNumber = last_lowestArrayNumber(printersSlot, number_printers);
                last_subtract_number_array(printersSlot, number_printers, lowestNumber);
                last_compare(printersSlot, number_printers, lowestNumber, printed_Documents, printersName);
                replaceZeros(printersSlot, number_printers);
                printf("\n");
            }
           
            break;
        }

    }
    
    printf("\nnumero de paginas impressas-p\n");
    fclose(input); 
    fclose(output);

    return 0;
}


// pass the output file ass agument of the last and thedistributuion functions fprint print out in the output and fprint when the printer gets the file
// In the the distribution functions add the printed file to the stack
// print the stack
// numeber of prited files