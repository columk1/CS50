# CS50 Pset6 Cash

from cs50 import get_float


def main():

    # Prompt user for float and check if it is non-negative
    while True:
        change = get_float("Change owed: ")
        if change >= 0:
            getCoins(change)
            break


# Print the minimum number of coins required to make a quantity of change (arg is a float)
def getCoins(change):

    # Convert to integers
    change = change * 100
    coins = 0

    while change > 0:
        # Quarters
        if change >= 25:
            change -= 25
            coins += 1
        # Dimes
        elif change >= 10:
            change -= 10
            coins += 1
        # Nickels
        elif change >= 5:
            change -= 5
            coins += 1
        # Pennies
        else:
            change -= 1
            coins += 1

    print(coins)


if __name__ == "__main__":
    main()