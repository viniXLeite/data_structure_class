#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


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
   //char **all_docs;
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

void addStack(Stack *stackList, char *name, int pagesNumber) {
    // Alocar memória para o novo node
    StackNode *stackNode = (StackNode*) malloc(sizeof(StackNode));
    if (stackNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para StackNode\n");
        exit(EXIT_FAILURE);
    }

    // Alocar memória para o docName
    stackNode->docName = (char*) malloc(strlen(name) + 1);
    if (stackNode->docName == NULL) {
        fprintf(stderr, "Erro ao alocar memória para docName\n");
        free(stackNode);
        exit(EXIT_FAILURE);
    }

    // Copiar o nome do documento
    strcpy(stackNode->docName, name);

    // Configurar o número de páginas
    stackNode->pagesNumber = pagesNumber;

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
    // Alocar memória para o novo node
    QueueNode *queueNode = (QueueNode*) malloc(sizeof(QueueNode));
    if (queueNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória para QueueNode\n");
        exit(EXIT_FAILURE);
    }

    // Alocar memória para o docName
    queueNode->docName = (char*) malloc(strlen(name) + 1);
    if (queueNode->docName == NULL) {
        fprintf(stderr, "Erro ao alocar memória para docName\n");
        free(queueNode);
        exit(EXIT_FAILURE);
    }

    // Copiar o nome do documento
    strcpy(queueNode->docName, name);

    // Configurar o número de páginas
    queueNode->number_pages = number_pages;
    queueNode->next = NULL;

    // Adicionar o node à fila
    if (queueList->head == NULL) {
        queueList->head = queueNode;
        queueNode->previous = NULL;
    } else {
        queueList->tail->next = queueNode;
        queueNode->previous = queueList->tail;
    }

    queueList->tail = queueNode;
}

int queue_size(Queue* queue) {
    int size = 0;
    QueueNode* current = queue->head; // Supondo que 'head' aponta para o início da fila
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
}

void initialize_printersSlot(int printersSlot[], int number_printers, Queue* docQueue, Printing *printing) {
    int size = queue_size(docQueue);
    printf("Tamanho da fila: %d\n", size);
    if (size < number_printers) {
        fprintf(stderr, "Erro: docQueue não contém elementos suficientes.\n");
        exit(EXIT_FAILURE);
    }

    docQueue->current = docQueue->head; // Certifique-se de começar do início da fila

    for (int i = 0; i < number_printers; i++) {
        if (docQueue->current == NULL) {
            fprintf(stderr, "Erro inesperado: docQueue ficou vazia.\n");
            exit(EXIT_FAILURE);
        }

        printersSlot[i] = docQueue->current->number_pages;

        // Certifique-se de que printing[i].docName está alocado corretamente
        if (printing[i].docName == NULL) {
            fprintf(stderr, "Erro: docName não foi alocado para a impressora %d.\n", i);
            exit(EXIT_FAILURE);
        }

        strcpy(printing[i].docName, docQueue->current->docName);
        printing[i].number_pages = docQueue->current->number_pages;
        docQueue->current = docQueue->current->next;
    }
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


void docDistributuion(int printersSlot[], int number_printers, Queue* docQueue, Printing* printing, char** printersName, Stack* printedPaperStack, Stack** printLogs) {
    // find out why it's not printing the first printers name, maybe turn it iont a function parameter
    for(int i = 0; i <= number_printers-1; i++) {
        if(printersSlot[i] == 0) {
            addStack(printedPaperStack, printing[i].docName, printing[i].number_pages);
            addStack(printLogs[i], printing[i].docName, printing[i].number_pages);
            //printf("[%s] %s-%dp\n", printersName[i], printing[i].docName, printing[i].number_pages);
            StackNode* node = printLogs[i]->tail;
            printf("[%s] ", printersName[i]);
            while (node != NULL) {
                if (node->docName != NULL)
                    printf("%s-%dp ", node->docName, node->pagesNumber);
                if (node == printLogs[i]->head) {
                    break;
                }
                node = node->previous;
            }
            printf("\n");

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
    // Subtract lowest                prependName(buffer[i], printing[i].docName);

    return lowest;
}


void subtract_number_array(int printersSlot[], int number_printers, int lowestArrayNumber) {
    for(int i=0; i <= number_printers-1; i++)
        printersSlot[i] = printersSlot[i]-lowestArrayNumber;
}


void last_compare(int printersSlot[], int number_printers, int lowestArrayNumber, Printing *printing, char** printersName, Stack** printLogs) {
    for(int i = 0; i <= number_printers-1; i++) {
        if(printersSlot[i] == 0) {
            addStack(printLogs[i], printing[i].docName, printing[i].number_pages);
            StackNode* node = printLogs[i]->tail;
            printf("[%s] ", printersName[i]);
            while (node != NULL) {
                if (node->docName != NULL)
                    printf("%s-%dp ", node->docName, node->pagesNumber);
                if (node == printLogs[i]->head) {
                    break;
                }
                node = node->previous;
            }
            printf("\n");
            //printf("[%s] %s-%d", printersName[i], printing[i].docName, printing[i].number_pages);
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
    return lowest;
}


void replaceZeros(int printersSlot[], int number_printers) {
    for(int i = 0; i <= number_printers-1; i++) {
        if(printersSlot[i] == 0) 
            printersSlot[i] = 1000000;
    }
}


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
    
    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    Stack printedPapersStack;
    Stack *printedPapersStack_Pointer = &printedPapersStack;
    initialize_Stack(printedPapersStack_Pointer);

    Stack printLog;
    Stack *printLog_pointer = &printLog;
    initialize_Stack(printLog_pointer);
    Stack** printLogs = &printLog_pointer;

    Queue docNameQueue;
    Queue *docNameQueue_Pointer = &docNameQueue;
    initialize_Queue(docNameQueue_Pointer);
    LineFile printers = getFileLines(input);



    char *docName, *numberPagesStr;
    char numberPagesChar;
    int number_Pages;
    int all_number_pages = 0;

    int printedPages = 0;
    int number_printers = printers.linesNumber;
    
    printf("--Numero de impressoras = %d--\n", number_printers);
    for(int i = 0; i < number_printers; i++) {
        printf("Impressora: %s\n", printers.linesArray[i]);
    }

    int printersSlot[number_printers];
    int lowestNumber;
    
    Printing printed_Documents[number_printers];
    LineFile documents = getFileLines(input);
    fclose(input);

    for (int i = 0; i < documents.linesNumber; i++) {
        printf("Linha %d: %s\n", i + 1, documents.linesArray[i]);
        free(documents.linesArray[i]);
    }

    // Alocar memória para todos os elementos de numberPages fora do loop
    documents.numberPages = (int*) malloc(documents.linesNumber * sizeof(int));
    if (documents.numberPages == NULL) {
        fprintf(stderr, "Erro ao alocar memória para documents.numberPages\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < documents.linesNumber; i++) {
        docName = strtok(documents.linesArray[i], " ");
        numberPagesStr = strtok(NULL, "");
    
        // Verificação para garantir que numberPagesStr não seja NULL
        if (numberPagesStr != NULL) {
            number_Pages = atoi(numberPagesStr);  // Converter a string inteira para um número inteiro
            documents.numberPages[i] = number_Pages;
            all_number_pages += number_Pages;
            addQueue(docNameQueue_Pointer, documents.linesArray[i], documents.numberPages[i]);
        }
        else {
            fprintf(stderr, "Erro ao processar a linha %d: formato inválido\n", i);
            documents.numberPages[i] = 0;  // Definir um valor padrão ou tratar o erro conforme necessário
        }
    }

    printf("Tamango do docQueue--%d--\n", queue_size(docNameQueue_Pointer));

    QueueNode *current = docNameQueue_Pointer->head;
    while (current != NULL) {
        printf("Documento: %s\n", current->docName);
        current = current->next;
    }

    docNameQueue_Pointer->current = docNameQueue_Pointer->head;
    initialize_printersSlot(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents);

    while(1) {
        lowestNumber = lowestArrayNumber(printersSlot, number_printers);
        subtract_number_array(printersSlot, number_printers, lowestNumber);
        docDistributuion(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents, printers.linesArray, printedPapersStack_Pointer, printLogs);

        if(docNameQueue_Pointer->current == docNameQueue_Pointer->tail) {
            lowestNumber = lowestArrayNumber(printersSlot, number_printers);
            subtract_number_array(printersSlot, number_printers, lowestNumber);
            docDistributuion(printersSlot, number_printers, docNameQueue_Pointer, printed_Documents, printers.linesArray, printedPapersStack_Pointer, printLogs);

            for(int i = 0; i <= number_printers-2; i++) {
                lowestNumber = lowestArrayNumber(printersSlot, number_printers);
                subtract_number_array(printersSlot, number_printers, lowestNumber);
                last_compare(printersSlot, number_printers, lowestNumber, printed_Documents, printers.linesArray, printLogs);
                replaceZeros(printersSlot, number_printers);
                printf("\n");
            }
           
            break;
        }

    }

    printf("%d-p\n", all_number_pages);
    printedPapersStack_Pointer->current = printedPapersStack_Pointer->tail;

    while(1) {
        if (printedPapersStack_Pointer->current == printedPapersStack_Pointer->head) {
            printf("%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            break;
        }
        else {
            printf("%s-%dp\n", printedPapersStack_Pointer->current->docName, printedPapersStack_Pointer->current->pagesNumber);
            printedPapersStack_Pointer->current = printedPapersStack_Pointer->current->previous;
        }
    }

    fclose(input); 
    fclose(output);

    return 0;
}


// pass the output file ass agument of the last and thedistributuion functions fprint print out in the output and fprint when the printer gets the file
// In the the distribution functions add the printed file to the stack
// print the stack
// numeber of prited files