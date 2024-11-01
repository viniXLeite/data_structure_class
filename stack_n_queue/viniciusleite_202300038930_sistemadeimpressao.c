#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _stackNode {
    char *docName;
    int pagesNumber;
    struct _stackNode *next;
    struct _stackNode *previous;
} StackNode;

typedef struct _stack {
    StackNode *head;
    StackNode *current;
    StackNode *tail;
} Stack;

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

typedef struct _LineFile {
    int linesNumber;
    char** linesArray;
    int *numberPages;
} LineFile;

void initialize_Stack(Stack *stackList) {
    stackList->head = NULL;
    stackList->tail = NULL;
}

void initialize_Queue(Queue *queueList) {
    queueList->head = NULL;
    queueList->tail = NULL;
}

// To know if the first line of a file is a number
int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// Function responsible to read n lines of a file and store them to a Line variable 
LineFile read_lines_based_on_number(FILE* input) {
    LineFile Line;

    if (input == NULL) {
        printf("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char line[100];
    if (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = '\0';
        if (is_number(line)) {
            int count = atoi(line);
            Line.linesNumber = count;
            Line.linesArray = (char**) malloc(Line.linesNumber*51);

            for (int i = 0; i < count; i++) {
                if (fgets(line, sizeof(line), input)) {
                    // Removes the new line character 
                    line[strcspn(line, "\n")] = '\0';
                    Line.linesArray[i] = (char*) malloc(strlen(line)+1);
                    strcpy(Line.linesArray[i], line);
                }
            }
        } else {
            printf("A primeira linha do arquivo não é um número válido.\n");
        }
    }
    return Line;
}

void addQueue(Queue *queueList, char *name, int number_pages) {
    QueueNode *queueNode = (QueueNode*) malloc(sizeof(QueueNode));
    if (queueNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para QueueNode\n");
        exit(EXIT_FAILURE);
    }

    queueNode->docName = (char*) malloc(strlen(name) + 1);
    if (queueNode->docName == NULL) {
        fprintf(stderr, "Erro ao alocar memória para docName\n");
        exit(EXIT_FAILURE);
    }

    strcpy(queueNode->docName, name);
    queueNode->number_pages = number_pages;
    queueNode->next = NULL;

    if (queueList->head == NULL) {
        queueList->head = queueNode;
        queueNode->previous = NULL;
    } else {
        queueList->tail->next = queueNode;
        queueNode->previous = queueList->tail;
    }

    queueList->tail = queueNode;
}

void addStack(Stack *stackList, char *name, int pagesNumber) {
    StackNode *stackNode = (StackNode*) malloc(sizeof(StackNode));
    if (stackNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para StackNode\n");
        exit(EXIT_FAILURE);
    }

    stackNode->docName = (char*) malloc(strlen(name) + 1);
    if (stackNode->docName == NULL) {
        fprintf(stderr, "Erro ao alocar memória para docName\n");
        free(stackNode); // Libera o nó já alocado antes de sair
        exit(EXIT_FAILURE);
    }
    strcpy(stackNode->docName, name);
    stackNode->pagesNumber = pagesNumber;

    if (stackList->head == NULL) {
        stackList->head = stackNode;
        stackList->tail = stackNode;  // Corrige a inicialização da cauda
        stackNode->previous = NULL;
    } else {
        stackList->tail->next = stackNode;
        stackNode->previous = stackList->tail;
        stackList->tail = stackNode;  // Atualiza a cauda
    }

    stackNode->next = NULL;
}


int separateNumberFromString(const char *str) {
    char number_str[10];
    char name[51];
    int number;
    
    sscanf(str, "%s %s", name, number_str);
    number = atoi(number_str);
    
    return number;
}

// Initialize the printers' slot, in order to store on it the docQueue components

// Print out every printersSlot iteration 
void initialize_printersSlot(FILE* output, char** printersName, int* printerSlot, int number_printers, Queue* docQueue_Pointer, Printing* Printing, Stack** printLogs) {
    docQueue_Pointer->current = docQueue_Pointer->head;

    for(int i = 0; i <= number_printers-1; i++) { 
        if(docQueue_Pointer->current == NULL) {
            printerSlot[i] = 1000000000;
            continue;
        }
        printerSlot[i] = docQueue_Pointer->current->number_pages;
        strcpy(Printing[i].docName, docQueue_Pointer->current->docName);
        Printing[i].number_pages = docQueue_Pointer->current->number_pages;
        fprintf(output, "[%s] %s-%dp\n", printersName[i], Printing[i].docName, Printing[i].number_pages );
        addStack(printLogs[i], Printing[i].docName, Printing[i].number_pages);

        docQueue_Pointer->current = docQueue_Pointer->current->next;
    }

}

// Determines the lowest number of an array, in this case the array is the printersSlot
int lowestArrayNumber(int printersSlot[], int number_printers) {
    int lowest = printersSlot[0];
    for(int i = 0; i <= number_printers-1; i++) {
        if (printersSlot[i] <= lowest)
            lowest = printersSlot[i];
    }
    return lowest;
}

// Implement a function to count the number of elements on printLogs[i] => if (number(printLogs[i]) == 1) => do not print ou anything, else => print out


// Distribute the pages' number of each document of the queue in the printers' slot, and print out the printed ones on a output file
void docDistributuion(FILE* output, int printersSlot[], int number_printers, Queue* docQueue, Printing* printing, char** printersName, Stack* printedPaperStack, Stack** printLogs) {
    for (int i = 0; i < number_printers; i++) {
        if (printersSlot[i] == 0) {
            // Adiciona o documento à pilha de papel impresso
            addStack(printedPaperStack, printing[i].docName, printing[i].number_pages);

            if (docQueue->current == docQueue->tail) {
                // Processa o último documento da fila
                printersSlot[i] = docQueue->current->number_pages;
                strcpy(printing[i].docName, docQueue->current->docName);
                printing[i].number_pages = docQueue->current->number_pages;

                // Adiciona o documento ao log de impressão
                addStack(printLogs[i], printing[i].docName, printing[i].number_pages);
                StackNode* node = printLogs[i]->tail;
                fprintf(output, "[%s] ", printersName[i]);

                while (node != NULL) {
                    fprintf(output, "%s-%dp", node->docName, node->pagesNumber);
                    node = node->previous;
                    if (node != NULL) {
                        fprintf(output, ", ");
                    }
                }
                fprintf(output, "\n");
                return;
            } else {
                // Processa o próximo documento da fila
                printersSlot[i] = docQueue->current->number_pages;
                strcpy(printing[i].docName, docQueue->current->docName);
                printing[i].number_pages = docQueue->current->number_pages;

                // Adiciona o documento ao log de impressão
                addStack(printLogs[i], printing[i].docName, printing[i].number_pages);
                docQueue->current = docQueue->current->next;
            }

            // Imprime o log de documentos
            StackNode* node = printLogs[i]->tail;
            fprintf(output, "[%s] ", printersName[i]);

            while (node != NULL) {
                fprintf(output, "%s-%dp", node->docName, node->pagesNumber);
                node = node->previous;
                if (node != NULL) {
                    fprintf(output, ", ");
                }
            }
            fprintf(output, "\n");
        }
    }
}


void subtract_number_array(int printersSlot[], int number_printers, int lowestArrayNumber) {
    for(int i = 0; i < number_printers; i++)
        printersSlot[i] = printersSlot[i] - lowestArrayNumber;
}

// Treats the case of docDistribution when the node = docQueue_Pointer->tail
void last_compare(FILE *output, int printersSlot[], int number_printers, int lowestArrayNumber, Printing *printing, char** printersName, Stack** printLogs, Stack* printedPaperStack) {
    for(int i = 0; i < number_printers; i++) {
        if(printersSlot[i] == 0) {
            addStack(printedPaperStack, printing[i].docName, printing[i].number_pages);
        }
    }
}

void replaceZeros(int printersSlot[], int number_printers) {
    for(int i = 0; i < number_printers; i++) {
        if(printersSlot[i] == 0) 
            printersSlot[i] = 1000000;
    }
}


int main(int argc, char* argv[]) {
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    Stack printedPapersStack;
    Stack *printedPapersStack_Pointer = &printedPapersStack;
    initialize_Stack(printedPapersStack_Pointer);

    Stack printLog;
    Stack *printLog_pointer = &printLog;
    initialize_Stack(printLog_pointer);

    Queue docNameQueue;
    Queue *docNameQueue_Pointer = &docNameQueue;
    initialize_Queue(docNameQueue_Pointer);

    int total_pages = 0; 

    // Checks printers
    LineFile printers = read_lines_based_on_number(input);
    printf("--Read File--\nNumber of printers: %d\n",printers.linesNumber);
    for(int i = 0; i <= printers.linesNumber-1; i++) {
    }

    // Checks Documents
    LineFile Documents = read_lines_based_on_number(input);
    int number_Documents = Documents.linesNumber;
    char tempName[51];
    char tempInt[51];
    printf("\n--Read File--\nNumber of Documents: %d\n",Documents.linesNumber);
    Documents.numberPages = (int*) malloc(Documents.linesNumber*sizeof(int));

    // Separates number pages of each Document and then checks it
    for(int i = 0; i <= Documents.linesNumber-1; i++) {
        Documents.numberPages[i] = separateNumberFromString(Documents.linesArray[i]);
        sscanf(Documents.linesArray[i], "%s %s", tempName, tempInt);
        strcpy(Documents.linesArray[i], tempName);
    }

    // Checks number_printers and number_Documents
    int number_printers = printers.linesNumber;
    printf("\nNumber of printers (variable): %d\nNumber of Documents (variable): %d\n\n", number_printers, number_Documents);

    // Distribute the documents in the queue
    for(int i = 0; i <= number_Documents-1; i++) {
        addQueue(docNameQueue_Pointer, Documents.linesArray[i], Documents.numberPages[i]);
    }

    // Checks the Queue
    QueueNode* nodeQueueDoc = docNameQueue_Pointer->head;
    printf("--Queue--\n");
    while(nodeQueueDoc != NULL) {
        total_pages += nodeQueueDoc->number_pages;
        nodeQueueDoc = nodeQueueDoc->next;
    }

    Stack** printLogs = malloc(sizeof(Stack*) * number_printers); 
    for(int i = 0; i < number_printers; i++) {
        printLogs[i] = malloc(sizeof(Stack)); // Itialize a stack for each printer
        initialize_Stack(printLogs[i]);
    }
    printf("\n--Ok initalize printsLogs--\n\n");
    // Declaress printersSlot and initializes it and treats the case of number_documents < number_printers
    int printersSlot[number_printers];
    Printing Printing[number_printers];
    initialize_printersSlot(output, printers.linesArray, printersSlot, number_printers, docNameQueue_Pointer, Printing, printLogs);
    printf("\n--PrintesSlot--\n");

    int lowestNumber;

    while (1) {
        lowestNumber = lowestArrayNumber(printersSlot, number_printers);
        subtract_number_array(printersSlot, number_printers, lowestNumber);
        docDistributuion(output, printersSlot, number_printers, docNameQueue_Pointer, Printing, printers.linesArray, printedPapersStack_Pointer, printLogs);
        
        if (docNameQueue_Pointer->current == docNameQueue_Pointer->tail) {
            lowestNumber = lowestArrayNumber(printersSlot, number_printers);
            subtract_number_array(printersSlot, number_printers, lowestNumber);
            docDistributuion(output, printersSlot, number_printers, docNameQueue_Pointer, Printing, printers.linesArray, printedPapersStack_Pointer, printLogs);

            for (int i = 0; i < number_printers - 57; i++) {
                lowestNumber = lowestArrayNumber(printersSlot, number_printers);
                subtract_number_array(printersSlot, number_printers, lowestNumber);
                last_compare(output, printersSlot, number_printers, lowestNumber, Printing, printers.linesArray, printLogs, printedPapersStack_Pointer);
                replaceZeros(printersSlot, number_printers);
                //last_compare(output, printersSlot, number_printers, lowestNumber, Printing, printers.linesArray, printLogs, printedPapersStack_Pointer);
            }
           
            break;
        }
    }

    fprintf(output,"%d-p\n", total_pages);
    printf("%d-p\n", total_pages);

    printedPapersStack_Pointer->current = printedPapersStack_Pointer->tail;


    while (printedPapersStack_Pointer->current != NULL) {
        fprintf(output, "%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
        
        if (printedPapersStack_Pointer->current == printedPapersStack_Pointer->head) {
            break;
        }
        printedPapersStack_Pointer->current = printedPapersStack_Pointer->current->previous;
    }
    


    
    fclose(input);
    fclose(output);
    return 0;
}