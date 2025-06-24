def main():
    # Prompt the user for a positive float (dollar input)
    while True:
        try:
            dollars = float(input("Change owed: "))
            if dollars >= 0:
                break
        except ValueError:
            continue

    # Convert dollars to cents, rounding to avoid floating-point issues
    cents = round(dollars * 100)

    coins = 0
    for coin in [25, 10, 5, 1]:
        coins += cents // coin
        cents %= coin

    print(coins)

if __name__ == "__main__":
    main()
