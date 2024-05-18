#include <stdio.h>


int main() {
    int num = 7;
    int *pi = &num; // this * declares the pointer and &num initializes it
    *pi = 200; // However this statement assigns 200 to the variable pointed to by pi, changing its initial value
    int **pi_2 = &pi;

    printf("The address of num: %p and its values is: %d\n\n", &num, num);
    printf("The address of pi is %p and its value is: %p\nand the value storade by the pointed variable is: %d\n\n", &pi, pi, *pi); // the * dereferences the pointer
    printf("The address of pi_2 is %p and its value is: %p\nand the value storade by the pointed pointer is: %p\n", &pi_2, pi_2, *pi_2);
    return 0;
}
