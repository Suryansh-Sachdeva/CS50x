#include <stdio.h>

int main() {
    int rows;

    // Prompt user for number of rows
    printf("Enter number of rows: ");
    scanf("%d", &rows);
    // Outer loop for each row
    while (rows<1)
    {
        printf("Enter number of rows: ");
        scanf("%d", &rows);
    }
    if (rows>0){
        for(int i = 1; i <= rows; i++) {
            // Print spaces
            for(int j = 1; j <= rows - i; j++) {
                printf(" ");
            }
            // Print '#' characters
            for(int k = 1; k <= i; k++) {
                printf("#");
            }
            // Move to the next line
            printf("\n");
        }
    }
    return 0;
}
