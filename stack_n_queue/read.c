#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _fileLine {
    int linesNumber;
    char** linesArray;

} FileLine;


FileLine getFileLines(FILE* input) {
    char commandLine[10];
    char nameLine[51];
    FileLine fileLine;

    if(fgets(commandLine, sizeof(commandLine), input)) {
        char charCommandsNumber = commandLine[0];
        int commandsNumber = charCommandsNumber - '0';

        fileLine.linesNumber = commandsNumber;
        fileLine.linesArray = malloc(commandsNumber*sizeof(char*)+1);

        for(int i = 0; i < commandsNumber; i++) {
            if (fgets(nameLine, sizeof(nameLine), input)) {
                fileLine.linesArray[i] = malloc(sizeof(nameLine)+1);

                if(nameLine[strlen(nameLine)-1] == '\n') nameLine[strlen(nameLine)-1] = '\0';
                strcpy(fileLine.linesArray[i], nameLine);
            }
        }
    }

    return fileLine;
}

//int get_docPages()


int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    FileLine printers = getFileLines(input);
    printf("%d\n", printers.linesNumber);
    for(int i = 0; i <= printers.linesNumber-1; i++) {
        printf("%s\n", printers.linesArray[i]);
    }

    printf("\n");

    FileLine documents = getFileLines(input);
    printf("%d\n", documents.linesNumber);
    for(int i = 0; i <= documents.linesNumber-1; i++) {
        printf("%s\n", documents.linesArray[i]);
    }

    // to get the number of pages on a document use documents.linesArray[n][size(documents.linesArray[n])] 
    //and after that convert it integer

    fclose(input); 
    fclose(output);
}