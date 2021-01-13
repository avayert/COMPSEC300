/*
 * T3_heap_overflow
 *
 * This example is identical to the example in T1, except the memory in question
 * is heap memory instead of stack memory. The principle is the exact same.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 24

const char* secret_word = "abracadabra";


int main() {
    char* input_buffer = malloc(BUFFER_SIZE);

    printf("What is the secret word: ");
    scanf("%s", input_buffer);

    if (strcmp(input_buffer, secret_word) == 0)
        puts("You know the secret phrase!");
    else
        puts("That word is incorrect. Please try again later.");

    return 0;
}
