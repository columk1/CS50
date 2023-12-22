#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int count_letters(string s);
int count_words(string s);
int count_sentences(string s);
void get_grade_level(string text);

int main(void)
{
    // Get input text from user
    string text = get_string("Text: ");
    // Print Coleman-Lau index
    get_grade_level(text);
}

// Count the number of letters in a string
int count_letters(string s)
{
    int count = 0;
    for(int i = 0, n = strlen(s); i < n; i++)
    {
        if(isalpha(s[i]))
        {
            count++;
        }
    }
    return count;
}

// Count the number of words in a string
int count_words(string s)
{
    int count = 1;
    for(int i = 0, n = strlen(s); i <n; i++)
    {
        if(isspace(s[i]))
        {
            count++;
        }
    }
    return count;
}

// Count the number of sentences in a string
int count_sentences(string s)
{
    int count = 0;
    for(int i = 0, n = strlen(s); i <n; i++)
    {
        if(isalpha(s[i-1]))
        {
            if(s[i] == '.' || s[i] == '!' || s[i] == '?')
            {
            count++;
            }
        }
    }
    return count;
}

void get_grade_level(string text)
{
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculate grade level using Coleman-Lau formula
    float l = ((float) letters / words) * 100.0;
    float s = ((float) sentences / words) * 100.0;
    int grade = round(.0588 * l - 0.296 * s - 15.8);

    // Print result
    if(grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if(grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}