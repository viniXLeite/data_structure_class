#include <stdio.h>

int main() {
    int num;
    int *pi = &num;
    void* pv = &num; // a pointer to void can pointer to any type of data

    printf("Value of pi: %p\n", pi);
    printf("Size of pv: %lu\n", sizeof(void *)); // a pointer to void will always have the same alignment as a pointer to char
    return 0;
}