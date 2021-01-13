/*
 * T2_off_by_one_overflow
 *
 * This program has a off by one overflow due to a mistake in using <= instead of <
 * which leads to writing to an unintended memory location.
 *
 * In this program the result is not catastrophic, but in real code the input array
 * would most likely contain pointers, allowing malicious payloads to be executed in
 * unintended ways.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define BUFFER_SIZE 24

const char* secret = "sdvvzrug";


void caesar_cipher(char* buffer) {
    size_t size = strlen(buffer);

    for (size_t index = 0; index <= size; index++) {
        if (!isalpha(buffer[index]))
            continue;

        char c = tolower(buffer[index]) - 'a';
        buffer[index] = ((c + 3) % 26) + 'a';
    }
}


int main() {
    printf("The Riddler says: \"%s\" \n", secret);

    char* input_buffer[BUFFER_SIZE];
    fgets(input_buffer, BUFFER_SIZE, stdin);
    caesar_cipher(input_buffer);

    if (strcmp(input_buffer, secret) == 0)
        puts("Good job! You got it right!");
    else
        printf("Incorrect! ..but The Riddler says: \"%s\"\n", input_buffer);

    return 0;
}
