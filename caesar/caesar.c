#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

string encrypt(string p, int k);

// A program that encrypts the user's text input using Caesar’s cipher.

int main(int argc, string argv[])
{
    // Check for single argument
    if( argc != 2 )
    {
        printf("One argument expected.\n");
        return 1;
    }

    // Check if there are non-digits in the argument
    for(int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if(!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
             return 1;
        }
    }

    // Convert the argument to an integer
    int k = atoi(argv[1]);

    // Prompt user for plaintext
    string p = get_string("plaintext: ");

    // Encrypt using the rotate function and print the result
    printf("ciphertext: %s\n", encrypt(p, k));
}

// Convert all alphabetical characters in a string using Caesar's algorithm
string encrypt(string p, int k)
{
    for(int i = 0, n = strlen(p); i < n; i++)
    {
        if islower(p[i])
        {
            p[i] = (((p[i] + k) - 97) % 26) + 97;
        }
        else if isupper(p[i])
        {
            p[i] = (((p[i] + k) - 65) % 26) + 65;
        }
    }
    return p;
}