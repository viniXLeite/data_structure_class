#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _Printers {
    int linesNumber;
    char** linesArray;

} Printers;

typedef struct _Document {
    int linesNumber;
    char** linesArray;
    int *number_document;

} Document;


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

//int get_docPages()


int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    Printers printers = getPrinters(input);
    printf("%d\n", printers.linesNumber);
    for(int i = 0; i <= printers.linesNumber-1; i++) {
        printf("%s\n", printers.linesArray[i]);
    }

    printf("\n");

    Document documents = getDocuments(input);
    printf("\n%d\n", documents.linesNumber);
    for(int i = 0; i <= documents.linesNumber-1; i++) {
        printf("%s %d\n", documents.linesArray[i], documents.number_document[i]);
    }

    // to get the number of pages on a document use documents.linesArray[n][size(documents.linesArray[n])] 
    //and after that convert it integer

    fclose(input); 
    fclose(output);
}