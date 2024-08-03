#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        int commandsNumber = atoi(commandLine);

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

int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

Printers read_lines_based_on_number(FILE* input, FILE* output) {
    Printers Printers;

    if (input == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char line[100];
    if (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = '\0';
        fprintf(output,"%s\n", line);
        if (is_number(line)) {
            int count = atoi(line);
            Printers.linesNumber = count;
            Printers.linesArray = (char**) malloc(Printers.linesNumber*51);

            for (int i = 0; i < count; i++) {
                if (fgets(line, sizeof(line), input)) {
                    // Remove o caractere de nova linha, se presente
                    line[strcspn(line, "\n")] = '\0';
                    Printers.linesArray[i] = (char*) malloc(strlen(line)+1);
                    strcpy(Printers.linesArray[i], line);
                    fprintf(output,"%s\n", Printers.linesArray[i]);
                }
            }
        } else {
            printf("A primeira linha do arquivo não é um número válido.\n");
        }
    }

    return Printers;

}


int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    Printers printers = read_lines_based_on_number(input, output);
    Printers Documents = read_lines_based_on_number(input, output);
    //fprintf(output, "%d\n", printers.linesNumber);
    //for(int i = 0; i <= printers.linesNumber-1; i++) {
      //  fprintf(output, "%s\n", printers.linesArray[i]);
    //}

    //printf("\n");

    //Printers documents = getPrinters(input);
    //fprintf(output, "\n%d\n", documents.linesNumber);
    //for(int i = 0; i <= documents.linesNumber-1; i++) {
        //fprintf(output, "%s\n", documents.linesArray[i]);
    //}

    // to get the number of pages on a document use documents.linesArray[n][size(documents.linesArray[n])] 
    //and after that convert it integer

    fclose(input); 
    fclose(output);
}