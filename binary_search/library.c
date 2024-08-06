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
        if (is_number(line)) {
            int count = atoi(line);
            FileInfo.number_of_lines = count;
            FileInfo.array_of_lines = (char**) malloc(FileInfo.number_of_lines*51);

            for (int i = 0; i < count; i++) {
                if (fgets(line, sizeof(line), input)) {
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

void convert_ISBN_to_ll_int(char** array, int number_of_strings, long long int *ISBN) {
    for(int i = 0; i <= number_of_strings-1; i++) {
        ISBN[i] = strtoll(array[i], NULL, 10);
    }
}

void initialize_Books(Book *books, FileInfo BooksInfo, int number_of_books) {
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

void distribute_ISBNs(long long int* ISBN_array, Book *Books, int number_of_books) {
    printf("\n");
    for(int i = 0; i <= number_of_books-1; i++) {
        ISBN_array[i] = Books[i].ISBN;
    }
}


// change the position of elements
void swap(long long int* a, long long int* b) {
    long long int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(long long int arr[], int n, int i) {
    int largest = i; // Inicializa o maior como raiz
    int left = 2 * i + 1; // Índice do filho esquerdo
    int right = 2 * i + 2; // Índice do filho direito

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(&arr[i], &arr[largest]);

        heapify(arr, n, largest);
    }
}

void heapSort(long long int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i >= 0; i--) {
        swap(&arr[0], &arr[i]);

        heapify(arr, i, 0);
    }
}

void show_array(long long int* array, int j) {
    for(int i = 0; i <= j-1; i++) {
        if(i == 0) {
            printf("[%lld, ", array[i]);
        }
        else if(i == j-1) {
            printf("%lld]", array[i]);
        }
        else {
            printf("%lld, ", array[i]);
        }
    }
}



int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    printf("--Read lines--\n");
    FileInfo BooksInfo = read_lines_based_on_number(input, output);

    // Remember to convert it to long long int
    printf("\n---ISBNs to search---\n");
    FileInfo File_ISBN_to_search = read_lines_based_on_number(input, output);

    printf("\n---ISBNs to search(all int)---\n");
    long long int ISBNs_to_search[File_ISBN_to_search.number_of_lines];
    convert_ISBN_to_ll_int(File_ISBN_to_search.array_of_lines, File_ISBN_to_search.number_of_lines, ISBNs_to_search);
    show_array(ISBNs_to_search, File_ISBN_to_search.number_of_lines);

    // Creates an array of Book and then stores BooksInfo informations on it
    int number_of_books = BooksInfo.number_of_lines;
    printf("\n\nnumber of books: %d\n", number_of_books);
    Book Books[number_of_books];
    initialize_Books(Books, BooksInfo, number_of_books);

    // Creates an array of ISBN and stores Books[i].ISBNs on it
    long long int ISBN_array[number_of_books];
    distribute_ISBNs(ISBN_array, Books, number_of_books);
    printf("--Distributed ISBNs--\n");    
    show_array(ISBN_array, number_of_books);

    // Reorganizes the ISBN_array if it's not sorted with the heapsort algorithm
    heapSort(ISBN_array, number_of_books);
    printf("\n\n--heapsorted--\n");
    show_array(ISBN_array, number_of_books);



    fclose(input); 
    fclose(output);
}