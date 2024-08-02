#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Função para inicializar a pilha
void initialize_Stack(Stack *stackList) {
    stackList->head = NULL;
    stackList->tail = NULL;
}

// Função para inicializar a fila
void initialize_Queue(Queue *queueList) {
    queueList->head = NULL;
    queueList->tail = NULL;
}

// Função para adicionar um documento à pilha
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

// Função para adicionar um documento à fila
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

// Função para obter o tamanho da fila
int queue_size(Queue* queue) {
    int size = 0;
    QueueNode* current = queue->head;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

// Função para inicializar os slots das impressoras
void initialize_printersSlot(int printersSlot[], int number_printers, Queue* docQueue, Printing *printing) {
    int size = queue_size(docQueue);
    if (size < number_printers) {
        fprintf(stderr, "Erro: docQueue não contém elementos suficientes.\n");
        exit(EXIT_FAILURE);
    }

    docQueue->current = docQueue->head;

    for (int i = 0; i < number_printers; i++) {
        if (docQueue->current == NULL) {
            fprintf(stderr, "Erro inesperado: docQueue ficou vazia.\n");
            exit(EXIT_FAILURE);
        }

        printersSlot[i] = docQueue->current->number_pages;
        strcpy(printing[i].docName, docQueue->current->docName);
        printing[i].number_pages = docQueue->current->number_pages;
        docQueue->current = docQueue->current->next;
    }
}

// Função que distribui documentos
void docDistributuion(FILE* output, int printersSlot[], int number_printers, Queue* docQueue, Printing* printing, char** printersName, Stack* printedPaperStack, Stack** printLogs) {
    for(int i = 0; i < number_printers; i++) {
        if(printersSlot[i] == 0) {
            addStack(printedPaperStack, printing[i].docName, printing[i].number_pages);
            addStack(printLogs[i], printing[i].docName, printing[i].number_pages);

            StackNode* node = printLogs[i]->tail;
            printf("[%s] ", printersName[i]);
            fprintf(output, "[%s] ", printersName[i]);

            while (node != NULL) {
                if (node->docName != NULL) {
                    printf("%s-%dp ", node->docName, node->pagesNumber);
                    fprintf(output, "%s-%dp ", node->docName, node->pagesNumber);
                }
                node = node->previous;
            }
            printf("\n");
            fprintf(output, "\n");

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

// Função que encontra o menor valor no array
int lowestArrayNumber(int printersSlot[], int number_printers) {
    int lowest = printersSlot[0];
    for(int i = 1; i < number_printers; i++) {
        if (printersSlot[i] < lowest)
            lowest = printersSlot[i];
    }
    return lowest;
}

// Função que subtrai o menor valor do array
void subtract_number_array(int printersSlot[], int number_printers, int lowestArrayNumber) {
    for(int i = 0; i < number_printers; i++)
        printersSlot[i] -= lowestArrayNumber;
}

// Função para comparar e imprimir documentos
void last_compare(FILE *output, int printersSlot[], int number_printers, int lowestArrayNumber, Printing *printing, char** printersName, Stack** printLogs, Stack* printedPaperStack) {
    for(int i = 0; i < number_printers; i++) {
        if(printersSlot[i] == 0) {
            addStack(printLogs[i], printing[i].docName, printing[i].number_pages);
            addStack(printedPaperStack, printing[i].docName, printing[i].number_pages);

            StackNode* node = printLogs[i]->tail;
            printf("[%s] ", printersName[i]);
            fprintf(output, "[%s] ", printersName[i]);

            while (node != NULL) {
                if (node->docName != NULL) {
                    printf("%s-%dp ", node->docName, node->pagesNumber);
                    fprintf(output, "%s-%dp ", node->docName, node->pagesNumber);
                }
                node = node->previous;
            }
            printf("\n");
            fprintf(output, "\n");
        }
    }
}

// Função que encontra o menor valor não zero no array
int last_lowestArrayNumber(int printersSlot[], int number_printers) {
    int lowest = printersSlot[0];
    for(int i = 1; i < number_printers; i++) {
        if(printersSlot[i] < lowest && printersSlot[i] != 0)
            lowest = printersSlot[i];
    }
    return lowest;
}

// Função que substitui zeros no array por um valor alto
void replaceZeros(int printersSlot[], int number_printers) {
    for(int i = 0; i < number_printers; i++) {
        if(printersSlot[i] == 0) 
            printersSlot[i] = 1000000;
    }
}

// Função para ler as linhas do arquivo
LineFile getFileLines(FILE* input) {
    char commandLine[10];
    char nameLine[51];
    LineFile lineFile = {0, NULL};

    if (fgets(commandLine, sizeof(commandLine), input)) {
        int commandsNumber = atoi(commandLine);

        lineFile.linesNumber = commandsNumber;
        lineFile.linesArray = malloc(commandsNumber * sizeof(char*));
        if (lineFile.linesArray == NULL) {
            fprintf(stderr, "Erro ao alocar memória para linesArray\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < commandsNumber; i++) {
            if (fgets(nameLine, sizeof(nameLine), input)) {
                size_t len = strlen(nameLine);
                if (nameLine[len - 1] == '\n') {
                    nameLine[len - 1] = '\0';
                    len--;
                }

                lineFile.linesArray[i] = malloc((len + 1) * sizeof(char));
                if (lineFile.linesArray[i] == NULL) {
                    fprintf(stderr, "Erro ao alocar memória para linesArray[%d]\n", i);
                    exit(EXIT_FAILURE);
                }

                strcpy(lineFile.linesArray[i], nameLine);
            } else {
                fprintf(stderr, "Erro ao ler a linha %d\n", i);
                exit(EXIT_FAILURE);
            }
        }
    } else {
        fprintf(stderr, "Erro ao ler o comando inicial\n");
        exit(EXIT_FAILURE);
    }

    return lineFile;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return EXIT_FAILURE;
    }

    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        fclose(input);
        return EXIT_FAILURE;
    }

    Stack printedPapersStack;
    Stack *printedPapersStack_Pointer = &printedPapersStack;
    initialize_Stack(printedPapersStack_Pointer);

    Stack printLog;
    Stack *printLog_pointer = &printLog;
    initialize_Stack(printLog_pointer);

    Queue docNameQueue;
    Queue *docNameQueue_Pointer = &docNameQueue;
    initialize_Queue(docNameQueue_Pointer);

    LineFile printers = getFileLines(input);
    int all_number_pages = 0;
    int number_printers = printers.linesNumber;
    Stack** printLogs = malloc(sizeof(Stack*) * number_printers);

    if (printLogs == NULL) {
        fprintf(stderr, "Erro ao alocar memória para printLogs\n");
        fclose(input);
        fclose(output);
        return EXIT_FAILURE;
    }
    
    for(int i = 0; i < number_printers; i++) {
        printLogs[i] = malloc(sizeof(Stack)); // Itialize a stack for each printer
        if (printLogs[i] == NULL) {
            fprintf(stderr, "Erro ao alocar memória para printLogs[%d]\n", i);
            fclose(input);
            fclose(output);
            return EXIT_FAILURE;
        }
        initialize_Stack(printLogs[i]);
    }

    Printing printed_Documents[number_printers];
    LineFile documents = getFileLines(input);
    fclose(input);


    documents.numberPages = (int*) malloc(documents.linesNumber * sizeof(int));
    if (documents.numberPages == NULL) {
        fprintf(stderr, "Erro ao alocar memória para documents.numberPages\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < documents.linesNumber; i++) {
        char *line = strdup(documents.linesArray[i]);
        if (line == NULL) {
            fprintf(stderr, "Erro ao alocar memória para duplicar a linha\n");
            exit(EXIT_FAILURE);
        }

        char *docName = strtok(line, " ");
        char *numberPagesStr = strtok(NULL, "");

        if (docName != NULL) {
        } else {
            printf("Erro: Nome do documento não encontrado na linha %d\n", i);
        }

        int number_Pages = atoi(numberPagesStr);
   
        all_number_pages += number_Pages;       
        addQueue(docNameQueue_Pointer, docName, number_Pages);
        free(line); 
    }


    QueueNode *current = docNameQueue_Pointer->head;
    while (current != NULL) {
        current = current->next;
    }

    int printersSlot[number_printers];

    docNameQueue_Pointer->current = docNameQueue_Pointer->head;
    initialize_printersSlot(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents);

    // iteration responsible to distribute the documents on the printerSlot, subract the lowest and replace the zeros
    while (1) {
        int lowestNumber = lowestArrayNumber(printersSlot, number_printers);
        subtract_number_array(printersSlot, number_printers, lowestNumber);
        docDistributuion(output, printersSlot, number_printers, docNameQueue_Pointer, printed_Documents, printers.linesArray, printedPapersStack_Pointer, printLogs);

        if (docNameQueue_Pointer->current == docNameQueue_Pointer->tail) {
            lowestNumber = lowestArrayNumber(printersSlot, number_printers);
            subtract_number_array(printersSlot, number_printers, lowestNumber);
            docDistributuion(output, printersSlot, number_printers, docNameQueue_Pointer, printed_Documents, printers.linesArray, printedPapersStack_Pointer, printLogs);

            for (int i = 0; i < number_printers - 1; i++) {
                lowestNumber = lowestArrayNumber(printersSlot, number_printers);
                subtract_number_array(printersSlot, number_printers, lowestNumber);
                last_compare(output, printersSlot, number_printers, lowestNumber, printed_Documents, printers.linesArray, printLogs, printedPapersStack_Pointer);
                replaceZeros(printersSlot, number_printers);
                last_compare(output, printersSlot, number_printers, lowestNumber, printed_Documents, printers.linesArray, printLogs, printedPapersStack_Pointer);
            }
           
            break;
        }
    }

    printf("%d-p\n", all_number_pages);
    fprintf(output,"%d-p\n", all_number_pages);
    printedPapersStack_Pointer->current = printedPapersStack_Pointer->tail;

    
    while (1) {
        if (printedPapersStack_Pointer->current == printedPapersStack_Pointer->head) {
            printf("%s-%dp\n", printedPapersStack_Pointer->head->docName, printedPapersStack_Pointer->head->pagesNumber);
            fprintf(output, "%s-%dp\n", printedPapersStack_Pointer->head->docName, printedPapersStack_Pointer->head->pagesNumber);

            break;
        } else {
            printf("%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            fprintf(output,"%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            printedPapersStack_Pointer->current = printedPapersStack_Pointer->current->previous;
        }
    }

    fclose(output);

    // free memory
    for (int i = 0; i < number_printers; i++) {
        free(printers.linesArray[i]);
        free(printLogs[i]);
    }
    free(printers.linesArray);
    free(documents.numberPages);
    free(printLogs);
    
    return 0;
}
