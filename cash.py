def main():
    # Prompt the user until a valid non-negative integer is entered
    while True:
        try:
            cents = int(input("Change owed: "))
            if cents >= 0:
                break
        except ValueError:
            continue  # Skip invalid (non-integer) input

    coins = 0
    for coin in [25, 10, 5, 1]:
        while cents >= coin:
            cents -= coin
            coins += 1

    print(coins)

if __name__ == "__main__":
    main()
