#include <stdio.h>

int main() {
    int num = 7;
    int *pi = &num; // this * declares the pointer and then initializes it
    *pi = 200; // However this statement assigns 200 to the variable pointed to by pi, changing the variable initial value
    printf("The address of num: %p and its values is: %d\n", &num, num);
    printf("The address of pi is %p and its value is: %p\nand the value storade by the pointed variable is: %d\n", &pi, pi, *pi); // the * dereferences the pointer
    return 0;
}