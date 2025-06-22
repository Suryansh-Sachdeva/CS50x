#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Scrabble point values for each letter A-Z
const int POINTS[] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8,   // A-J
    5, 1, 3, 1, 1, 3, 10, 1, 1, 1,  // K-T
    1, 4, 4, 8, 4, 10               // U-Z
};

// Function to compute score for a given word
int compute_score(const char word[]) {
    int score = 0;

    for (int i = 0; word[i] != '\0'; i++) {
        if (isalpha(word[i])) {
            // Convert letter to uppercase and calculate its index
            int index = toupper(word[i]) - 'A';
            score += POINTS[index];
        }
    }

    return score;
}

int main() {
    char word1[100];
    char word2[100];

    // Get input from both players
    printf("Player 1: ");
    scanf("%99s", word1);  // Avoids buffer overflow

    printf("Player 2: ");
    scanf("%99s", word2);

    // Calculate scores
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Display results
    printf("\n--- Results ---\n");
    printf("Player 1 Score: %d\n", score1);
    printf("Player 2 Score: %d\n", score2);

    // Determine winner
    if (score1 > score2) {
        printf("🎉 Player 1 wins!\n");
    } else if (score2 > score1) {
        printf("🎉 Player 2 wins!\n");
    } else {
        printf("🤝 It's a tie!\n");
    }

    return 0;
}
