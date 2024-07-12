#include <stdio.h>

int main() {
    int vector[] = {12, 34 , 6};
    int *pi = vector; // when a pointer to an array is declared it will firstly point the initial value of this data structure 
    printf("pi adress: %p\npi value: %p\nvalue of the element pointed by pi %d\n\n", &pi, pi, *pi); 
    pi += 1;  // The amount added to pi will be 1 times sizeof(int) (which is the size of the elements of the array)
             // In this integer's array case, pi's new value shall be pi + 4 bytes
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

// When one pointer is subtracted from another the result is the difference between their adresses, as follows the example:

/*
int vector[] = {28, 41, 7};
int *p0 = vector;
int *p1 = vector+1;
int *p2 = vector+2;
printf("p2-p0: %d\n",p2-p0); p2-p0: 2
printf("p2-p1: %d\n",p2-p1); p2-p1: 1
printf("p0-p1: %d\n",p0-p1); p0-p1: -1
*/
