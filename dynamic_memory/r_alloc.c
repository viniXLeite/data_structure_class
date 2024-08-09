#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    char *str1;
    char *str2;

    str1 = (char *) malloc(16);
    strcpy(str1, "019AB");
    str2 = realloc(str1, 8);

    printf("Str1 Value: %p [%s]\n", str1, str1);
    printf("str2 Value: %p [%s]\n", str2, str2);
    free(str2);
    return 0;

}