#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main() {
    char paragraph[1000];

    // Input paragraph
    printf("Enter a paragraph:\n");
    fgets(paragraph, sizeof(paragraph), stdin);

    int letters = 0;
    int words = 0;
    int sentences = 0;

    for (int i = 0; paragraph[i] != '\0'; i++) {
        if (isalpha(paragraph[i])) {
            letters++;
        } else if (paragraph[i] == ' ') {
            words++;
        } else if (paragraph[i] == '.' || paragraph[i] == '!' || paragraph[i] == '?') {
            sentences++;
        }
    }

    // Account for last word (not followed by space)
    words++;

    // Calculate L and S
    float L = (float) letters / words * 100;
    float S = (float) sentences / words * 100;

    // Coleman-Liau index
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int grade = round(index);

    printf("\n--- Readability Report ---\n");
    printf("Letters: %d\n", letters);
    printf("Words: %d\n", words);
    printf("Sentences: %d\n", sentences);
    printf("Grade Level: ");

    if (grade < 1) {
        printf("Before Grade 1\n");
    } else if (grade >= 16) {
        printf("Grade 16+\n");
    } else {
        printf("Grade %d\n", grade);
    }

    return 0;
}
