#include <cs50.h>
#include <stdio.h>
#include <math.h>

long sum1 = 0;
long sum2 = 0;

//Luhn's Algorithm
bool checkSum(long arg)
{
    while (arg != 0)
    {
       sum1 += (arg % 10);
       arg /= 10;
       int sum2Num = (arg % 10)*2;
       if (sum2Num > 9) {
        sum2 += 1 + (sum2Num % 10);
       } else {
        sum2 += sum2Num;
       }
       arg /= 10;
    }
    long result = sum1 + sum2;
    return result % 10 == 0;
}

//Return the number of digits in the argument
int countDigits(long num)
{
    int count = 0;
    while (num > 0)
    {
        num = num / 10;
        count++;
    }
    return count;
}

//Check for recognisable number structures and print result
string checkCard(long cardNumber)
{
    int count = countDigits(cardNumber);
    int firstDigits = cardNumber / pow(10, count - 2);

    if ((count == 13 || count == 16) && firstDigits / 10 == 4) {
        return "VISA";
    }
    else if (count == 16 && firstDigits >= 51 && firstDigits <= 55) {
        return "MASTERCARD";
    }
    else if (count == 15 && (firstDigits == 34 || firstDigits == 37)) {
        return "AMEX";
    }
    else {
        return "INVALID";
    }
}

int main(void)
{
    long cardNumber = get_long("Enter credit card number: ");
    if(checkSum(cardNumber)) {
        printf("%s\n", checkCard(cardNumber));
    } else {
        printf("INVALID\n");
    }
}