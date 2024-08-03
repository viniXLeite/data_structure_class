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

// Função para inicializar a fila
void initialize_Queue(Queue *queueList) {
    queueList->head = NULL;
    queueList->tail = NULL;
}


int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

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
                    // Remove o caractere de nova linha, se presente
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
    //if (stackNode->docName == NULL) {
        //fprintf(stderr, "Erro ao alocar memória para docName\n");
        //exit(EXIT_FAILURE);
    //}

    strcpy(stackNode->docName, name);
    stackNode->pagesNumber = pagesNumber;

    if (stackList->head == NULL) {
        stackList->head = stackNode;
        stackNode->previous = NULL;
    } else {
        stackList->tail->next = stackNode;
        stackNode->previous = stackList->tail;
    }

    stackNode->next = NULL;
    stackList->tail = stackNode;
}

int separarNumero(const char *str) {
    char numero_str[10];
    char nome[51];
    int numero;
    
    // Usando sscanf para separar a string em partes
    sscanf(str, "%s %s", nome, numero_str);
    
    // Convertendo a parte numérica para inteiro
    numero = atoi(numero_str);
    
    return numero;
}

void initialize_printersSlot(int* printerSlot, int number_printers, Queue* docQueue_Pointer, Printing* Printing) {
    docQueue_Pointer->current = docQueue_Pointer->head;

    for(int i = 0; i <= number_printers-1; i++) { 
        if(docQueue_Pointer->current == NULL) {
            printerSlot[i] = 1000000000;
            continue;
        }
        printerSlot[i] = docQueue_Pointer->current->number_pages;
        strcpy(Printing[i].docName, docQueue_Pointer->current->docName);
        Printing[i].number_pages = docQueue_Pointer->current->number_pages;
        docQueue_Pointer->current = docQueue_Pointer->current->next;
    }

}


//---------//

int lowestArrayNumber(int printersSlot[], int number_printers) {
    int lowest = printersSlot[0];
    for(int i = 1; i < number_printers; i++) {
        if (printersSlot[i] < lowest)
            lowest = printersSlot[i];
    }
    return lowest;
}

void docDistributuion(FILE* output, int printersSlot[], int number_printers, Queue* docQueue, Printing* printing, char** printersName, Stack* printedPaperStack, Stack** printLogs) {
    for(int i = 0; i < number_printers; i++) {
        if(printersSlot[i] == 0) {
            addStack(printedPaperStack, printing[i].docName, printing[i].number_pages);
            addStack(printLogs[i], printing[i].docName, printing[i].number_pages);
            //printf("ok until addStack\n");

            StackNode* node = printLogs[i]->tail;
            //printf("[%s] ", printersName[i]);
            fprintf(output, "[%s] ", printersName[i]);

            while (node != NULL) {
                if (node->docName != NULL) {
                    //printf("%s-%dp ", node->docName, node->pagesNumber);
                    fprintf(output, "%s-%dp ", node->docName, node->pagesNumber);
                }
                node = node->previous;
            }
            fprintf(output, "\n");
            //printf("\n");

            if(docQueue->current == docQueue->tail) {
                printersSlot[i] = docQueue->current->number_pages;
                strcpy(printing[i].docName, docQueue->current->docName);
                printing[i].number_pages = docQueue->current->number_pages;
                return;
            } else {
                    printersSlot[i] = docQueue->current->number_pages;
                    strcpy(printing[i].docName, docQueue->current->docName);
                    printing[i].number_pages = docQueue->current->number_pages;
                    docQueue->current = docQueue->current->next;
            }
        }
    }
}

void subtract_number_array(int printersSlot[], int number_printers, int lowestArrayNumber) {
    for(int i = 0; i < number_printers; i++)
        printersSlot[i] -= lowestArrayNumber;
}

void last_compare(FILE *output, int printersSlot[], int number_printers, int lowestArrayNumber, Printing *printing, char** printersName, Stack** printLogs, Stack* printedPaperStack) {
    for(int i = 0; i < number_printers; i++) {
        if(printersSlot[i] == 0) {
            addStack(printLogs[i], printing[i].docName, printing[i].number_pages);
            addStack(printedPaperStack, printing[i].docName, printing[i].number_pages);

            StackNode* node = printLogs[i]->tail;
            //printf("[%s] ", printersName[i]);
            fprintf(output, "[%s] ", printersName[i]);

            while (node != NULL) {
                if (node->docName != NULL) {
                    //printf("%s-%dp ", node->docName, node->pagesNumber);
                    fprintf(output, "%s-%dp ", node->docName, node->pagesNumber);
                }
                node = node->previous;
            }
            //printf("\n");
            fprintf(output, "\n");
        }
    }
}

int last_lowestArrayNumber(int printersSlot[], int number_printers) {
    int lowest = printersSlot[0];
    for(int i = 1; i < number_printers; i++) {
        if(printersSlot[i] < lowest && printersSlot[i] != 0)
            lowest = printersSlot[i];
    }
    return lowest;
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
        //printf("Printer: %s\n", printers.linesArray[i]);
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
        Documents.numberPages[i] = separarNumero(Documents.linesArray[i]);
        sscanf(Documents.linesArray[i], "%s %s", tempName, tempInt);
        strcpy(Documents.linesArray[i], tempName);
        //printf("Doc: %s, pages: %d\n", Documents.linesArray[i], Documents.numberPages[i]);
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
        //printf("Queue: %s--%dp\n", nodeQueueDoc->docName, nodeQueueDoc->number_pages);
        total_pages += nodeQueueDoc->number_pages;
        nodeQueueDoc = nodeQueueDoc->next;
    }

    // Declaress printersSlot and initializes it and treats the case of number_documents < number_printers
    int printersSlot[number_printers];
    Printing Printing[number_printers];
    initialize_printersSlot(printersSlot, number_printers, docNameQueue_Pointer, Printing);
    printf("\n--PrintesSlot--\n");
    //for(int i = 0; i <= number_printers-1; i++) {
        //printf("printersSlot: %d\n", printersSlot[i]);
    //}

    Stack** printLogs = malloc(sizeof(Stack*) * number_printers); 
    for(int i = 0; i < number_printers; i++) {
        printLogs[i] = malloc(sizeof(Stack)); // Itialize a stack for each printer
        initialize_Stack(printLogs[i]);
    }
    printf("\n--Ok initalize printsLogs--\n\n");

    while (1) {
        int lowestNumber = lowestArrayNumber(printersSlot, number_printers);
        subtract_number_array(printersSlot, number_printers, lowestNumber);
        docDistributuion(output, printersSlot, number_printers, docNameQueue_Pointer, Printing, printers.linesArray, printedPapersStack_Pointer, printLogs);
        if (docNameQueue_Pointer->current == docNameQueue_Pointer->tail) {
            lowestNumber = lowestArrayNumber(printersSlot, number_printers);
            subtract_number_array(printersSlot, number_printers, lowestNumber);
            docDistributuion(output, printersSlot, number_printers, docNameQueue_Pointer, Printing, printers.linesArray, printedPapersStack_Pointer, printLogs);

            for (int i = 0; i < number_printers - 1; i++) {
                lowestNumber = lowestArrayNumber(printersSlot, number_printers);
                subtract_number_array(printersSlot, number_printers, lowestNumber);
                last_compare(output, printersSlot, number_printers, lowestNumber, Printing, printers.linesArray, printLogs, printedPapersStack_Pointer);
                replaceZeros(printersSlot, number_printers);
                last_compare(output, printersSlot, number_printers, lowestNumber, Printing, printers.linesArray, printLogs, printedPapersStack_Pointer);
            }
           
            break;
        }
    }

    fprintf(output,"%d-p\n", total_pages);
    printf("%d-p\n", total_pages);
    printedPapersStack_Pointer->current = printedPapersStack_Pointer->tail;

    
    /* if(number_Documents < number_printers) {
            total_slots = number_printers - number_Documents
            printersSlot2[total_slots]
            initialize(printersSlot, total_slots)
            
            segmentation fault nesse caso em else do docdtribution quando numero de documentos e menor que o de impressoras 
            e o loop nao para quando igual
    */

    while (1) {
        if (printedPapersStack_Pointer->current == printedPapersStack_Pointer->head) {
            //printf("%s-%dp\n", printedPapersStack_Pointer->head->docName, printedPapersStack_Pointer->head->pagesNumber);
            fprintf(output, "%s-%dp\n", printedPapersStack_Pointer->head->docName, printedPapersStack_Pointer->head->pagesNumber);

            break;
        } else {
            //printf("%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            fprintf(output,"%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            printedPapersStack_Pointer->current = printedPapersStack_Pointer->current->previous;
        }
    }

    printedPapersStack_Pointer->current = printedPapersStack_Pointer->tail;
    
    while (1) {
        if (printedPapersStack_Pointer->current == printedPapersStack_Pointer->head) {
            //printf("%s-%dp\n", printedPapersStack_Pointer->head->docName, printedPapersStack_Pointer->head->pagesNumber);
            fprintf(output, "%s-%dp\n", printedPapersStack_Pointer->head->docName, printedPapersStack_Pointer->head->pagesNumber);

            break;
        } else {
            //printf("%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            fprintf(output,"%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            printedPapersStack_Pointer->current = printedPapersStack_Pointer->current->previous;
        }
    }


    fclose(input);
    fclose(output);
    return 0;
}