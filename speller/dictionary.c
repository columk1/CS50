// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Choose number of buckets in hash table
const unsigned int N = 24389;
unsigned int word_count = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Use the hash function to find the index of the word
    int index = hash(word);

    // Declare the node at the head of that index
    node *n = table[index];

    // Loop through the linked list at this index
    while (n != NULL)
    {
        // Return true if a match is found
        if (strcasecmp(n->word, word) == 0)
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        // Multiplicative hash function based on advice found at https://www.codeproject.com/Articles/32829/Hash-Functions-An-Empirical-Comparison
        hash = (hash << 3) + toupper(word[i]);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        printf("Error: Failed to open file.\n");
        return false;
    }

    // Create a buffer using the max word length allowed by the application. The loop creates strings by stopping at each whitespace until the end of the file.
    char str[LENGTH + 1];
    while (fscanf(f, "%s", str) != EOF)
    {
        // Increment dictionary word count
        word_count++;

        // Allocate memory for a node and check if it's valid
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        // Copy scanned string to new node
        strcpy(n->word, str);
        n->next = NULL;

        // Get the array index by passing the string through the hash function
        int index = hash(str);

        // If the index does not exist yet, assign the new node to the first element
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        // If it already exists, link the current node to the first element of the index, then assign the new node to be the new first element
        else
        {
            n->next = table[index];
            table[index] = n;
        }
    }
    fclose(f);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Recursive function to free each node of a linked list
void freenode(node *n)
{
    if (n->next != NULL)
    {
        freenode(n->next);
    }
    free(n);
}

// Unloads dictionary from memory, returning true if successful
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            freenode(table[i]);
        }
    }
    return true ;
}
