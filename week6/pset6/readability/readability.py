
def main():
    text = get_text()

    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    L = letters/words * 100
    S = sentences/words * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)

    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def count_letters(text):
    counter = 0
    for i in text:
        if i.isalpha():
            counter += 1

    return counter


def count_words(text):
    counter = 1
    for i in text:
        if i == ' ':
            counter += 1

    return counter


def count_sentences(text):
    counter = 0
    for i in text:
        if i == '!' or i == '?' or i == '.':
            counter += 1

    return counter


def get_text():
    return input("Text: ")


if __name__ == "__main__":
    main()
