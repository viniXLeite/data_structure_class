#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Remenber to use atoi from ctype and trunc(x) from math.h

typedef struct _FileInfo {
    int number_of_lines;
    char** array_of_lines;

} FileInfo;

typedef struct _book { 
    long long int ISBN;
    char author[51];
    char title[101];
} Book;

int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

FileInfo read_lines_based_on_number(FILE* input, FILE* output) {
    FileInfo FileInfo;

    char line[170];
    if (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = '\0';
        fprintf(output,"%s\n", line);
        if (is_number(line)) {
            int count = atoi(line);
            FileInfo.number_of_lines = count;
            FileInfo.array_of_lines = (char**) malloc(FileInfo.number_of_lines*51);

            for (int i = 0; i < count; i++) {
                if (fgets(line, sizeof(line), input)) {
                    // Remove o caractere de nova linha, se presente
                    line[strcspn(line, "\n")] = '\0';
                    FileInfo.array_of_lines[i] = (char*) malloc(strlen(line)+1);
                    strcpy(FileInfo.array_of_lines[i], line);
                    printf("%s\n", FileInfo.array_of_lines[i]);
                }
            }
        } else {
            printf("The first line of the file is not a valid number.\n");
        }
    }

    return FileInfo;
}

void initialize_Books(FileInfo BooksInfo, int number_of_books) {
    Book books[number_of_books];
    char temp_author_title[150];
    char temp_IBSN[15];

    printf("\n--initialize Books[i]--\n");
    for(int i = 0; i <= number_of_books-1; i++) {
        sscanf(BooksInfo.array_of_lines[i], "%s %[^\n]", temp_IBSN, temp_author_title);

        // Find the position of '&'
        char *posicao_e_comercial = strchr(temp_author_title, '&');
        if (posicao_e_comercial != NULL) {
            // Copy the title from the position right after '&'
            strcpy(books[i].title, posicao_e_comercial + 1);

            // Ends the author on &'
            strncpy(books[i].author, temp_author_title, posicao_e_comercial - temp_author_title);
            books[i].author[posicao_e_comercial - temp_author_title] = '\0';
        }

        books[i].ISBN = strtoll(temp_IBSN, NULL, 10);
        printf("ISBN: %lld, Author: %s, Title: %s;\n", books[i].ISBN, books[i].author, books[i].title);

    }
}

// change the position of elements
void swap(long long int* a, long long int* b) {
    long long int temp = *a;
    *a = *b;
    *b = temp;
}

// Patition the array
int partition(long long int arr[], int low, int high) {
    long long int pivot = arr[high]; // Pivô
    int i = (low - 1); // Índice do menor elemento

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++; 
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(long long int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    printf("--Books--\n");
    FileInfo BooksInfo = read_lines_based_on_number(input, output);

    printf("\n---ISBNs---\n");
    FileInfo IBNs = read_lines_based_on_number(input, output);

    // Creates an array of Book and then store BooksInfo informations on it
    int number_of_books = BooksInfo.number_of_lines;
    Book Books[number_of_books];
    initialize_Books(BooksInfo, number_of_books);



    fclose(input); 
    fclose(output);
}