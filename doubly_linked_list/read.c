#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	FILE* input = fopen("entrada", "r");
	FILE* output = fopen("saida", "w");


    // Store the content of the file
    char commandLine[70];
    char *command, *name;
    // If the file exist
    if(input != NULL) {
        while(fgets(commandLine, 70, input)) {
            command = strtok(commandLine, " ");
            name = strtok(NULL, "");
            if (commandLine[0] == 'A') {
                printf("[%s] %s", command, name);
                fprintf(output, "[%s] %s", command, name);
            }
            else if (commandLine[0] == 'S') {
                printf("[%s] %s", command, name);
                fprintf(output, "[%s] %s", command, name);
            }
            else {
                printf("[%s] %s", command, name);
                fprintf(output, "[%s] %s", command, name);
            }
        }
    }

    // Close the file
    fclose(input); 
    fclose(output);
}