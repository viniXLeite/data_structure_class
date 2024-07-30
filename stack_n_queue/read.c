#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _Document {
    int linesNumber;
    char** linesArray;

} Document;


Document getDocuments(FILE* input) {
    char commandLine[10];
    char nameLine[51];
    Document Document;

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
            }
        }
    }

    return Document;
}

//int get_docPages()


int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    Document printers = getDocuments(input);
    printf("%d\n", printers.linesNumber);
    for(int i = 0; i <= printers.linesNumber-1; i++) {
        printf("%s\n", printers.linesArray[i]);
    }

    printf("\n");

    Document documents = getDocuments(input);
    printf("%d\n", documents.linesNumber);
    for(int i = 0; i <= documents.linesNumber-1; i++) {
        printf("%s\n", documents.linesArray[i]);
    }

    // to get the number of pages on a document use documents.linesArray[n][size(documents.linesArray[n])] 
    //and after that convert it integer

    fclose(input); 
    fclose(output);
}