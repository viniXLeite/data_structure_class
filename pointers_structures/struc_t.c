#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person {
    char *firstName;
    char *lastName;
    char *title;
    int age;
} Person;

void initializePerson(Person *person, const char* fn, 
        const char* ln, const char* title, unsigned int age) {
    person ->firstName = (char*) malloc(strlen(fn)+1);
    strcpy(person->firstName, fn);
    person ->lastName = (char*) malloc(strlen(ln)+1);
    strcpy(person->lastName, ln);
    person ->title = (char*) malloc(strlen(title)+1);
    strcpy(person->title, title);
    person ->age = age;
}

void deallocatePerson(Person *person) {
    free(person->firstName);
    free(person->lastName);
    free(person->title);
}

void processPerson() {
    Person person;
    initializePerson(&person, "peter", "underwood", "Manager", 36);
    printf("Employee data:\nFirst name: %s\nLast Name: %s\nTitle: %s\nAge: %d\n", person.firstName, person.lastName, person.title, person.age);
    deallocatePerson(&person);
}

int main() {
    printf("Employee's data base\n");
    processPerson();
    return 0;
}
