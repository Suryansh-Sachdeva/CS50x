def main():
    # Prompt user for number of rows
    while True:
        try:
            rows = int(input("Enter number of rows: "))
            if rows > 0:
                break
        except ValueError:
            pass  # ignore non-integer input

    # Print the pattern
    for i in range(1, rows + 1):
        spaces = rows - i
        hashes = i
        print(" " * spaces + "#" * hashes)

if __name__ == "__main__":
    main()
