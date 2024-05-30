#include <stdio.h>

//If a pointer is declared as global or static, it is initialized to NULL when the program starts.

int main() {
    int number = 10;
    int* pi = NULL; // pi does not point to any memory address, we could either write "pi = 0;" or "int *pi = 0;" 
                    // *pi = 0; this one would refer to the 0 integer
    int ** pi_2 = &pi; // pi is storaged in some part of the memory, although it does not point to any value 
    if (pi == NULL) 
        printf("pi has a NULL value\n");
    else 
        printf("pi is a regular pointer\n");

    printf("\nThe adreess of pi: %p, its value is: %p\n", &pi, pi);
    printf("\nThe adreess of pi 2: %p, its value is: %p, the value of the pointer pointed by pi 2: %p\n", &pi_2, pi_2, *pi_2);
    return 0;
}

