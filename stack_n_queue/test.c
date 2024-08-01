#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 1024// 51*number_docs

void prependName(char *dest, const char *name) {
    char temp[MAX_BUFFER_SIZE];

    // Copia o conteúdo atual de dest para temp
    strncpy(temp, dest, MAX_BUFFER_SIZE - 1);
    temp[MAX_BUFFER_SIZE - 1] = '\0'; // Certifica-se de que a string está terminada

    // Formata a nova string com o nome à esquerda
    sprintf(dest, "%s %s", name, temp);
}

// Add -np at the end of each name
// varAux = (char*) malloc(sizeof(name)+15);
// pass varAux in prependName

int main() {
    char buffer[MAX_BUFFER_SIZE] = ""; // Inicializa o buffer vazio

    // Nomes para serem adicionados
    const char *names[] = {"Alice-2p", "Bob-3p", "Charlie", "calr", "ewfef", "ewffwef"};
    size_t numNames = sizeof(names) / sizeof(names[0]);

    // Adiciona os nomes no buffer em um laço
    for (size_t i = 0; i < numNames; i++) {
        prependName(buffer, names[i]);
        printf("%s\n", buffer);
    }

    // Imprime o resultado final

    return 0;
}
