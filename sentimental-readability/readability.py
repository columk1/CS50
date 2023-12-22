from cs50 import get_string
import re


def main():
    # Get input text from user
    text = get_string('Text: ')
    # Print grade level (Coleman-Lau index)
    getGradeLevel(text)


def getGradeLevel(str):
    # Count the letters, words and sentences found in the string. The empty string at the end of each regex split is removed with - 1
    letters = len(re.split('[a-zA-Z]', str)) - 1
    words = len(str.split())
    sentences = len(re.split('[.!?]', str)) - 1

    # Calculate grade level using Coleman-Lau formula
    l = (letters / words) * 100
    s = (sentences / words) * 100
    grade = round(.0588 * l - 0.296 * s - 15.8)

    # Print the result
    if grade < 1:
        print('Before Grade 1')
    elif grade > 16:
        print('Grade 16+')
    else:
        print('Grade ', grade)


main()