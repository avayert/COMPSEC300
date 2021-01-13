/*
 * T1_unlimited_buffer_overflow
 *
 * This vulnerability relies on the fact the program reads user input
 * and uses an unbounded write to write it to an array. This means the program
 * can override practically any memory address on the stack with a specifically
 * crafted payload, such as in the T4_function_pointer_oveflow program.
 */

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 24

const char* secret_word = "abracadabra";


int main() {
    char input_buffer[BUFFER_SIZE];

    printf("What is the secret word: ");
    scanf("%s", input_buffer);

    if (strcmp(input_buffer, secret_word) == 0)
        puts("You know the secret phrase!");
    else
        puts("That word is incorrect. Please try again later.");

    return 0;
}
