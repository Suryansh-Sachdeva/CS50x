#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Function to encrypt a character
char rotate(char c, int key) {
    if (isupper(c)) {
        return ((c - 'A' + key) % 26) + 'A';
    } else if (islower(c)) {
        return ((c - 'a' + key) % 26) + 'a';
    } else {
        return c;
    }
}

int main(int argc, char *argv[]) {
    // Check for correct number of command-line arguments
    if (argc != 2) {
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    // Convert key to integer
    int key = atoi(argv[1]);

    // Check if key is a valid non-negative integer
    if (key < 0) {
        printf("Key must be a non-negative integer.\n");
        return 1;
    }

    // Get plaintext from user
    char plaintext[1000];
    printf("Plaintext: ");
    fgets(plaintext, sizeof(plaintext), stdin);

    // Encrypt and display ciphertext
    printf("Ciphertext: ");
    for (int i = 0; plaintext[i] != '\0'; i++) {
        printf("%c", rotate(plaintext[i], key));
    }

    return 0;
}
