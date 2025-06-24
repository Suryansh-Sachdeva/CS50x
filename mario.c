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
