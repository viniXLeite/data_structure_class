#include <stdio.h>

int main() {
    int vector[] = {12, 34 , 6};
    int *pi = vector; // when a pointer points to an array it will firstly be the initial value of this data structure 
    printf("pi adress: %p\npi value: %p\nvalue of the element pointed by pi %d\n\n", &pi, pi, *pi); 
    pi += 1;  // The amount added to pi will be 1 times sizeof(int) (which is the size of the array's type)
             // In this integer's array case, pi's new value shall be pi + 4
            // Pi points to the second array's element 
    printf("pi adress: %p\npi value: %p\nvalue of the element pointed by pi %d\n\n", &pi, pi, *pi);
    pi += 1; // Pi points to the third array's element
    printf("pi adress: %p\npi value: %p\nvalue of the element pointed by pi %d\n\n", &pi, pi, *pi);

    pi -= 1; // Subtracting an integer from a pointer follows the same logic
    printf("pi adress: %p\npi value: %p\nvalue of the element pointed by pi %d\n\n", &pi, pi, *pi);
    pi -= 1;
    printf("pi adress: %p\npi value: %p\nvalue of the element pointed by pi %d\n\n", &pi, pi, *pi);

    *pi = 15; // Adresses 15 to the pointed element
    printf("vector[0] = %d\n", *(vector+0)); // *(vector+2) is equivalent to write vector[2]

    return 0;
}