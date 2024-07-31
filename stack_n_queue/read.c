#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _Printers {
    int linesNumber;
    char** linesArray;
    int *number_document;

} Printers;

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

//int get_docPages()


int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    Printers printers = getPrinters(input);
    fprintf(output, "%d\n", printers.linesNumber);
    for(int i = 0; i <= printers.linesNumber-1; i++) {
        fprintf(output, "%s\n", printers.linesArray[i]);
    }

    printf("\n");

    Printers documents = getPrinters(input);
    fprintf(output, "\n%d\n", documents.linesNumber);
    for(int i = 0; i <= documents.linesNumber-1; i++) {
        fprintf(output, "%s\n", documents.linesArray[i]);
    }

    // to get the number of pages on a document use documents.linesArray[n][size(documents.linesArray[n])] 
    //and after that convert it integer

    fclose(input); 
    fclose(output);
}