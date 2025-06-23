#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;

const int GENERATIONS = 3;

// Function prototypes
person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    // Allocate memory for new person
    person *new_person = malloc(sizeof(person));
    if (new_person == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    // Base case: no parents (oldest generation)
    if (generations > 1)
    {
        // Recursively create parents
        new_person->parents[0] = create_family(generations - 1);
        new_person->parents[1] = create_family(generations - 1);

        // Randomly assign alleles from parents
        new_person->alleles[0] = new_person->parents[0]->alleles[rand() % 2];
        new_person->alleles[1] = new_person->parents[1]->alleles[rand() % 2];
    }
    else
    {
        // Generation 0: no parents
        new_person->parents[0] = NULL;
        new_person->parents[1] = NULL;

        // Random alleles
        new_person->alleles[0] = random_allele();
        new_person->alleles[1] = random_allele();
    }

    return new_person;
}

// Print the family tree
void print_family(person *p, int generation)
{
    if (p == NULL)
    {
        return;
    }

    // Indentation
    for (int i = 0; i < generation * 4; i++)
    {
        printf(" ");
    }

    printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);

    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Free memory recursively
void free_family(person *p)
{
    if (p == NULL)
    {
        return;
    }

    // Free parents first
    free_family(p->parents[0]);
    free_family(p->parents[1]);

    // Then free the current person
    free(p);
}

// Randomly chooses allele: A, B, or O
char random_allele()
{
    int r = rand() % 3;
    return (r == 0) ? 'A' : (r == 1) ? 'B' : 'O';
}
