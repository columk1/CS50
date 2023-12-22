#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Ask for user's name
    string name = get_string("What is your name? ");

    // Say hello
    printf("Hello, %s!\n", name);
}