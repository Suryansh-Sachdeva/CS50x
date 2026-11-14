#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Scrabble point values
int POINTS[] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8,
    5, 1, 3, 1, 1, 3, 10, 1, 1, 1,
    1, 4, 4, 8, 4, 10
};

// Function to compute score
int compute_score(char word[]) {
    int score = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        if (isalpha(word[i])) {
            int index = toupper(word[i]) - 'A';  // Convert to index 0–25
            score += POINTS[index];
        }
    }
    return score;
}

int main() {
    char word1[100], word2[100];

    printf("Player 1: ");
    scanf("%s", word1);

    printf("Player 2: ");
    scanf("%s", word2);

    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    if (score1 > score2) {
        printf("Player 1 wins! (%d vs %d)\n", score1, score2);
    } else if (score2 > score1) {
        printf("Player 2 wins! (%d vs %d)\n", score2, score1);
    } else {
        printf("It's a tie! (%d each)\n", score1);
    }

    return 0;
}
