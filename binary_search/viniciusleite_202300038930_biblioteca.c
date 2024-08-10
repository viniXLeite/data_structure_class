#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

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

FileInfo read_lines_based_on_number(FILE* input) {
    FileInfo fileInfo;
    char line[170];

    if (fgets(line, sizeof(line), input)) {
        line[strcspn(line, "\n")] = '\0';
        if (is_number(line)) {
            int count = atoi(line);
            fileInfo.number_of_lines = count;
            fileInfo.array_of_lines = (char**) malloc(count*170);
            
            for (int i = 0; i < count; i++) {
                if (fgets(line, sizeof(line), input)) {
                    line[strcspn(line, "\n")] = '\0';
                    fileInfo.array_of_lines[i] = (char*) malloc(strlen(line)+1);
                    strcpy(fileInfo.array_of_lines[i], line);
                }
            }
        } else {
            printf("The first line of the file is not a valid number.\n");
        }
    }

    return fileInfo;
}

void convert_ISBN_to_ll_int(char** array, int number_of_strings, long long int *ISBN) {
    for(int i = 0; i < number_of_strings; i++) {
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
        //printf("ISBN: %lld, Author: %s, Title: %s;\n", books[i].ISBN, books[i].author, books[i].title);

    }

}

void distribute_ISBNs(long long int* ISBN_array, Book *books, int number_of_books) {
    for(int i = 0; i < number_of_books; i++) {
        ISBN_array[i] = books[i].ISBN;
    }
}

void show_array(long long int* array, int j) {
    printf("[");
    for(int i = 0; i < j; i++) {
        if(i > 0) printf(", ");
        printf("%lld", array[i]);
    }
    printf("]\n");
}

int regular_binarySearch(long long int *vector, int n, long long int x, int *steps_binary) {
    int i = 0, j = n - 1, counter = 0;
    while (i <= j) {
        counter++;
        int mid = (i + j) / 2;
        if (vector[mid] == x) {
            *steps_binary = counter;
            return mid;
        } else if (vector[mid] < x) {
            i = mid + 1;
        } else {
            j = mid - 1;
        }
    }
    *steps_binary = counter+1;
    return -1;
}


int interpolated_search(long long int arr[], int n, long long int target, int *steps_interpolated) {
    int i = 0;         
    int j = n - 1;     
    
    *steps_interpolated = 0; 

    while (i <= j /*&& target >= arr[i] && target <= arr[j]*/) {
        if (i == j) {
            (*steps_interpolated)++; // added

            if (arr[i] == target) {
                return i;
            }
            return -1;
        }
        
        int pos = i + ((arr[j] - arr[i]) % (j - i + 1));

        (*steps_interpolated)++;

        if (arr[pos] == target)
            return pos;
        
        if (arr[pos] < target)
            i = pos + 1;
        else
            j = pos - 1;
    }
    return -1; 
}


void compare_search_algorithms(FILE* output, long long int *ISBN_array, int number_of_books, long long int *ISBN_to_search, 
    int number_ISBNs_to_search, Book *Books, int* all_steps_binary, int* all_steps_interpolated, 
    int *number_victories_binary, int *number_victories_interpolated) {

    int steps_binary, steps_interpolated;
    int binary_position, interpolated_position;

    for(int i = 0; i < number_ISBNs_to_search; i++) {
        binary_position = regular_binarySearch(ISBN_array, number_of_books, ISBN_to_search[i], &steps_binary);
        all_steps_binary[0] += steps_binary;

        interpolated_position = interpolated_search(ISBN_array, number_of_books, ISBN_to_search[i], &steps_interpolated);
        all_steps_interpolated[0] += steps_interpolated;

        if (steps_interpolated <= steps_binary) {
            number_victories_interpolated[0]++;
        } else {
            number_victories_binary[0]++;
        }

        if (interpolated_position != -1) {
            fprintf(output, "[%lld]B=%d,I=%d:Author:%s,Title:%s\n", ISBN_to_search[i], steps_binary, steps_interpolated, Books[binary_position].author, Books[binary_position].title);
        } else {
            fprintf(output, "[%lld]B=%d,I=%d:ISBN_NOT_FOUND\n", ISBN_to_search[i], steps_binary, steps_interpolated);
        }
    }
}

int main(int argc, char* argv[]) {

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    FileInfo BooksInfo = read_lines_based_on_number(input);
    FileInfo File_ISBN_to_search = read_lines_based_on_number(input);
    printf("Ok read_lines_based_on_number\n");

    int number_ISBNs_to_search = File_ISBN_to_search.number_of_lines;
    long long int *ISBNs_to_search = malloc(number_ISBNs_to_search * sizeof(long long int));
    convert_ISBN_to_ll_int(File_ISBN_to_search.array_of_lines, number_ISBNs_to_search, ISBNs_to_search);
    printf("Ok convert_ISBN_to_ll_int\n");

    int number_of_books = BooksInfo.number_of_lines;
    Book *Books = malloc(number_of_books * sizeof(Book));
    initialize_Books(Books, BooksInfo, number_of_books);
    printf("Ok initialize_Books\n");

    long long int *ISBN_array = malloc(number_of_books * sizeof(long long int));
    distribute_ISBNs(ISBN_array, Books, number_of_books);
    printf("Ok distribute_ISBNs\n");

    int all_steps_binary = 0;
    int all_steps_interpolated = 0;
    int number_victories_binary = 0;
    int number_victories_interpolated = 0;

    compare_search_algorithms(output, ISBN_array, number_of_books, ISBNs_to_search, number_ISBNs_to_search, 
        Books, &all_steps_binary, &all_steps_interpolated, &number_victories_binary, &number_victories_interpolated);

    int average_steps_binary = all_steps_binary / number_ISBNs_to_search;
    int average_steps_interpolated = all_steps_interpolated / number_ISBNs_to_search;
    printf("Ok compare_search_algorithms\n");

    fprintf(output, "BINARY=%d:%d\n", number_victories_binary, average_steps_binary);
    fprintf(output, "INTERPOLATION=%d:%d\n", number_victories_interpolated, average_steps_interpolated);

    // Free allocated memory
    for (int i = 0; i < BooksInfo.number_of_lines; i++) {
        free(BooksInfo.array_of_lines[i]);
    }
    free(BooksInfo.array_of_lines);

    for (int i = 0; i < File_ISBN_to_search.number_of_lines; i++) {
        free(File_ISBN_to_search.array_of_lines[i]);
    }
    free(File_ISBN_to_search.array_of_lines);
    free(ISBNs_to_search);
    free(Books);
    free(ISBN_array);

    fclose(input);
    fclose(output);
    return 0;
}
