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


int regular_binarySearch(long long int *vector, int n, long long int x, int *steps_binary) {
    int i = 0, j = n - 1;
    int p = (i + j) / 2;
    int counter = 0;

    while (j >= i) {
        counter += 1;  // Incrementa steps_binary a cada iteração

        if (vector[p] == x) {
            steps_binary[0] = counter;
            return p;  // Retorna o índice se o elemento for encontrado
        } else if (vector[p] > x) {
            j = p - 1;
        } else {
            i = p + 1;
        }
        p = (i + j) / 2;
    }

    steps_binary[0] = counter;
    return -1;  // Retorna -1 se o elemento não for encontrado
}


int interpolatedBinarySearch(long long int arr[], int n, long long int target, int *steps_interpolated) {
    int low = 0;
    int high = n - 1;
    int counter = 0;
    // store the position ,the counter and the sum of all steps here. Use void func and pass 'result' as arg of this funtion 

    while (low <= high) {
        counter += 1;

        int pos = low + ((arr[high] - arr[low])) % (high - low + 1);

        if (pos < low || pos > high) {
            break;
        }

        if (arr[pos] == target) {
            steps_interpolated[0] = counter;
            return pos;
        }

        if (arr[pos] > target) {
            high = pos - 1;
        } else {
            low = pos + 1;
        }
    }
    
    steps_interpolated[0] = counter;
    return -1;
}

void compare_search_algorithms(FILE* output, long long int *ISBN_array, int number_of_books, long long int *ISBN_to_search, 
    int number_ISBNs_to_search, Book *Books, int* steps_binary, 
    int* steps_interpolated, int* all_steps_binary, 
    int* all_steps_interpolated, int *number_victories_binary, int *number_victories_interpolated) {

    int binary_position;
    int interpolated_position;

    for(int i = 0; i <= number_ISBNs_to_search-1; i++) {
        binary_position = regular_binarySearch(ISBN_array, number_of_books, ISBN_to_search[i], steps_binary);
        all_steps_binary[0] += steps_binary[0];
        printf("\n\nRegular Binary search, position: %d, steps: %d", binary_position, steps_binary[0]);

        interpolated_position = interpolatedBinarySearch(ISBN_array, number_of_books, ISBN_to_search[i], steps_interpolated);
        all_steps_interpolated[0] += steps_interpolated[0];
        printf("\nInterpolated Binary search, position: %d, steps: %d\n", interpolated_position, steps_interpolated[0]);

        if(steps_interpolated[0] <= steps_binary[0])
            number_victories_interpolated[0] += 1;

        else
            number_victories_binary[0] += 1; 

        if(interpolated_position != -1) {
            fprintf(output, "[%lld]B=%d,I=%d:Author:%s,Title:%s\n", ISBN_to_search[i], steps_binary[0], steps_interpolated[0], Books[binary_position].author, Books[binary_position].title);
            printf("[%lld]B=%d,I=%d:Author:%s,Title:%s\n", ISBN_to_search[i], steps_binary[0], steps_interpolated[0], Books[binary_position].author, Books[binary_position].title);
        }
        else {
            fprintf(output, "[%lld]B=%d,I=%d:ISBN_NOT_FOUND\n", ISBN_to_search[i], steps_binary[0], steps_interpolated[0]);
            printf("[%lld]B=%d,I=%d:ISBN_NOT_FOUND\n", ISBN_to_search[i], steps_binary[0], steps_interpolated[0]);
        }
    }

}

int main(int argc, char* argv[]) {
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");

    printf("--Read lines--\n");
    FileInfo BooksInfo = read_lines_based_on_number(input, output);

    printf("\n---ISBNs to search---\n");
    FileInfo File_ISBN_to_search = read_lines_based_on_number(input, output);

    // Converts the ISBNs to search from strings to integers
    printf("\n---ISBNs to search(all int)---\n");
    int number_ISBNs_to_search = File_ISBN_to_search.number_of_lines;
    long long int ISBNs_to_search[number_ISBNs_to_search];
    convert_ISBN_to_ll_int(File_ISBN_to_search.array_of_lines, number_ISBNs_to_search, ISBNs_to_search);
    //show_array(ISBNs_to_search, number_ISBNs_to_search);

    // Creates an array of Book and then stores BooksInfo informations on it
    int number_of_books = BooksInfo.number_of_lines;
    printf("\n\nnumber of books: %d\n", number_of_books);
    Book Books[number_of_books];
    initialize_Books(Books, BooksInfo, number_of_books);

    // Creates an array of ISBN and stores Books[i].ISBNs on it
    long long int ISBN_array[number_of_books];
    distribute_ISBNs(ISBN_array, Books, number_of_books);
    printf("--Distributed ISBNs--\n");    
    //show_array(ISBN_array, number_of_books);

    int steps_binary[1];
    int steps_interpolated[1];

    //Remenber to implement the trucated average of steps for each search algorithm
    int all_steps_binary[1];
    all_steps_binary[0] = 0;

    int all_steps_interpolated[1];
    all_steps_binary[0] = 0;

    int number_victories_binary[1];
    number_victories_binary[0] = 0;

    int number_victories_interpolated[1];
    number_victories_interpolated[0] = 0; // pass it as argument of compare_search

    compare_search_algorithms(output, ISBN_array, number_of_books, ISBNs_to_search, 
    number_ISBNs_to_search, Books, steps_binary, steps_interpolated, 
    all_steps_binary, all_steps_interpolated, number_victories_binary, number_victories_interpolated);

    int average_steps_bynary = trunc(all_steps_binary[0]/number_ISBNs_to_search);
    int average_steps_interpolated = trunc(all_steps_interpolated[0]/number_ISBNs_to_search);

    fprintf(output, "BINARY=%d:%d\n", number_victories_binary[0], average_steps_bynary);
    fprintf(output, "INTERPOLATION=%d:%d", number_victories_interpolated[0], average_steps_interpolated);

    fclose(input); 
    fclose(output);
}