def main():

    # Prompt for height and draw pyramid if input meets parameters
    while True:
        try:
            height = int(input('Height: '))
            if height and height > 0 and height < 9:
                drawPyramid(height)
                break
        except ValueError:
            print('Enter a number between 1 and 8.')


# Draw a pyramid of specified height using hashes for blocks
def drawPyramid(height):
    for i in range(1, height + 1):
        print(' ' * (height - i) + '#' * i + "  " + '#' * i)


main()
