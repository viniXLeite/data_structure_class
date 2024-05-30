#include <stdio.h>

int main() {
    int num;
    int *pi = &num;
    void* pv = &num; // a pointer to void can point to any type of data

    printf("Value of pi: %p\n", pi);
    printf("Size of pv: %lu bytes\n", sizeof(pi)); // a pointer to void will always have the same memory alignment as a pointer to char (char *) which is 8 bytes
    return 0; // On most modern platforms, the size of a pointer to data is normally the same Pointer Size and Types regardless of the pointer type
}
