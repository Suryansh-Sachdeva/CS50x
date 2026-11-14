import re
import math

def main():
    # Input paragraph
    paragraph = input("Enter a paragraph:\n")

    letters = sum(1 for c in paragraph if c.isalpha())
    words = len(paragraph.split())
    sentences = len(re.findall(r'[.!?]', paragraph))

    # Calculate L and S
    L = (letters / words) * 100
    S = (sentences / words) * 100

    # Coleman-Liau index
    index = 0.0588 * L - 0.296 * S - 15.8
    grade = round(index)

    # Output
    print("\n--- Readability Report ---")
    print(f"Letters: {letters}")
    print(f"Words: {words}")
    print(f"Sentences: {sentences}")
    print("Grade Level: ", end="")

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")

if __name__ == "__main__":
    main()
